
# configuration goes here

SRCS = $(wildcard src/*.c)


# the binaries will be hello_world.elf, hello_world.bin, hello_world.hex
PROJECT_NAME = hello_world

# end of configuration section
###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS  = -g -O2 -Wall -Tsrc/STM32F746NGHx_FLASH.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F746xx

###################################################

vpath %.c src
vpath %.a lib

CFLAGS += -Iinc -Ilib -Ilib/inc 
CFLAGS += -Ilib/inc/core -Ilib/inc/peripherals 
CFLAGS += -Ilib/CMSIS/Include -Ilib/CMSIS/Device/ST/STM32F7xx/Include
CFLAGS += -DSTM32F746xx 

# the startup file
SRCS += lib/startup_stm32f746xx.s 

OBJS = $(SRCS:.c=.o)

.PHONY: all
all: lib project

.PHONY: lib
lib:
	$(MAKE) -C lib

.PHONY: project
project: lib $(PROJECT_NAME).elf 

$(PROJECT_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -Llib -lstm32f7 -lc -specs=nosys.specs
	$(OBJCOPY) -O ihex $(PROJECT_NAME).elf $(PROJECT_NAME).hex
	$(OBJCOPY) -O binary $(PROJECT_NAME).elf $(PROJECT_NAME).bin

.PHONY: clean
clean:
	$(MAKE) -C lib clean
	rm -f *.o
	rm -f $(PROJECT_NAME).elf
	rm -f $(PROJECT_NAME).hex
	rm -f $(PROJECT_NAME).bin

.PHONY: flash
flash: $(PROJECT_NAME).elf
	openocd -f utils/stm32f7discovery.cfg -c \
		"program $(PWD)/$(PROJECT_NAME).elf reset exit"

