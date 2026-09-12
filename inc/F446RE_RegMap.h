/*
 * stm32-Nucleo-f446RE_RegMap

 * Minimal,register map for STM32F401RE.
 * Only what's needed for GPIO bit-banging (RCC + GPIO blocks).
 * No CMSIS / ST HAL dependency
 */

#ifndef STM32F446_REGS_H
#define STM32F446_REGS_H

#include <stdint.h>

#define PERIPH_BASE          (0x40000000UL) //APB1 Base 
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL) //AHB1 PeripheralBase

#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL) 
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800UL)

//Structs To access the different (all) registers associated with an Individual GPIO port

typedef struct {
    volatile uint32_t MODER;    /* 0x00 mode register            */
    volatile uint32_t OTYPER;   /* 0x04 output type register     */
    volatile uint32_t OSPEEDR;  /* 0x08 output speed register    */
    volatile uint32_t PUPDR;    /* 0x0C pull-up/pull-down reg    */
    volatile uint32_t IDR;      /* 0x10 input data register      */
    volatile uint32_t ODR;      /* 0x14 output data register     */
    volatile uint32_t BSRR;     /* 0x18 bit set/reset register   */
    volatile uint32_t LCKR;     /* 0x1C config lock register     */
    volatile uint32_t AFR[2];   /* 0x20-0x24 alternate function  */
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    uint32_t RESERVED0[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;   /* 0x30 AHB1 peripheral clock enable */
    volatile uint32_t AHB2ENR;
    uint32_t RESERVED2[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

//Peripheral Structure Handle Macros

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)  
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE   ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOH   ((GPIO_TypeDef *)GPIOH_BASE)
#define RCC    ((RCC_TypeDef *)RCC_BASE)

//Enabling the GPIO Clock for each GPIO Peripheral PORT

#define RCC_AHB1ENR_GPIOAEN   (1UL << 0)
#define RCC_AHB1ENR_GPIOBEN   (1UL << 1)
#define RCC_AHB1ENR_GPIOCEN   (1UL << 2)
#define RCC_AHB1ENR_GPIODEN   (1UL << 3)
#define RCC_AHB1ENR_GPIOEEN   (1UL << 4)
#define RCC_AHB1ENR_GPIOHEN   (1UL << 7)

#define GPIO_MODE_INPUT       0x0U
#define GPIO_MODE_OUTPUT      0x1U
#define GPIO_MODE_AF          0x2U
#define GPIO_MODE_ANALOG      0x3U

#define GPIO_OTYPE_PP         0x0U   /* push-pull   */
#define GPIO_OTYPE_OD         0x1U   /* open-drain  */

#define GPIO_PUPD_NONE        0x0U
#define GPIO_PUPD_PU          0x1U
#define GPIO_PUPD_PD          0x2U

#endif /* STM32F446_REGS_H */

