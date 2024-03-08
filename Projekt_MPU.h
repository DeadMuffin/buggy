#pragma once

#include <stdint.h>

//Allgemeine Makros
#define ENABLE 									1
#define DISABLE 								0
#define SET 									ENABLE
#define RESET 									DISABLE
#define GPIO_PIN_SET        					SET
#define GPIO_PIN_RESET      					RESET

//Basisaddressen
#define MPU6050_ADDRESS 0x68
#define ADDRESS_AMOUNT 40

//Map-Konfiguration (Access Param)
#define SELF_TEST_X             0
#define SELF_TEST_Y             1
#define SELF_TEST_Z             2
#define SELF_TEST_A             3
#define PWR_MGMT_1              4
#define PWR_MGMT_2              5
#define FIFO_COUNTH             6
#define FIFO_COUNTL             7
#define FIFO_R_W                8
#define USER_CTRL               9
#define SIGNAL_PATH_RESET       10
#define I2C_MST_DELAY_CTRL      11
#define I2C_SLV0_DO             12
#define I2C_SLV1_DO             13
#define I2C_SLV2_DO             14
#define I2C_SLV3_DO             15
#define INT_PIN_CFG             16
#define INT_ENABLE              17
#define I2C_SLV4_CTRL           18
#define I2C_SLV4_DO             19
#define I2C_SLV4_REG            20
#define SMPLRT_DIV              21
#define CONFIG                  22
#define GYRO_CONFIG             23
#define ACCEL_CONFIG            24
#define FIFO_EN                 25
#define I2C_MST_CTRL            26
#define I2C_SLV0_ADDR           27
#define I2C_SLV0_REG            28
#define I2C_SLV0_CTRL           29
#define I2C_SLV1_ADDR           30
#define I2C_SLV1_REG            31
#define I2C_SLV1_CTRL           32
#define I2C_SLV2_ADDR           33
#define I2C_SLV2_REG            34
#define I2C_SLV2_CTRL           35
#define I2C_SLV3_ADDR           36
#define I2C_SLV3_REG            37
#define I2C_SLV3_CTRL           38
#define I2C_SLV4_ADDR           39

//Interne Datenstruktur zur Verwaltung des MPU6050s
typedef struct {
    int8_t reg_config;
    uint8_t address;
} data;

//Aue�ere Datenstruktur zur Verwaltung des MPU6050s
typedef struct {
    data access[ADDRESS_AMOUNT];
} MPU6050_Register;
extern MPU6050_Register mpu6050;

//Datentyp f�r Accelerometer Output
enum {
    MPU6050_REG_ACCEL_XOUT_H = 0x3B,
    MPU6050_REG_ACCEL_XOUT_L = 0x3C,
    MPU6050_REG_ACCEL_YOUT_H = 0x3D,
    MPU6050_REG_ACCEL_YOUT_L = 0x3E,
    MPU6050_REG_ACCEL_ZOUT_H = 0x3F,
    MPU6050_REG_ACCEL_ZOUT_L = 0x40
} MPU6050_AccelerometerOutput_t;

//Datentyp f�r Temperatur Output
enum {
    MPU6050_TEMP_OUT_H = 0x41,
    MPU6050_TEMP_OUT_L = 0x42
} MPU6050_TemperatureOutput_t;

//Datentyp f�r Gyroskop Output
enum {
    MPU6050_REG_GYRO_XOUT_H = 0x43,
    MPU6050_REG_GYRO_XOUT_L = 0x44,
    MPU6050_REG_GYRO_YOUT_H = 0x45,
    MPU6050_REG_GYRO_YOUT_L = 0x46,
    MPU6050_REG_GYRO_ZOUT_H = 0x47,
    MPU6050_REG_GYRO_ZOUT_L = 0x48
} MPU6050_GyroscopeOutput_t;

//Initialisierung des Sensors
void mpu6050_init(int fd);

//Kalibrierungshilfe des Sensors
void mpu6050_calibrate(int fd);

//Auslesen der Daten
void mpu6050_GyroAuslesen(int fd);
int get_gyroZ();

//Reset
void mpu6050_reset(int fd);

//Erkennung ob Fahrrichtung links, rechts oder gerade
int mpu6050_FahrzustandErkennung();

//Wiederholtes Auslesen der Gyro-Messwerte
void mpu6050_WiederholteAuslese();

void mpu6050_Setup();


