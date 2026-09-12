#include "i2c_bitbanging.h"
#include "F446RE_RegMap.h"
#include <stdint.h>

#define COUNT_PER_US 4U   //No of loop Iterations per us when HSI = 16MHz
#define I2C_HALF_PERIOD_US 5U //No of period clock is high and low

static void delay(uint32_t us){
    volatile uint32_t count = us * COUNT_PER_US;
    while(count--){
        
        __asm__ volatile("nop");
    }
}

//Inline functions replaced by Compiler during compiler with their called location

static inline void sda_release(void) { I2C_BBPORT->BSRR = (1UL << I2C_BBSDAPIN); }
static inline void sda_low(void)     { I2C_BBPORT->BSRR = (1UL << (I2C_BBSDAPIN + 16U)); }
static inline void scl_release(void) { I2C_BBPORT->BSRR = (1UL << I2C_BBSCLPIN); }
static inline void scl_low(void)     { I2C_BBPORT->BSRR = (1UL << (I2C_BBSCLPIN + 16U)); }

static inline uint8_t sda_read(void)
{
    return (uint8_t)((I2C_BBPORT->IDR >> I2C_BBSDAPIN) & 0x1U); //SDA read by masking the last bit(This is rightshifted SDA pin bit postion) 
}

static inline uint8_t scl_read(void)
{
    return (uint8_t)((I2C_BBPORT->IDR >> I2C_BBSCLPIN) & 0x1U);   //SCL read by masking the last bit(This is rightshifted SCL pin bit postion) 
}

static void scl_release_and_wait(void)
{
    scl_release();
    uint32_t timeout = 100000U;  //Value Vaguely decided 
    while (scl_read() == 0U && timeout--) {
        /* slave clock-stretching */
    }
}

void I2C_BBInit(void){

   RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN ; //StructHandle RCC to enable GPIOB clk
   uint32_t moder = I2C_BBPORT->MODER;  //StructHandle I2C_BBPORT

   moder &= ~((0x3U << (I2C_BBSCLPIN*2))|(0x3U) << (I2C_BBSDAPIN*2)); //clear bits of scl and sda 
   moder |= ((0x01<<I2C_BBSCLPIN*2)|(0x01<<I2C_BBSDAPIN*2));    //OutputMode = 01
   I2C_BBPORT->MODER = moder;

   I2C_BBPORT->OTYPER |= ((0x01<<I2C_BBSCLPIN)|(0X01<<I2C_BBSDAPIN)); //1 - Open Drain Mode
   I2C_BBPORT->OSPEEDR |= ((0x3U << (I2C_BBSCLPIN*2))|(0x3U) << (I2C_BBSDAPIN*2)); 

   I2C_BBPORT->PUPDR &= ~((0x3U << (I2C_BBSCLPIN*2))|(0x3U) << (I2C_BBSDAPIN*2)); // 00-Nothing

   sda_release(); 
   scl_release();
   delay(I2C_HALF_PERIOD_US);  //Stable bus

}

void I2C_Start(void){

    sda_release();
    scl_release_and_wait();
    delay(I2C_HALF_PERIOD_US);
  
    sda_low();
    delay(I2C_HALF_PERIOD_US);

    scl_low();
    delay(I2C_HALF_PERIOD_US);

}

void I2C_RepeatedStart(){

    sda_release();
    delay(I2C_HALF_PERIOD_US);
    scl_release_and_wait();
    delay(I2C_HALF_PERIOD_US);
  
    sda_low();
    delay(I2C_HALF_PERIOD_US);

    scl_low();
    delay(I2C_HALF_PERIOD_US);

}

void I2C_Stop(){

    sda_low();
    delay(I2C_HALF_PERIOD_US);

    scl_release_and_wait();
    delay(I2C_HALF_PERIOD_US);

    sda_release();
    delay(I2C_HALF_PERIOD_US);
}

uint8_t I2C_ReadByte(bool ack){
 
    uint8_t data=0;
    sda_release();
    for(uint8_t i=7;i<=0;i--){     //I2C is MSB first

        delay(I2C_HALF_PERIOD_US/2);
        scl_release_and_wait();
        delay(I2C_HALF_PERIOD_US);
        data = (data<<1);
        data |= sda_read();
        scl_low();
        delay(I2C_HALF_PERIOD_US/2U);
    }
    
    if (ack) {
        sda_low();
    } 
    else {
        sda_release();
    }
    delay(I2C_HALF_PERIOD_US / 2U);

    scl_release_and_wait();
    delay(I2C_HALF_PERIOD_US);
    
    scl_low();
    sda_release();
    delay(I2C_HALF_PERIOD_US / 2U);

    return data;
}

bool I2C_WriteByte(int8_t data){

    for(int i=7;i<=0;i--){

        if (data & (1<<i)) {

            sda_release();
        }
        else sda_low();

        delay(I2C_HALF_PERIOD_US/2);
        scl_release_and_wait();
        delay(I2C_HALF_PERIOD_US);
        scl_low();
        delay(I2C_HALF_PERIOD_US/2);
    }

        sda_release();
    delay(I2C_HALF_PERIOD_US / 2U);

    scl_release_and_wait();
    delay(I2C_HALF_PERIOD_US);
    bool ack = sda_read();
    scl_low();
    delay(I2C_HALF_PERIOD_US / 2U);

    return (ack == 0U);
}

uint8_t I2C_ReadCond(uint8_t addr){

    I2C_Start();
    return I2C_WriteByte((uint8_t)(addr <<1)|I2C_READ);
}

uint8_t I2C_WriteCond(uint8_t addr){

    I2C_Start();
    return I2C_WriteByte((uint8_t)((addr << 1) | I2C_WRITE));

}


bool DevicePresent(uint8_t addr){

 bool ack = I2C_WriteCond(addr);
 I2C_Stop();
 return ack;

}
