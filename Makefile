all: BareMetal.elf BareMetal.bin

%.o: %.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -g -Wall -Iinc -c $< -o $@

BareMetal.elf: src/main.o src/i2c_bitbanging.o src/mpu6050.o startup_stm32f446RE.o
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -nostdlib -nostartfiles -T Linkerdescription.ld $^ -o $@

BareMetal.bin: BareMetal.elf
	arm-none-eabi-objcopy -O binary $< $@

clean:
	rm -f src/*.o *.o *.elf *.bin

.PHONY: all clean