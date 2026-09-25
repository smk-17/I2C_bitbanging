CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ARCH_FLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard

CFLAGS  = $(ARCH_FLAGS) -g -Wall -Iinc
LDFLAGS = $(ARCH_FLAGS) -nostartfiles -T Linkerdescription.ld -lgcc

OBJS = src/main.o src/i2c_bitbanging.o src/mpu6050.o startup_stm32f446RE.o

all: BareMetal.elf BareMetal.bin

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

BareMetal.elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

BareMetal.bin: BareMetal.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f src/*.o *.o *.elf *.bin

.PHONY: all clean