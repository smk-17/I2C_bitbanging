#include "MPU6050.h"


static uint8_t writebyte(uint8_t reg, uint8_t value){

    I2C_Start();
   uint8_t ok =  I2C_WriteByte(MPU_ADDR<<1);

    I2C_WriteByte(reg);
    I2C_WriteByte(value);
    I2C_Stop();
    return ok;

}

uint8_t MPU_Init(void){

    uint8_t status = writebyte(PWR_MGMT, 0x01);
    return status;
}

void MPU_Read(MPU6050_DATA_t* mpu, MPU6050_DATA_t* mpu2){
   
    uint8_t buffer[14]={0};
   I2C_Start();
   I2C_WriteByte(MPU_ADDR<<1);
   I2C_WriteByte(ACC_OUT);
   I2C_RepeatedStart();
   I2C_WriteByte((MPU_ADDR<<1)|1);

    for (int i=0 ; i<14;i++){

        buffer[i]= I2C_ReadByte(i<13);
}
I2C_Stop();

mpu->ax = (int16_t)((buffer[0]<<8) | buffer[1]);
mpu->ay = (int16_t)((buffer[2]<<8) | buffer[3]);
mpu->az = (int16_t)((buffer[4]<<8) | buffer[5]);

mpu->gx = (int16_t)((buffer[8]<<8) | buffer[9]);
mpu->gy = (int16_t)((buffer[10]<<8) | buffer[11]);
mpu->gz = (int16_t)((buffer[12]<<8) | buffer[13]);

mpu2->ax = (float)((mpu->ax)/16384.0f);
mpu2->ay = (float)((mpu->ay)/16384.0f);
mpu2->az = (float)((mpu->az)/16384.0f);

mpu2->gx = (float)((mpu->gx)/131.0f);
mpu2->gy = (float)((mpu->gy)/131.0f);
mpu2->gz = (float)((mpu->gz)/131.0f);


}