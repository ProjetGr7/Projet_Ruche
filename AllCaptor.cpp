#include "mbed.h"
#include "DS1820.h"
#include "DHT.h"
#include "DavisAnemometer.h"
#include "HX711.h"

DS1820 ds1820(D2); // OK
Serial pc(USBTX, USBRX); //  OK
Serial sigfox(D1,D0); // pas OK
static DavisAnemometer anemometer(A1 /* wind direction */, D6 /* wind speed */); // OK
HX711 Balance(D4,D5);       // Déclaration de l'objet HX711

DHT dht22(D3, DHT22); // pas OK

int main(){
    
    float temp = 0; //temperature de la sonde
    int resultat = 0;
    int erreur=0;
    long valeur;
    long valeurTare;
    float poids;
    
    float humidite = 0, tempDHT22 = 0;
    anemometer.enable();
    valeurTare = Balance.getValue();    
    pc.printf("\r\n--Commencer--\r\n");
    //sigfox.printf("ESSAI");
    while(1){
        //ThisThread::sleep_for(2000);
        erreur = dht22.readData(); 
            
        if(!(ds1820.begin() && erreur == 0)){
            printf("ERREUR DS1820 ou DHT22\r\n");
        }
        ds1820.startConversion();
        //wait(10);
        resultat = ds1820.read(temp);
        tempDHT22 = dht22.ReadTemperature(CELCIUS); // lire temperature par DHT22
        humidite= dht22.ReadHumidity(); // lire humidite par DHT22
        switch (resultat) {
                  case 0:                 // pas d'erreur -> 'temp' comporte valeur de temperature
                      pc.printf("tempSonde = %3.1f%cC ,tempDHT22: %f%cC , humidite: %f \r\n", temp, 176,tempDHT22,176,humidite);
                     // sigfox.printf("AT$SF=%02x%02x%02x\r\n",tempDHT22,humidite,(int)temp); //envoyer le message a backend sigfox

                    break;
  
                  case 1:                 // pas de capteur present -> 'temp' n'est pas mis a jour
                      pc.printf("pas de capteur present\n\r");
                      break;
  
                  case 2:                 // erreur de CRC  -> 'temp' n'est pas mis a jour
                      pc.printf("erreur de CRC\r\n");
        }
        
        
        
        
        if(anemometer.readWindDirection() <=309 && anemometer.readWindDirection() >=306){
                printf("La direction est Nord\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed());    

            }
        else if(anemometer.readWindDirection() <=328 && anemometer.readWindDirection() >=326){
                printf("La direction est Sud\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
               
            }
        else if(anemometer.readWindDirection() <=295 && anemometer.readWindDirection() >=291){
            
                printf("La direction est Ouest\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
                
            }
            
         else if(anemometer.readWindDirection() <=324 && anemometer.readWindDirection() >=321){
            
                printf("La direction est Est\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
            }
         else if(anemometer.readWindDirection() <=262 && anemometer.readWindDirection() >=259){
            
                printf("La direction est Nord-Ouest\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
            }
         else if(anemometer.readWindDirection() <=277 && anemometer.readWindDirection() >=274){
        
                printf("La direction est Sud-Ouest\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
            }
            
         else if(anemometer.readWindDirection() <=302 && anemometer.readWindDirection() >=298){
            
                printf("La direction est Sud-Est\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
            }
         else if(anemometer.readWindDirection() <=285 && anemometer.readWindDirection() >=283){
            
                printf("La direction est Nord-Est\r\n [speed] %.2f km/h\r\n",anemometer.readWindSpeed()); 
            }
        
        valeur = Balance.getValue();                                        // On récupère la valeur du module
        poids = ((float)valeur-(float)valeurTare)/143200;         // Convertionde la valeur de l'ADC en grammes
        pc.printf("Valeur : %ld          Poids : %.3lf\n",valeur,poids);    // Affichage du poids
        
        pc.printf("----------------------------\n");
            
       sigfox.printf("AT$SF=%02x%02x%02x%02x%02x\r\n",(int)tempDHT22,(int)humidite,(int)temp,(int)anemometer.readWindSpeed(),(int) poids);

        //humidite= dht22.ReadHumidity();
        //tempDHT22 = dht22.ReadTemperature(CELCIUS); 
       // pc.printf("tempDHT22=%3.1f /humidite= %3.1f \r\n",tempDHT22,humidite);                
        
        wait(10);
        
    }
    
    
    
    
    
    
}
