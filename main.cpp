#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <thread>

#include "adafruitmotorhat.h"
#include "Projekt_Menue.h"
#include "Projekt_LED.h"
#include "Projekt_HC-SR04.h"
#include "Projekt_MPU.h"
#include "Projekt_Bewegungen.h"

extern bool flag_hcsr;
extern bool flag_mpu;
extern int Richtung;
extern float Abstand_Wand;

/****************
Globale Variablen
****************/

bool kill_motor = false;
bool drove_forward = true;

//Programmende ueber STRG-C
void signalHandler(int signum) {
	std::cout << "Strg-C Programmende" << std::endl;

	delay(500);	
	flag_hcsr = false;
	flag_mpu = false;
	delay(1000);
	release();
	exit(signum);
}

/*
Stoppt den Motor solange die Distanz zur Wand noch kleiner als der Threshhold ist.
*/
void Motor_Stop() {
	kill_motor = true;
	light_on();
	delay(50);
	Break();

	while (Abstand_Wand <= 35 && flag_hcsr) {
		Break();
		Abstand_Wand = hcsr04_Distanzberechnung();
		delay(1000);
	}
	light_off();
	kill_motor = false;
}

/*
Wird benutzt um regelmäßig die Distanz zur Wand zu berechnen und falls die Nähe kritisch wird wird die Funktion Motor_Stop aufgerufen.
*/
void WiederholteDistanzberechnung() {
	std::cout << "Thread-HCSR gestartet" << std::endl;
	hcsr04_setup();
	while (flag_hcsr) {
		delay(10);
		Abstand_Wand = hcsr04_Distanzberechnung();
		if (Abstand_Wand < 35) Motor_Stop();
	}
	std::cout << "Thread-HCSR beendet" << std::endl;
}

/*
Fahren ist dafür da um gerade vorwaerts und rueckwaerts zu fahren.
Validiert wird durch die Funktion mpu6050_FahrzustandErkennung, die die Variable Richtung beschreibt und der Ausgleich erfolgt durch die Funktionen zu_weit_rechts/links
flag_gerade sagt aus, ob wir zuletzt den Kurs korrigiert haben.
delayflag wird genutzt um einen delay zu erzeugen ohne große Werte in der Delayfunktion zu haben.
kill_motor ist dafür da um den Thread zu pausieren bzw die Fahrt für einen Moment zu stoppen.
flag_mpu & flag_hcsr sagen aus, ob der MPU bzw der Ultraschall noch aktiviert sind und beenden ggf den Thread Motor.
*/
void Fahren() {
	bool flag_gerade = true;
	int delayflag = 0;
	
	while (flag_mpu && flag_hcsr) {
		if (kill_motor) {
			while (kill_motor) {
				if (!flag_mpu) break;
			}
		}
		if (Richtung == -1 && flag_mpu) {
				if (drove_forward) {	
					zu_weit_links_v();
				} 
				else { 
					zu_weit_rechts_r();
				}
			flag_gerade = false;
		}
		else if (Richtung == 1 && flag_mpu) {
			if (drove_forward) {
				zu_weit_rechts_v();
			} else { 
				zu_weit_links_r();
			}
			flag_gerade = false;
		}
		delay(10);
		if (!flag_gerade && flag_mpu)
			delayflag++;

		if (delayflag > 15 && flag_mpu) {
			
			if(drove_forward){
				Gerade();
			} else { 
				Rueck();
			}
			delayflag = 0;
			flag_gerade = true;
		}
	}
}

int main() {
	signal(SIGINT, signalHandler);
	wiringPiSetup();
	int eingabe = 0;
	bool was_running = false;
	std::thread motor;
	std::thread ultraschall(WiederholteDistanzberechnung);
	std::thread gyro(mpu6050_W3
	wiederholteAuslese);
	
	delay(100);
	logo();
	hauptmenue();
	std::cout << std::endl;

	while (eingabe != -1) {
		std::cout << "Befehl: "; 
		std::cin >> eingabe;

		switch (eingabe) {
				case 0:
				{
					std::cout << "Geradeaus und stoppen" << std::endl;
					drove_forward = true;
					Gerade();
					Fahren();
					break;
				}
				case 1:
				{
					std::cout << "Rechteck rechts" << std::endl;
					kill_motor = false;
					drove_forward = true;
					if (!motor.joinable()) {
						motor = std::thread(Fahren);
					}
					for(int i = 0; i <= 3; i++) {
						Gerade();
						delay(3000);
						kill_motor = true;
						KurveRechts();
						kill_motor = false;
					}
					kill_motor = true;
					delay(50);
					Break();
					break;				
				}
				case 2:
				{
					std::cout << "Rechteck links" << std::endl;
					kill_motor = false;
					drove_forward = true;

					if (!motor.joinable()) {
						motor = std::thread(Fahren);
					}
					for(int i = 0; i <= 3; i++){
						Gerade();
						delay(3000);
						kill_motor = true;
						KurveLinks();
						kill_motor = false;
					}
					kill_motor = true;
					delay(50);
					Break();
					break;					
				}
				case 3:
				{
					std::cout << "Selber steuern" << std::endl;
					steuerungsmenue();
					eingabe = 0;
					while (eingabe != -1) {
						char eingabechar;
						std::cout << "Befehl: "; 
						std::cin >> eingabechar;
						eingabe = int(eingabechar);
						switch (eingabe) {
						case 119: //w
						{
							drove_forward = true;
							std::cout << "Vorwaerts" << std::endl;
							if (motor.joinable()) {
								kill_motor = false;
								Gerade();	
							}
							else {
								kill_motor = false;
								motor = std::thread(Fahren);
								Gerade();
							}
							was_running = true;
							break;
							
						}
						case 115: //s
						{
							std::cout << "Rueckwaerts" << std::endl;
							drove_forward = false;
							if (motor.joinable()) {
								kill_motor = false;
								Rueck();	
							}
							else {
								kill_motor = false;
								motor = std::thread(Fahren);
								Rueck();
							}
							was_running = true;
							break;
						
						}
						case 100: //d
						{
							std::cout << "Rechts" << std::endl;
							kill_motor = true;
							delay(10);
							KurveRechts();
							Break();
							if(was_running){
								kill_motor = false;
							}
							break;
						}
						case 97:  //a
						{
							std::cout << "Links" << std::endl;
							kill_motor = true;
							delay(10);
							KurveLinks();
							Break();
							if(was_running){
								kill_motor = false;
							}
							break;
						}
						case 120: //x
						{
							std::cout << "Stopp" << std::endl;
							kill_motor = true;
							was_running = false;
							delay(100);
							Break();
							
							break;
						}
						case 109: //m
						{
							steuerungsmenue();
							break;
						}
						default: 
						{
							shrek();
							break;
						}
					}
				}				
				kill_motor = true;
				delay(50);
				Break();
				break;
				}
				case 4:
				{
					hauptmenue();
					break;
				}
				case 5:
				{
					logo();
					break;
				}
				default:
				{
				shrek();
				break;
			}
		}
	}
	return 0;
}