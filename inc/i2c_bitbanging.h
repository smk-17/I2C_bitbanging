
#ifndef I2C_BITBANDING_H
#define I2C_BITBANDING_H

#define I2C_BITBANDING_ENABLED

#include <stdint.h>

#define I2C_BBPORT  GPIOB
#define I2C_SCLPIN  8U
#define I2C_SDAPIN  9U

#define I2C_WRITE   0x00U  //I2C write Bit= 0
#define I2C_READ    0x01U  //I2C Read Bit= 1


void I2C_BBInit();
void I2C_Start();
void I2C_RepeatedStart();
void I2C_Stop();

uint8_t I2C_ReadCond(uint8_t data);
uint8_t I2C_WriteCond(uint8_t data);


uint8_t DevicePresent(uint8_t Ack);
uint8_t I2CBusScan(void (*onfound)(uint8_t addr));


#endif 