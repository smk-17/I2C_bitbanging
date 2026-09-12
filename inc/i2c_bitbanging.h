
#ifndef I2C_BITBANDING_H
#define I2C_BITBANDING_H

#include <stdint.h>
#include <stdbool.h>

#define I2C_BBPORT  GPIOB
#define I2C_BBSCLPIN  8U
#define I2C_BBSDAPIN  9U

#define I2C_WRITE   0x00U  //I2C write Bit= 0
#define I2C_READ    0x01U  //I2C Read Bit= 1


void I2C_BBInit();
void I2C_Start();
void I2C_RepeatedStart();
void I2C_Stop();

bool I2C_WriteByte(int8_t data);     
uint8_t I2C_ReadByte(bool ack);  

uint8_t I2C_ReadCond(uint8_t addr);
uint8_t I2C_WriteCond(uint8_t addr);


bool DevicePresent(uint8_t addr);

#endif //I2C-BITBANGING-H 