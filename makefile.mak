#TOOLCHAIN SETUP
# Define the prefix for the GNU Arm Embedded Toolchain
PREFIX  = arm-none-eabi-  #acts as a place holder
CC      = $(PREFIX)gcc
AS      = $(PREFIX)gcc -x assembler-with-cpp
CP      = $(PREFIX)objcopy
SZ      = $(PREFIX)size

#FILE AND DIRECTORY CONFIGURATION
TARGET   = i2c_bitbang_project
BUILD_DIR = build

# Automatically find all C source files in the current folder
SRCS     = main.c i2c_bitbang.c startup_stm32f446.c
# Convert source file names into object file paths inside the build directory
OBJS     = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))

#HARDWARE ARCHITECTURE FLAGS (STM32F446RE: Cortex-M4 with FPU)
MCU_FLAGS = -mcpu=cortex-m4 \
            -mthumb \
            -mfloat-abi=hard \
            -mfpu=fpv4-sp-d16

# 4. COMPILER FLAGS
# -O0: No optimization (makes debugging easier line-by-line)
# -g3: Generates deep debugging information for GDB
# -ffreestanding: Tells compiler that a standard OS environment does not exist
# -nostdlib: Do not use standard C libraries
CFLAGS    = $(MCU_FLAGS) \
            -O0 \
            -g3 \
            -Wall \
            -ffreestanding \
            -nostdlib \
            -I.

# 5. LINKER FLAGS
LINK_SCRIPT = stm32f446xx.ld
LDFLAGS     = -T $(LINK_SCRIPT) \
              -nostdlib \
              -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref,--gc-sections

# ==============================================================================
# BUILD TARGETS AND RULES
# ==============================================================================

# Default rule executed when you just type 'make'
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Rule to compile C source files into Object files (.o)
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling: $<"
	@$(CC) -c $(CFLAGS) $< -o $@

# Rule to link Object files together into the final ELF execution binary
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "Linking binary file: $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@
	@echo "--- Target Memory Footprint Layout ---"
	@$(SZ) $@

# Rule to extract raw binary data (.bin) from the ELF file for flashing
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@$(CP) -O binary $< $@

# Rule to safely create the build output directory if it does not exist
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean rule to completely wipe the build outputs and start fresh
clean:
	@echo "Cleaning up build directory..."
	@rm -rf $(BUILD_DIR)

# Convenience rule to launch OpenOCD for debugging via the Nucleo ST-Link
openocd:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

.PHONY: all clean openocd