CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

vpath %.c src src/peripherals

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -Iinc -Iinc/core -Iinc/peripherals -DSTM32F746xx 
CFLAGS += -ICMSIS/Include -ICMSIS/Device/ST/STM32F7xx/Include
CFLAGS += -ISTM32746G-Discovery -I ./lib/Utilities
CFLAGS += -IComponents/ampire480272 -IComponents/ampire640480 
CFLAGS += -IComponents/Common
CFLAGS += -IComponents/exc7200 -IComponents/ft5336 -IComponents/mfxstm32l152
CFLAGS += -IComponents/n25q128a -IComponents/n25q512a -IComponents/ov9655
CFLAGS += -IComponents/rk043fn48h -IComponents/s5k5cag -IComponents/stmpe811
CFLAGS += -IComponents/ts3510 -IComponents/wm8994 

SRCS = system_stm32f7xx.c \
	stm32f7xx_hal_adc.c stm32f7xx_hal_adc_ex.c stm32f7xx_hal.c \
	stm32f7xx_hal_can.c stm32f7xx_hal_cec.c stm32f7xx_hal_cortex.c \
	stm32f7xx_hal_crc.c stm32f7xx_hal_crc_ex.c stm32f7xx_hal_cryp.c \
	stm32f7xx_hal_cryp_ex.c stm32f7xx_hal_dac.c stm32f7xx_hal_dac_ex.c \
	stm32f7xx_hal_dcmi.c stm32f7xx_hal_dcmi_ex.c stm32f7xx_hal_dma2d.c \
	stm32f7xx_hal_dma.c stm32f7xx_hal_dma_ex.c stm32f7xx_hal_eth.c \
	stm32f7xx_hal_flash.c stm32f7xx_hal_flash_ex.c stm32f7xx_hal_gpio.c \
	stm32f7xx_hal_hash.c stm32f7xx_hal_hash_ex.c stm32f7xx_hal_hcd.c \
	stm32f7xx_hal_i2c.c stm32f7xx_hal_i2c_ex.c stm32f7xx_hal_i2s.c \
	stm32f7xx_hal_irda.c stm32f7xx_hal_iwdg.c stm32f7xx_hal_lptim.c \
	stm32f7xx_hal_ltdc.c stm32f7xx_hal_msp_template.c stm32f7xx_hal_nand.c \
	stm32f7xx_hal_nor.c stm32f7xx_hal_pcd.c stm32f7xx_hal_pcd_ex.c \
	stm32f7xx_hal_pwr.c stm32f7xx_hal_pwr_ex.c stm32f7xx_hal_qspi.c \
	stm32f7xx_hal_rcc.c stm32f7xx_hal_rcc_ex.c stm32f7xx_hal_rng.c \
	stm32f7xx_hal_rtc.c stm32f7xx_hal_rtc_ex.c stm32f7xx_hal_sai.c \
	stm32f7xx_hal_sai_ex.c stm32f7xx_hal_sd.c stm32f7xx_hal_sdram.c \
	stm32f7xx_hal_smartcard.c stm32f7xx_hal_spi.c stm32f7xx_hal_sram.c \
	stm32f7xx_hal_tim.c stm32f7xx_hal_tim_ex.c stm32f7xx_hal_uart.c \
	stm32f7xx_hal_usart.c stm32f7xx_hal_wwdg.c stm32f7xx_ll_fmc.c \
	stm32f7xx_ll_sdmmc.c stm32f7xx_ll_usb.c \
	STM32746G-Discovery/stm32746g_discovery_audio.c \
	STM32746G-Discovery/stm32746g_discovery.c \
	STM32746G-Discovery/stm32746g_discovery_camera.c \
	STM32746G-Discovery/stm32746g_discovery_eeprom.c \
	STM32746G-Discovery/stm32746g_discovery_lcd.c \
	STM32746G-Discovery/stm32746g_discovery_qspi.c \
	STM32746G-Discovery/stm32746g_discovery_sd.c \
	STM32746G-Discovery/stm32746g_discovery_sdram.c \
	STM32746G-Discovery/stm32746g_discovery_ts.c \
	Components/stmpe811/stmpe811.c \
	Components/mfxstm32l152/mfxstm32l152.c \
	Components/ts3510/ts3510.c \
	Components/wm8994/wm8994.c \
	Components/exc7200/exc7200.c \
	Components/s5k5cag/s5k5cag.c \
	Components/ft5336/ft5336.c \
	Components/ov9655/ov9655.c \
	Utilities/Log/lcd_log.c \
	Utilities/Fonts/font20.c \
	Utilities/Fonts/font8.c \
	Utilities/Fonts/font12.c \
	Utilities/Fonts/font16.c \
	Utilities/Fonts/font24.c \

	#Utilities/CPU/cpu_utils.c \

OBJS = $(SRCS:.c=.o)

.PHONY: libstm32f7.a

all: libstm32f7.a

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f7.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	@rm -f $(OBJS) libstm32f7.a
