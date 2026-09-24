#ifndef MPU_6050_H
#define MPU_6050_H


#include "stdio.h"
#include "i2c_bitbanging.h"
#include "F446RE_RegMap.h"
#include "stdint.h"
#include "stdbool.h"

#define MPU_ADDR        0x68

#define WHO_AM_I 		0x75
#define PWR_MGMT 		0x6B

#define ACC_OUT         0X3B

typedef struct {

    int16_t ax;
    int16_t ay;
    int16_t az;

    int16_t gx;
    int16_t gy;
    int16_t gz;


}MPU6050_DATA_t ;



uint8_t MPU_Init(void);
void MPU_Read(MPU6050_DATA_t *Mpu);

#endif