#include "mbed.h"
#include "DS1820.h"
#include "DHT.h"
  
  
 //Code pour DS18B20
  Serial      pc(USBTX, USBRX);
  DHT sensor(D3, DHT22);
  Serial sigfox(D1,D0);

  DigitalOut  led(LED1);
  DS1820      ds1820(D4);  // substitute D4 with actual mbed pin name connected to 1-wire bus
  float       temp = 0;
  int         result = 0;
  
  int main()
  {
    int error = 0;
    int h = 0, c = 0;
    
      pc.printf("\r\n--Starting--\r\n");
         error = sensor.readData(); 
        if (error == 0)                            
        {
            c   = sensor.ReadTemperature(CELCIUS);
            h   = sensor.ReadHumidity();
            
            
            //pc.printf("temperature %d\n", c);
            //pc.printf("humidite %d\n,", h);
        } 
        else
        {
            printf("Error: %d\n", error);
        } 
      if (ds1820.begin()) {
          while (1) {
              ds1820.startConversion();   // start temperature conversion from analog to digital
              //ThisThread::sleep_for(1000);// let DS1820 complete the temperature conversion
              wait(10);
              result = ds1820.read(temp); // read temperature from DS1820 and perform cyclic redundancy check (CRC)
              switch (result) {
                  case 0:                 // no errors -> 'temp' contains the value of measured temperature
                      //pc.printf("temp = %3.1f%cC\r\n", temp, 176);
                      sigfox.printf("AT$SF=%02x%02x%02x\r\n",c,h,(int)temp);
                    break;
  
                  case 1:                 // no sensor present -> 'temp' is not updated
                      pc.printf("no sensor present\n\r");
                      break;
  
                  case 2:                 // CRC error -> 'temp' is not updated
                      pc.printf("CRC error\r\n");
              }
  
              led = !led;
          }
      }
      else
         pc.printf("No DS1820 sensor found!\r\n");
        
  }