
#include "i2c_bitbanging.h"
#include "MPU6050.h"

MPU6050_DATA_t raw;

int main(){

I2C_BBInit();
while (!MPU_Init()){}

while(1){

    MPU_Read(&raw);
    for (volatile uint32_t i = 0; i < 400000; i++) { }
}

    return 0;
}
