#include <stdint.h>

#define SRAM_START  0x20000000U
#define SRAM_SIZE   (128U * 1024U)
#define SRAM_END    ((SRAM_START) + (SRAM_SIZE))

#define STACK_START SRAM_END

// Defined in the linker script 
extern uint32_t _sidata;   
extern uint32_t _sdata;    
extern uint32_t _edata;    
extern uint32_t _sbss;    
extern uint32_t _ebss;     

int main(void);

void Default_Handler(void);
void Reset_Handler(void);

// Every exception falls back to Default_Handler No interrupts needed for my code

void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

// Vector table: 16 core entries.

__attribute__((section(".isr_vector"), used))

const uint32_t vectors[16] = {
    STACK_START,                    // 0  initial stack pointre 
    (uint32_t)&Reset_Handler,       // 1  reset                 
    (uint32_t)&NMI_Handler,         // 2                        
    (uint32_t)&HardFault_Handler,   // 3                       
    (uint32_t)&MemManage_Handler,   // 4                        
    (uint32_t)&BusFault_Handler,    // 5                        
    (uint32_t)&UsageFault_Handler,  // 6                        
    0, 0, 0, 0,                     // 7-10 reserved            
    (uint32_t)&SVC_Handler,         // 11                       
    (uint32_t)&DebugMon_Handler,    // 12                       
    0,                              // 13 reserved              
    (uint32_t)&PendSV_Handler,      // 14                       
    (uint32_t)&SysTick_Handler      // 15                       
};



void Reset_Handler(void)
{
    //Enable the FPU 

    *(volatile uint32_t *)0xE000ED88UL |= (0xFUL << 20);
    __asm__ volatile("dsb\n isb");

    // Copying .data from flash to RAM 
    
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    // Zeroing  .bss
    for (dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0U;
    }

    //Call Application main()
    main();

    while (1) { }
}

void Default_Handler(void)
{
    while (1) { }
}