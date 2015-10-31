
#include "main.h"
#include "RGB565_480x272.h"
#include "STM32746G-Discovery.h"


static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void LCD_Config(void);


int main(void)
{
        RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;


        // Configure the MPU attributes as Write Through 
        MPU_Config();

        // Enable the CPU Cache
        CPU_CACHE_Enable();

        // STM32F7xx HAL library initialization:
        // - Configure the Flash ART accelerator on ITCM interface
        // - Configure the Systick to generate an interrupt each 1 msec
        // - Set NVIC Group Priority to 4
        // - Low Level Initialization
        HAL_Init();

        // Configure the System clock to have a frequency of 216 MHz 
        SystemClock_Config();

        // LCD clock configuration
        // PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
        // PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz
        // PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz
        // LTDC clock frequency = PLLLCDCLK/LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        PeriphClkInitStruct.PLLSAI.PLLSAIN = 192;
        PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
        PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

        BSP_LED_Init(LED1);
        LCD_Config();

        while (1)
        {
        }
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow : 
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 216000000
 *            HCLK(Hz)                       = 216000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 432
 *            PLL_P                          = 2
 *            PLL_Q                          = 9
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 7
 */
void SystemClock_Config(void)
{
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_OscInitTypeDef RCC_OscInitStruct;
        HAL_StatusTypeDef ret = HAL_OK;

        // Enable HSE Oscillator and activate PLL with HSE as source
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 25;
        RCC_OscInitStruct.PLL.PLLN = 432;  
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 9;

        ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
        if(ret != HAL_OK)
        {
                while(1) { ; }
        }

        // Activate the OverDrive to reach the 216 MHz Frequency 
        ret = HAL_PWREx_EnableOverDrive();
        if(ret != HAL_OK)
        {
                while(1) { ; }
        }

        // Select PLL as system clock source and configure the HCLK, PCLK1 and
        // PCLK2 clocks dividers 
        RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | 
                                       RCC_CLOCKTYPE_HCLK | 
                                       RCC_CLOCKTYPE_PCLK1 | 
                                       RCC_CLOCKTYPE_PCLK2);
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2; 

        ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
        if(ret != HAL_OK)
        {
                while(1) { ; }
        }  
}

static void Error_Handler(void)
{
        BSP_LED_On(LED1);
        while(1)
        {
        }
}

/**
 * @brief Configure the MPU attributes as Write Through for SRAM1/2.
 * @note The Base Address is 0x20010000 since this memory interface is the
 * AXI. The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
 */
static void MPU_Config(void)
{
        MPU_Region_InitTypeDef MPU_InitStruct;

        // Disable the MPU
        HAL_MPU_Disable();

        // Configure the MPU attributes as WT for SRAM
        MPU_InitStruct.Enable = MPU_REGION_ENABLE;
        MPU_InitStruct.BaseAddress = 0x20010000;
        MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
        MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
        MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
        MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
        MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
        MPU_InitStruct.Number = MPU_REGION_NUMBER0;
        MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
        MPU_InitStruct.SubRegionDisable = 0x00;
        MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

        HAL_MPU_ConfigRegion(&MPU_InitStruct);

        // Enable the MPU
        HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  CPU L1-Cache enable.
 */
static void CPU_CACHE_Enable(void)
{
        SCB_EnableICache();
        SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
        while (1)
        {
        }
}
#endif

/**
 * @brief LCD Configuration.
 * @note  This function Configure tha LTDC peripheral :
 *        1) Configure the Pixel Clock for the LCD
 *        2) Configure the LTDC Timing and Polarity
 *        3) Configure the LTDC Layer 1 :
 *           - The frame buffer is located at FLASH memory
 *           - The Layer size configuration : 480x272                      
 */
static void LCD_Config(void)
{ 
        static LTDC_HandleTypeDef hltdc_F;
        LTDC_LayerCfgTypeDef      pLayerCfg;

        // LTDC Initialization ---------------------------------------------

        // Polarity configuration
        // Initialize the horizontal synchronization polarity as active low
        hltdc_F.Init.HSPolarity = LTDC_HSPOLARITY_AL;
        // Initialize the vertical synchronization polarity as active low 
        hltdc_F.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
        // Initialize the data enable polarity as active low 
        hltdc_F.Init.DEPolarity = LTDC_DEPOLARITY_AL; 
        // Initialize the pixel clock polarity as input pixel clock
        hltdc_F.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

        // The RK043FN48H LCD 480x272 is selected 
        // Timing Configuration
        hltdc_F.Init.HorizontalSync = (RK043FN48H_HSYNC - 1);
        hltdc_F.Init.VerticalSync = (RK043FN48H_VSYNC - 1);
        hltdc_F.Init.AccumulatedHBP = (RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
        hltdc_F.Init.AccumulatedVBP = (RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
        hltdc_F.Init.AccumulatedActiveH = (RK043FN48H_HEIGHT + 
                                           RK043FN48H_VSYNC + 
                                           RK043FN48H_VBP - 1);
        hltdc_F.Init.AccumulatedActiveW = (RK043FN48H_WIDTH + 
                                           RK043FN48H_HSYNC + 
                                           RK043FN48H_HBP - 1);
        hltdc_F.Init.TotalHeigh = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + 
                                   RK043FN48H_VBP + RK043FN48H_VFP - 1);
        hltdc_F.Init.TotalWidth = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + 
                                   RK043FN48H_HBP + RK043FN48H_HFP - 1);

        // Configure R,G,B component values for LCD background color : all
        // black background
        hltdc_F.Init.Backcolor.Blue = 0;
        hltdc_F.Init.Backcolor.Green = 0;
        hltdc_F.Init.Backcolor.Red = 0;

        hltdc_F.Instance = LTDC;

        // Layer1 Configuration -------------------------------------------

        // Windowing configuration 
        /* In this case all the active display area is used to display a
         * picture then :
         * Horizontal start = 
         *              horizontal synchronization + Horizontal back porch = 43 
         * Vertical start = 
         *              vertical synchronization + vertical back porch     = 12
         * Horizontal stop = Horizontal start + window width -1 = 43 + 480 -1 
         * Vertical stop = Vertical start + window height -1  = 12 + 272 -1  
         */

        pLayerCfg.WindowX0 = 0;
        pLayerCfg.WindowX1 = 480;
        pLayerCfg.WindowY0 = 0;
        pLayerCfg.WindowY1 = 272;

        // Pixel Format configuration
        pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;

        // Start Address configuration : frame buffer is located at FLASH memory
        pLayerCfg.FBStartAdress = (uint32_t)&RGB565_480x272;

        // Alpha constant (255 == totally opaque)
        pLayerCfg.Alpha = 255;

        // Default Color configuration (configure A,R,G,B component values) :
        // no background color
        pLayerCfg.Alpha0 = 0; // fully transparent
        pLayerCfg.Backcolor.Blue = 0;
        pLayerCfg.Backcolor.Green = 0;
        pLayerCfg.Backcolor.Red = 0;

        pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
        pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;

        pLayerCfg.ImageWidth  = 480;
        pLayerCfg.ImageHeight = 272;

        if(HAL_LTDC_Init(&hltdc_F) != HAL_OK)
        {
                Error_Handler(); 
        }

        if(HAL_LTDC_ConfigLayer(&hltdc_F, &pLayerCfg, 1) != HAL_OK)
        {
                Error_Handler(); 
        }  
}



