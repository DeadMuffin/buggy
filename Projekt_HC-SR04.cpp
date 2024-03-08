#include <stdio.h>
#include <wiringPi.h>
#include "Projekt_HC-SR04.h"

bool flag_hcsr = true;
float Abstand_Wand = 100;

//Setup des Ultraschallsensors hcsr04
void hcsr04_setup() {
    wiringPiSetup();
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

//Messen und Nachverarbeitung der Werte des Ultraschallsensors
float hcsr04_Distanzberechnung() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    while (digitalRead(ECHO_PIN) == LOW);
    long startZeit = micros();

    while (digitalRead(ECHO_PIN) == HIGH);
    long Abstandszeit = micros() - startZeit;
   
    //Entfernung = Zeit * Geschwindigkeit/2
    float Schallgeschwindigkeit = 0.0343;
    float Distanz = Abstandszeit * Schallgeschwindigkeit / 2;

    return Distanz;
}