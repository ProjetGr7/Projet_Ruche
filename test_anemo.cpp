#include "mbed.h"

AnalogIn girouette(A0);
DigitalOut  led(LED2);
DigitalIn anemo(D4);
int main()
{
    while (1) {
        printf("tension: %0.1f\r\n", girouette.read()*500);
        printf("num: %d\r\n", anemo.read());
        led = !led;
        wait(0.5);
    }
}