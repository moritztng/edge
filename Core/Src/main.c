/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define OV7670_I2C_ADDR 0x42

#define PLL_FACTOR 4

/* Registers */
#define REG_GAIN 0x00     /* Gain lower 8 bits (rest in vref) */
#define REG_BLUE 0x01     /* blue gain */
#define REG_RED 0x02      /* red gain */
#define REG_VREF 0x03     /* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1 0x04     /* Control 1 */
#define COM1_CCIR656 0x40 /* CCIR656 enable */
#define REG_BAVE 0x05     /* U/B Average level */
#define REG_GbAVE 0x06    /* Y/Gb Average level */
#define REG_AECHH 0x07    /* AEC MS 5 bits */
#define REG_RAVE 0x08     /* V/R Average level */
#define REG_COM2 0x09     /* Control 2 */
#define COM2_SSLEEP 0x10  /* Soft sleep mode */
#define REG_PID 0x0a      /* Product ID MSB */
#define REG_VER 0x0b      /* Product ID LSB */
#define REG_COM3 0x0c     /* Control 3 */
#define COM3_SWAP 0x40    /* Byte swap */
#define COM3_SCALEEN 0x08 /* Enable scaling */
#define COM3_DCWEN 0x04   /* Enable downsamp/crop/window */
#define REG_COM4 0x0d     /* Control 4 */
#define REG_COM5 0x0e     /* All "reserved" */
#define REG_COM6 0x0f     /* Control 6 */
#define REG_AECH 0x10     /* More bits of AEC value */
#define REG_CLKRC 0x11    /* Clocl control */
#define CLK_EXT 0x40      /* Use external clock directly */
#define CLK_SCALE 0x3f    /* Mask for internal clock scale */
#define REG_COM7 0x12     /* Control 7 */
#define COM7_RESET 0x80   /* Register reset */
#define COM7_FMT_MASK 0x38
#define COM7_FMT_VGA 0x00
#define COM7_FMT_CIF 0x20   /* CIF format */
#define COM7_FMT_QVGA 0x10  /* QVGA format */
#define COM7_FMT_QCIF 0x08  /* QCIF format */
#define COM7_RGB 0x04       /* bits 0 and 2 - RGB format */
#define COM7_YUV 0x00       /* YUV */
#define COM7_BAYER 0x01     /* Bayer format */
#define COM7_PBAYER 0x05    /* "Processed bayer" */
#define REG_COM8 0x13       /* Control 8 */
#define COM8_FASTAEC 0x80   /* Enable fast AGC/AEC */
#define COM8_AECSTEP 0x40   /* Unlimited AEC step size */
#define COM8_BFILT 0x20     /* Band filter enable */
#define COM8_AGC 0x04       /* Auto gain enable */
#define COM8_AWB 0x02       /* White balance enable */
#define COM8_AEC 0x01       /* Auto exposure enable */
#define REG_COM9 0x14       /* Control 9  - gain ceiling */
#define REG_COM10 0x15      /* Control 10 */
#define COM10_HSYNC 0x40    /* HSYNC instead of HREF */
#define COM10_PCLK_HB 0x20  /* Suppress PCLK on horiz blank */
#define COM10_HREF_REV 0x08 /* Reverse HREF */
#define COM10_VS_LEAD 0x04  /* VSYNC on clock leading edge */
#define COM10_VS_NEG 0x02   /* VSYNC negative */
#define COM10_HS_NEG 0x01   /* HSYNC negative */
#define REG_HSTART 0x17     /* Horiz start high bits */
#define REG_HSTOP 0x18      /* Horiz stop high bits */
#define REG_VSTART 0x19     /* Vert start high bits */
#define REG_VSTOP 0x1a      /* Vert stop high bits */
#define REG_PSHFT 0x1b      /* Pixel delay after HREF */
#define REG_MIDH 0x1c       /* Manuf. ID high */
#define REG_MIDL 0x1d       /* Manuf. ID low */
#define REG_MVFP 0x1e       /* Mirror / vflip */
#define MVFP_MIRROR 0x20    /* Mirror image */
#define MVFP_FLIP 0x10      /* Vertical flip */

#define REG_AEW 0x24      /* AGC upper limit */
#define REG_AEB 0x25      /* AGC lower limit */
#define REG_VPT 0x26      /* AGC/AEC fast mode op region */
#define REG_HSYST 0x30    /* HSYNC rising edge delay */
#define REG_HSYEN 0x31    /* HSYNC falling edge delay */
#define REG_HREF 0x32     /* HREF pieces */
#define REG_TSLB 0x3a     /* lots of stuff */
#define TSLB_YLAST 0x04   /* UYVY or VYUY - see com13 */
#define REG_COM11 0x3b    /* Control 11 */
#define COM11_NIGHT 0x80  /* NIght mode enable */
#define COM11_NMFR 0x60   /* Two bit NM frame rate */
#define COM11_HZAUTO 0x10 /* Auto detect 50/60 Hz */
#define COM11_50HZ 0x08   /* Manual 50Hz select */
#define COM11_EXP 0x02
#define REG_COM12 0x3c     /* Control 12 */
#define COM12_HREF 0x80    /* HREF always */
#define REG_COM13 0x3d     /* Control 13 */
#define COM13_GAMMA 0x80   /* Gamma enable */
#define COM13_UVSAT 0x40   /* UV saturation auto adjustment */
#define COM13_UVSWAP 0x01  /* V before U - w/TSLB */
#define REG_COM14 0x3e     /* Control 14 */
#define COM14_DCWEN 0x10   /* DCW/PCLK-scale enable */
#define REG_EDGE 0x3f      /* Edge enhancement factor */
#define REG_COM15 0x40     /* Control 15 */
#define COM15_R10F0 0x00   /* Data range 10 to F0 */
#define COM15_R01FE 0x80   /*            01 to FE */
#define COM15_R00FF 0xc0   /*            00 to FF */
#define COM15_RGB565 0x10  /* RGB565 output */
#define COM15_RGB555 0x30  /* RGB555 output */
#define REG_COM16 0x41     /* Control 16 */
#define COM16_AWBGAIN 0x08 /* AWB gain enable */
#define REG_COM17 0x42     /* Control 17 */
#define COM17_AECWIN 0xc0  /* AEC window - must match COM4 */
#define COM17_CBAR 0x08    /* DSP Color bar */

/*
 * This matrix defines how the colors are generated, must be
 * tweaked to adjust hue and saturation.
 *
 * Order: v-red, v-green, v-blue, u-red, u-green, u-blue
 *
 * They are nine-bit signed quantities, with the sign bit
 * stored in 0x58.  Sign for v-red is bit 0, and up from there.
 */
#define REG_CMATRIX_BASE 0x4f
#define CMATRIX_LEN 6
#define REG_CMATRIX_SIGN 0x58

#define REG_BRIGHT 0x55  /* Brightness */
#define REG_CONTRAS 0x56 /* Contrast control */

#define REG_GFIX 0x69 /* Fix gain control */

#define REG_DBLV 0x6b    /* PLL control an debugging */
#define DBLV_BYPASS 0x0a /* Bypass PLL */
#define DBLV_X4 0x4a     /* clock x4 */
#define DBLV_X6 0x8a     /* clock x6 */
#define DBLV_X8 0xca     /* clock x8 */

#define REG_SCALING_XSC 0x70 /* Test pattern and horizontal scale factor */
#define TEST_PATTTERN_0 0x80
#define REG_SCALING_YSC 0x71 /* Test pattern and vertical scale factor */
#define TEST_PATTTERN_1 0x80

#define REG_REG76 0x76   /* OV's name */
#define R76_BLKPCOR 0x80 /* Black pixel correction enable */
#define R76_WHTPCOR 0x40 /* White pixel correction enable */

#define REG_RGB444 0x8c  /* RGB 444 control */
#define R444_ENABLE 0x02 /* Turn on RGB444, overrides 5x5 */
#define R444_RGBX 0x01   /* Empty nibble at end */

#define REG_HAECC1 0x9f /* Hist AEC/AGC control 1 */
#define REG_HAECC2 0xa0 /* Hist AEC/AGC control 2 */

#define REG_BD50MAX 0xa5 /* 50hz banding step limit */
#define REG_HAECC3 0xa6  /* Hist AEC/AGC control 3 */
#define REG_HAECC4 0xa7  /* Hist AEC/AGC control 4 */
#define REG_HAECC5 0xa8  /* Hist AEC/AGC control 5 */
#define REG_HAECC6 0xa9  /* Hist AEC/AGC control 6 */
#define REG_HAECC7 0xaa  /* Hist AEC/AGC control 7 */
#define REG_BD60MAX 0xab /* 60hz banding step limit */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

LPTIM_HandleTypeDef hlptim1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim1_ch2;

/* USER CODE BEGIN PV */
volatile uint8_t line_received = 0;
volatile uint8_t frame_start = 0;
uint8_t gpio_b_pins[] = {3, 4, 6, 7, 9, 10, 13, 14};

static uint8_t ov7670_registers[][2] = {
    {REG_COM7, COM7_RESET},
    /*
     * Clock scale: 3 = 15fps
     *              2 = 20fps
     *              1 = 30fps
     */
    {REG_CLKRC, 0x1},          /* OV: clock scale (30 fps) */
    {REG_TSLB, 0x04},          /* OV */
    {REG_COM7, COM7_FMT_QVGA}, /* QVGA */
    /*
     * Set the hardware window.  These values from OV don't entirely
     * make sense - hstop is less than hstart.  But they work...
     */
    {REG_HSTART, 0x15}, //(168 >> 3) & 0xff
    {REG_HSTOP, 0x3},   //(24 >> 3) & 0xff
    {REG_HREF, 0x80},   //(0xb6 & 0xc0) | ((24 & 0x7) << 3) | (168 & 0x7)
    {REG_VSTART, 0x3},  //(12 >> 2) & 0xff
    {REG_VSTOP, 0x7b},  //(492 >> 2) & 0xff
    {REG_VREF, 0x0},    //(0x0a & 0xf0) | ((492 & 0x3) << 2) | (12 & 0x3)

    {REG_COM3, 0},
    {REG_COM14, 0},
    /* Mystery scaling numbers */
    {REG_SCALING_XSC, 0x3a},
    {REG_SCALING_YSC, 0x35},
    {0x72, 0x11},
    {0x73, 0xf0},
    {0xa2, 0x02},
    {REG_COM10, COM10_PCLK_HB},

    /* Gamma curve values */
    {0x7a, 0x20},
    {0x7b, 0x10},
    {0x7c, 0x1e},
    {0x7d, 0x35},
    {0x7e, 0x5a},
    {0x7f, 0x69},
    {0x80, 0x76},
    {0x81, 0x80},
    {0x82, 0x88},
    {0x83, 0x8f},
    {0x84, 0x96},
    {0x85, 0xa3},
    {0x86, 0xaf},
    {0x87, 0xc4},
    {0x88, 0xd7},
    {0x89, 0xe8},

    /* AGC and AEC parameters.  Note we start by disabling those features,
       then turn them only after tweaking the values. */
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT},
    {REG_GAIN, 0},
    {REG_AECH, 0},
    {REG_COM4, 0x40}, /* magic reserved bit */
    {REG_COM9, 0x18}, /* 4x gain + magic rsvd bit */
    {REG_BD50MAX, 0x05},
    {REG_BD60MAX, 0x07},
    {REG_AEW, 0x95},
    {REG_AEB, 0x33},
    {REG_VPT, 0xe3},
    {REG_HAECC1, 0x78},
    {REG_HAECC2, 0x68},
    {0xa1, 0x03}, /* magic */
    {REG_HAECC3, 0xd8},
    {REG_HAECC4, 0xd8},
    {REG_HAECC5, 0xf0},
    {REG_HAECC6, 0x90},
    {REG_HAECC7, 0x94},
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC},

    /* Almost all of these are magic "reserved" values.  */
    {REG_COM5, 0x61},
    {REG_COM6, 0x4b},
    {0x16, 0x02},
    {REG_MVFP, 0x07},
    {0x21, 0x02},
    {0x22, 0x91},
    {0x29, 0x07},
    {0x33, 0x0b},
    {0x35, 0x0b},
    {0x37, 0x1d},
    {0x38, 0x71},
    {0x39, 0x2a},
    {REG_COM12, 0x78},
    {0x4d, 0x40},
    {0x4e, 0x20},
    {REG_GFIX, 0},
    {0x6b, 0x4a},
    {0x74, 0x10},
    {0x8d, 0x4f},
    {0x8e, 0},
    {0x8f, 0},
    {0x90, 0},
    {0x91, 0},
    {0x96, 0},
    {0x9a, 0},
    {0xb0, 0x84},
    {0xb1, 0x0c},
    {0xb2, 0x0e},
    {0xb3, 0x82},
    {0xb8, 0x0a},

    /* More reserved magic, some of which tweaks white balance */
    {0x43, 0x0a},
    {0x44, 0xf0},
    {0x45, 0x34},
    {0x46, 0x58},
    {0x47, 0x28},
    {0x48, 0x3a},
    {0x59, 0x88},
    {0x5a, 0x88},
    {0x5b, 0x44},
    {0x5c, 0x67},
    {0x5d, 0x49},
    {0x5e, 0x0e},
    {0x6c, 0x0a},
    {0x6d, 0x55},
    {0x6e, 0x11},
    {0x6f, 0x9f}, /* "9e for advance AWB" */
    {0x6a, 0x40},
    {REG_BLUE, 0x40},
    {REG_RED, 0x60},
    {REG_COM8, COM8_FASTAEC | COM8_AECSTEP | COM8_BFILT | COM8_AGC | COM8_AEC | COM8_AWB},

    /* Matrix coefficients */
    {0x4f, 0x80},
    {0x50, 0x80},
    {0x51, 0},
    {0x52, 0x22},
    {0x53, 0x5e},
    {0x54, 0x80},
    {0x58, 0x9e},

    {REG_COM16, COM16_AWBGAIN},
    {REG_EDGE, 0},
    {0x75, 0x05},
    {0x76, 0xe1},
    {0x4c, 0},
    {0x77, 0x01},
    {REG_COM13, 0xc3},
    {0x4b, 0x09},
    {0xc9, 0x60},
    {REG_COM16, 0x38},
    {0x56, 0x40},

    {0x34, 0x11},
    {REG_COM11, COM11_EXP | COM11_HZAUTO},
    {0xa4, 0x88},
    {0x96, 0},
    {0x97, 0x30},
    {0x98, 0x20},
    {0x99, 0x30},
    {0x9a, 0x84},
    {0x9b, 0x29},
    {0x9c, 0x03},
    {0x9d, 0x4c},
    {0x9e, 0x3f},
    {0x78, 0x04},

    /* Extra-weird stuff.  Some sort of multiplexor register */
    {0x79, 0x01},
    {0xc8, 0xf0},
    {0x79, 0x0f},
    {0xc8, 0x00},
    {0x79, 0x10},
    {0xc8, 0x7e},
    {0x79, 0x0a},
    {0xc8, 0x80},
    {0x79, 0x0b},
    {0xc8, 0x01},
    {0x79, 0x0c},
    {0xc8, 0x0f},
    {0x79, 0x0d},
    {0xc8, 0x20},
    {0x79, 0x09},
    {0xc8, 0x80},
    {0x79, 0x02},
    {0xc8, 0xc0},
    {0x79, 0x03},
    {0xc8, 0x40},
    {0x79, 0x05},
    {0xc8, 0x30},
    {0x79, 0x26},

    {0xff, 0xff}, /* END MARKER */
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM1_Init(void);
static void MX_LPTIM1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_LPTIM1_Init();
  MX_I2C2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  W25qxx_Init();
  W25qxx_EraseBlock(0);
  Capture_Frame(0);
  uint8_t frame[4800];
  W25qxx_ReadBlock(frame, 0, 0, 4800);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_11;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x20303E5D;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief LPTIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV1;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */
  /* USER CODE END LPTIM1_Init 2 */
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB3 PB4 PB7 PB9
                           PB6 PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PC0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  switch (htim->Channel)
  {
  case HAL_TIM_ACTIVE_CHANNEL_3:
    frame_start = 1;
    break;
  case HAL_TIM_ACTIVE_CHANNEL_1:
    line_received += 1;
  }
}

uint8_t Spi_Send(uint8_t data)
{
  uint8_t reception;
  HAL_SPI_TransmitReceive(&hspi2, &data, &reception, 1, 100);
  return reception;
}

void Spi_Enable_Write()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  Spi_Send(0x06);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void Spi_Send_Page(uint32_t page_address, uint8_t *p_data, uint16_t size)
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  Spi_Send(0x02);
  Spi_Send((page_address & 0xFF0000) >> 16);
  Spi_Send((page_address & 0xFF00) >> 8);
  Spi_Send(page_address & 0xFF);
  HAL_SPI_Transmit(&hspi2, p_data, size, 100);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void Spi_Wait_Write_End()
{
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
  Spi_Send(0x05);
  uint8_t ret;
  do
  {
    ret = Spi_Send(0xA5);
    HAL_Delay(1);
  } while ((ret & 0x01) == 0x01);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void Capture_Frame(uint32_t frame_address)
{
  uint8_t frame_buffer[4800];
  uint16_t n_lines;
  uint16_t line_buffer[1024];
  uint32_t frame_index = 0;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14);
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_LPTIM_PWM_Start(&hlptim1, 3, 2);
  HAL_Delay(1000);
  int index_registers = 0;
  while (ov7670_registers[index_registers][0] != 0xff || ov7670_registers[index_registers][1] != 0xff)
  {
    HAL_I2C_Master_Transmit(&hi2c2, OV7670_I2C_ADDR, ov7670_registers[index_registers], 2, HAL_MAX_DELAY);
    HAL_Delay(10);
    index_registers++;
  }
  HAL_DMA_Start(&hdma_tim1_ch2, (uint32_t)&GPIOB->IDR, (uint32_t)line_buffer, 1024);
  HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_2);
  __HAL_TIM_ENABLE_DMA(&htim1, TIM_DMA_CC2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);

  while (!frame_start)
    ;
  frame_start = 0;
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
  while (frame_index < 4800)
  {
    while (!line_received)
      ;
    line_received = 0;
    if (n_lines % 4 == 0)
    {
      int start_line_buffer = ((1024 - __HAL_DMA_GET_COUNTER(&hdma_tim1_ch2)) + 1024 - 640) % 1024;
      for (int i = 0; i < 640; i += 8)
      {
        uint8_t value = 0;
        for (int j = 0; j < 8; j++)
        {
          value |= ((line_buffer[(start_line_buffer + i) % 1024] >> gpio_b_pins[j]) & 1) << j;
        }
        frame_buffer[frame_index++] = value;
      }
    }
    n_lines++;
  }
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13 | GPIO_PIN_14);
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_LPTIM_PWM_Stop(&hlptim1);
  HAL_TIM_IC_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_3);
  uint8_t *p_frame_buffer = frame_buffer;
  for (int i = 0; i < 4800; i += 256)
  {
    Spi_Enable_Write();
    HAL_Delay(1);
    uint32_t n_pixels_left = 4800 - i;
    Spi_Send_Page(i, p_frame_buffer + i, n_pixels_left < 256 ? n_pixels_left : 256);
    HAL_Delay(1);
    Spi_Wait_Write_End();
    HAL_Delay(1);
  }
  W25qxx_ReadBlock(frame_buffer, 0, 0, 4800);
  return;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
