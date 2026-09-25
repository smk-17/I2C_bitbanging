
#include "i2c_bitbanging.h"
#include "MPU6050.h"

//Only reason is for plotting results MCUviewer

volatile float ax_plot;
volatile float ay_plot;
volatile float az_plot;

volatile float gx_plot;
volatile float gy_plot;
volatile float gz_plot;

MPU6050_DATA_t raw;
MPU6050_DATA_t data;

int main(){

I2C_BBInit();
while (!MPU_Init()){}

while(1){

    MPU_Read(&raw,&data);

    ax_plot=data.ax;
    ay_plot=data.ay;
    az_plot=data.az;

    gx_plot=data.gx;
    gy_plot=data.gy;
    gz_plot=data.gz;


    for (volatile uint32_t i = 0; i < 400000; i++) { }
}

    return 0;
}
