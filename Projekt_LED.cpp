#include <wiringPi.h>

void light_on()
{
    wiringPiSetup();
    pinMode(4, OUTPUT);
    digitalWrite(4, HIGH);
    delay(500);
}

void light_off()
{
    wiringPiSetup();
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    delay(500);
    pinMode(4, INPUT);
}