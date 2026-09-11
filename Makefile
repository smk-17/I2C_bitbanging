CC = arm-none-eabi-gcc #CrosCompiler
OBJCOPY = arm-none-eabi-objcopy  #ObjDup

TARGET = BareMetal
LDSCRIPT = Linkerdescription.ld

CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -O0 -g -Wall -ffreestanding -nostdlib -Iinc. #0 optimisation and GDB Debugging

SRCS = src/main.c src/i2c_bitbanging.c startup_stm32f446RE.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET).elf $(TARGET).bin  #Creation of .elf and .bin file

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@   

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -T $(LDSCRIPT) -nostartfiles $(OBJS) -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm -f src/*.o *.elf *.bin  #Removing the Object and Build files          

.PHONY: all clean