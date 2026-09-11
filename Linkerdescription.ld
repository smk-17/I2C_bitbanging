ENTRY(Reset_Handler)

MEMORY 
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K  /* Read-only code & constants */ 
  SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 116K   /* Read, write, execute */
  SRAM2 (rwx) : ORIGIN = 0x2001C000, LENGTH = 16K    /* Read, write, execute */
}


__min__heap_size = 0x400;  /*1024Bytes */
__min__stack_size = 0x200;  /* 512Bytes */

SECTIONS 
{

  .text :
  { 
    KEEP(*(.isr_vector))  /* Keeps vector table from being garbage-collected */
    *(.text)              /* Program code */
    *(.text.*)           
    *(.rodata)            /* Read-only data (const, string literals) */
    *(.rodata.*)
    . = ALIGN(4);     /* Memory is word aligned */
    _etext = .;           /* End of .text section */
  } > FLASH    /* In .text LMA=VMA */

  /* Initialized data loaded in FLASH (LMA), copied to SRAM1 (VMA) at startup
 Excecution address from where the var are fetched during runtime 
 LMA - Storaged address in falsh for code and innitial values of variables  */
 
  
  .data :
  {
    _sdata = .;           /* Start of .data in SRAM */
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _edata = .;           /* End of .data in SRAM */ 
  } > SRAM1 AT> FLASH

  /* Export LMA location of .data so Startup Code knows where to copy from in FLASH
   (si= start of initialised data in Flash) */

  _sidata = LOADADDR(.data);

  /* Uninitialized data allocated directly in SRAM1 and zeroed out at startup  */
  .bss :
  {
    _sbss = .;            /* Start of .bss in SRAM */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;            /* End of .bss in SRAM */
    __bss_end__ = _ebss;
  } > SRAM1
   
   .heap :
  {
    . = ALIGN(8);
    _sheap = .;
    . = . + __min__heap_size;
    _eheap = .;
  
  } > SRAM1

  .stack :   /* start of stack is always the end of SRAM (Top most ) */
  {
    . = ALIGN(8);
    . = . + __min__stack_size;
    _estack = .;  /*Top of RAM stack passed to Vector Table */ 
  } > SRAM1


}
