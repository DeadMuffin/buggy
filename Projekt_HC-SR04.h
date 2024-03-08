#pragma once

//Einstellen der Pins
#define TRIG_PIN 3 // 22 BCM
#define ECHO_PIN 2 // 27 BCM

//Setup des Ultraschallsensors
void hcsr04_setup();

//Messen und Nachverarbeitung der Werte
float hcsr04_Distanzberechnung();