#include <signal.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <cstdlib>
#include <iostream>

#include "adafruitmotorhat.h"
#include "adafruitdcmotor.h"
#include "Projekt_Bewegungen.h"
#include "Projekt_MPU.h"

int motor_speed = 100;
AdafruitMotorHAT hat;
std::shared_ptr<AdafruitDCMotor> leftengine = hat.getMotor(1);
std::shared_ptr<AdafruitDCMotor> rightengine = hat.getMotor(4);

void release() {    
    std::cout << "Release" << std::endl;
    leftengine->run(AdafruitDCMotor::kRelease);
    rightengine->run(AdafruitDCMotor::kRelease);
}

void Break() {
    leftengine->run(AdafruitDCMotor::kBrake);
    rightengine->run(AdafruitDCMotor::kBrake);
}

void Gerade() { 
    leftengine->setSpeed(motor_speed);
    rightengine->setSpeed(motor_speed);

    leftengine->run(AdafruitDCMotor::kForward);
    rightengine->run(AdafruitDCMotor::kForward);
}

void Rueck() {
    leftengine->setSpeed(motor_speed);
    rightengine->setSpeed(motor_speed);

    leftengine->run(AdafruitDCMotor::kBackward);
    rightengine->run(AdafruitDCMotor::kBackward);
}


void KurveRechts() {
    leftengine->setSpeed(motor_speed);
    rightengine->setSpeed(0);
    leftengine->run(AdafruitDCMotor::kForward);
    rightengine->run(AdafruitDCMotor::kForward);

    delay(250);    
    float Winkelgeschwindigkeit = -1 * get_gyroZ();
    int motorRunTime = ((90 / Winkelgeschwindigkeit)* 1000);
    delay(motorRunTime-300);
}

void KurveLinks() {
    
    leftengine->setSpeed(0);
    rightengine->setSpeed(motor_speed);
    leftengine->run(AdafruitDCMotor::kForward);
    rightengine->run(AdafruitDCMotor::kForward);

    delay(250);
    
    float Winkelgeschwindigkeit = get_gyroZ();
    int motorRunTime = ((90 / Winkelgeschwindigkeit)* 1000);
    delay(motorRunTime-300);  
}

void zu_weit_links_r(){
    leftengine->setSpeed(motor_speed);
    rightengine->setSpeed(motor_speed - motor_speed/5);
    leftengine->run(AdafruitDCMotor::kBackward);
    rightengine->run(AdafruitDCMotor::kBackward);
}

void zu_weit_rechts_r(){
    leftengine->setSpeed(motor_speed - motor_speed/5);
    rightengine->setSpeed(motor_speed);
    leftengine->run(AdafruitDCMotor::kBackward);
    rightengine->run(AdafruitDCMotor::kBackward);
}

void zu_weit_rechts_v() {
    leftengine->setSpeed(motor_speed - motor_speed/5);
    rightengine->setSpeed(motor_speed);
    leftengine->run(AdafruitDCMotor::kForward);
    rightengine->run(AdafruitDCMotor::kForward);
}

void zu_weit_links_v() {
    leftengine->setSpeed(motor_speed);
    rightengine->setSpeed(motor_speed - motor_speed/5);
    leftengine->run(AdafruitDCMotor::kForward);
    rightengine->run(AdafruitDCMotor::kForward);
}