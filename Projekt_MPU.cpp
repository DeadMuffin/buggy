#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>
#include <string>
#include <math.h>

#include "Projekt_MPU.h"

/****************
Globale Variablen
****************/

int gyroX, gyroY, gyroZ;
int Richtung = 0;
bool flag_mpu = true;

//Vorgefertigtes Zugriffsregister für den MPU6050
MPU6050_Register mpu6050 = {
    .access = {
        {-1, 0x0D},
        {-1, 0x0E},
        {-1, 0x0F},
        {-1, 0x10},
        {-1, 0x6B},
        {-1, 0x6C},
        {-1, 0x72},
        {-1, 0x73},
        {-1, 0x74},
        {-1, 0x6A},
        {-1, 0x68},
        {-1, 0x67},
        {-1, 0x63},
        {-1, 0x64},
        {-1, 0x65},
        {-1, 0x66},
        {-1, 0x37},
        {-1, 0x38},
        {-1, 0x34},
        {-1, 0x33},
        {-1, 0x32},
        {-1, 0x19},
        {-1, 0x1A},
        {-1, 0x1B},
        {-1, 0x1C},
        {-1, 0x23},
        {-1, 0x24},
        {-1, 0x25},
        {-1, 0x26},
        {-1, 0x27},
        {-1, 0x28},
        {-1, 0x29},
        {-1, 0x2A},
        {-1, 0x2B},
        {-1, 0x2C},
        {-1, 0x2D},
        {-1, 0x2E},
        {-1, 0x2F},
        {-1, 0x30},
        {-1, 0x31}
    }
};

//Samplemenge und Empfindlichkeit
#define CALIBRATION_SAMPLES 1000
#define GYRO_SCALE_FACTOR 131.0 

//Kalibrierte Offsets
int gyroOffsetX = -14;
int gyroOffsetY = 2;
int gyroOffsetZ = 0;

int get_gyroZ() {
    return gyroZ;
}

void mpu6050_init(int fd) {
    std::cout << "mpu6050_init gestartet..." << std::endl;
    for (int i = 0; i < ADDRESS_AMOUNT; i++) {
        if (mpu6050.access[i].reg_config > -1) {
            std::cout << "Beschreiben von Adresse " << mpu6050.access[i].address << std::endl;
            wiringPiI2CWriteReg8(fd, mpu6050.access[i].address, mpu6050.access[i].reg_config);
            delayMicroseconds(100);
        }
    }
    std::cout << "mpu6050_init abgeschlossen..." << std::endl;
}

void mpu6050_reset(int fd) {
    std::cout << "mpu6050_reset gestartet..." << std::endl;
    uint8_t Device_RESET = 0b10000000;
    wiringPiI2CWriteReg8(fd, mpu6050.access[PWR_MGMT_1].address, Device_RESET);
    delayMicroseconds(100);
    uint8_t TGA_RESET = 0b00000111;
    wiringPiI2CWriteReg8(fd, mpu6050.access[SIGNAL_PATH_RESET].address, TGA_RESET);
    std::cout << "mpu6050_reset abgeschlossen..." << std::endl;
}

void mpu6050_calibrate(int fd) {
    int sumX = 0, sumY = 0, sumZ = 0;

    std::cout << "mpu6050_calibrate gestartet..." << std::endl;
    for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
        mpu6050_GyroAuslesen(fd);
        sumX += gyroX;
        sumY += gyroY;
        sumZ += gyroZ;
        delayMicroseconds(5);
    }

    //Durchschnittliche Offset-Werte berechnen und ausgeben
    gyroOffsetX = sumX / CALIBRATION_SAMPLES;
    gyroOffsetY = sumY / CALIBRATION_SAMPLES;
    gyroOffsetZ = sumZ / CALIBRATION_SAMPLES;

    std::cout << "mpu6050_calibrate abgeschlossen." << std::endl;
    std::cout << "Gyro-Offset X: " << gyroOffsetX   << std::endl;
    std::cout << "Gyro-Offset Y: " << gyroOffsetY   << std::endl;
    std::cout << "Gyro-Offset Z: " << gyroOffsetZ   << std::endl;
}

void mpu6050_GyroAuslesen(int fd) {
    //Rohdaten auslesen und Offset beruecksichtigen
    short rawGyroXH = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_XOUT_H) - gyroOffsetX;
    short rawGyroXL = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_XOUT_L) - gyroOffsetX;
    rawGyroXH = rawGyroXH << 8;
    int rawGyroX = rawGyroXH + rawGyroXL;
  
    short rawGyroYH = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_YOUT_H) - gyroOffsetY;
    short rawGyroYL = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_YOUT_L) - gyroOffsetY;
    rawGyroYH = rawGyroYH << 8;
    int rawGyroY = rawGyroYH + rawGyroYL;

    short rawGyroZH = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_ZOUT_H) - gyroOffsetZ;
    short rawGyroZL = wiringPiI2CReadReg8(fd, MPU6050_REG_GYRO_ZOUT_L) - gyroOffsetZ;
    rawGyroZH = rawGyroZH << 8;
    int rawGyroZ = rawGyroZH + rawGyroZL;
 
  
    gyroX = (float) rawGyroX / GYRO_SCALE_FACTOR;
    gyroY = (float) rawGyroY / GYRO_SCALE_FACTOR;
    gyroZ = (float) rawGyroZ / GYRO_SCALE_FACTOR;
}

int mpu6050_FahrzustandErkennung() {
    //Schwellenwerte fuer die Erkennung einer Kurve
    const int DrehSchwelle = 5; 

    if (gyroZ > DrehSchwelle) {
        return -1; //links
    }
    else if (gyroZ < -DrehSchwelle) {
        return 1; //rechts
    }
    else {
        return 0; //geradeaus
    }
}

void mpu6050_WiederholteAuslese() {
   std::cout << "Thread-Gyro gestartet" << std::endl;
    int gyrofd = wiringPiI2CSetup(0x68);
    mpu6050_Setup();
    mpu6050_init(gyrofd);
    while (flag_mpu) {
        delay(10);
        mpu6050_GyroAuslesen(gyrofd);
        Richtung = mpu6050_FahrzustandErkennung();
    }
   std::cout << "Thread-Gyro verlassen" << std::endl;
}


void mpu6050_Setup(){
    mpu6050.access[PWR_MGMT_1].reg_config = 0x00;
    mpu6050.access[GYRO_CONFIG].reg_config = 0x00;
}