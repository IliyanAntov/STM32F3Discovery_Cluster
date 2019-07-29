/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "user_main.h"
#include "stm32f3xx_hal.h"
#include "CANSPI.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

#include "snow_tiger.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
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
  MX_SPI1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  CANSPI_Initialize();
  HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //----------------------------------------------------------PERFORMANCE TEST
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("FPS TEST, 40 loop 2 screens", 10, 10, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		uint32_t Timer_Counter = 0;
	  		for(uint32_t j = 0; j < 2; j++)
	  		{
	  			HAL_TIM_Base_Start(&htim3);
	  			for(uint16_t i = 0; i < 10; i++)
	  			{
	  				ILI9341_Fill_Screen(WHITE);
	  				ILI9341_Fill_Screen(BLACK);
	  			}

	  			//20.000 per second!
	  			HAL_TIM_Base_Stop(&htim3);
	  			Timer_Counter += __HAL_TIM_GET_COUNTER(&htim3);
	  			__HAL_TIM_SET_COUNTER(&htim3, 0);
	  		}
	  		Timer_Counter /= 2;

	  		char counter_buff[30];
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		sprintf(counter_buff, "Timer counter value: %d", Timer_Counter*2);
	  		ILI9341_Draw_Text(counter_buff, 10, 10, BLACK, 1, WHITE);

	  		double seconds_passed = 2*((float)Timer_Counter / 20000);
	  		printf(counter_buff, "Time: %.3f Sec", seconds_passed);
	  		ILI9341_Draw_Text(counter_buff, 10, 30, BLACK, 2, WHITE);

	  		double timer_float = 20/(((float)Timer_Counter)/20000);	//Frames per sec

	  		printf(counter_buff, "FPS:  %.2f", timer_float);
	  		ILI9341_Draw_Text(counter_buff, 10, 50, BLACK, 2, WHITE);
	  		double MB_PS = timer_float*240*320*2/1000000;
	  		printf(counter_buff, "MB/S: %.2f", MB_PS);
	  		ILI9341_Draw_Text(counter_buff, 10, 70, BLACK, 2, WHITE);
	  		double SPI_utilized_percentage = (MB_PS/(6.25 ))*100;		//50mbits / 8 bits
	  		printf(counter_buff, "SPI Utilized: %.2f", SPI_utilized_percentage);
	  		ILI9341_Draw_Text(counter_buff, 10, 90, BLACK, 2, WHITE);
	  		HAL_Delay(10000);


	  		static uint16_t x = 0;
	  		static uint16_t y = 0;

	  		char Temp_Buffer_text[40];

	  //----------------------------------------------------------COUNTING MULTIPLE SEGMENTS
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Counting multiple segments at once", 10, 10, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);


	  		for(uint16_t i = 0; i <= 10; i++)
	  		{
	  		sprintf(Temp_Buffer_text, "Counting: %d", i);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 30, BLUE, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 50, RED, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 70, GREEN, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 90, BLACK, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 110, BLUE, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 130, RED, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 150, GREEN, 2, WHITE);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 170, WHITE, 2, BLACK);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 190, BLUE, 2, BLACK);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 210, RED, 2, BLACK);
	  		}

	  		HAL_Delay(1000);

	  //----------------------------------------------------------COUNTING SINGLE SEGMENT
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Counting single segment", 10, 10, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		for(uint16_t i = 0; i <= 100; i++)
	  		{
	  		sprintf(Temp_Buffer_text, "Counting: %d", i);
	  		ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 3, WHITE);
	  		}

	  		HAL_Delay(1000);

	  //----------------------------------------------------------ALIGNMENT TEST
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Rectangle alignment check", 10, 10, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		ILI9341_Draw_Hollow_Rectangle_Coord(50, 50, 100, 100, BLACK);
	  		ILI9341_Draw_Filled_Rectangle_Coord(20, 20, 50, 50, BLACK);
	  		ILI9341_Draw_Hollow_Rectangle_Coord(10, 10, 19, 19, BLACK);
	  		HAL_Delay(1000);

	  //----------------------------------------------------------LINES EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Horizontal and Vertical lines", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		for(uint32_t i = 0; i < 30000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			uint16_t radiusr = 0;
	  			uint16_t colourr = 0;
	  			random_num = 28;
	  			xr = random_num;
	  			random_num = 289;
	  			yr = random_num;
	  			random_num = 2128;
	  			radiusr = random_num;
	  			random_num = 123;
	  			colourr = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			radiusr &= 0x001F;
	  			//ili9341_drawpixel(xr, yr, WHITE);
	  			ILI9341_Draw_Horizontal_Line(xr, yr, radiusr, colourr);
	  			ILI9341_Draw_Vertical_Line(xr, yr, radiusr, colourr);
	  		}

	  		HAL_Delay(1000);

	  //----------------------------------------------------------HOLLOW CIRCLES EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Circles", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);


	  		for(uint32_t i = 0; i < 3000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			uint16_t radiusr = 0;
	  			uint16_t colourr = 0;
	  			random_num = 28;
	  			xr = random_num;
	  			random_num = 1328;
	  			yr = random_num;
	  			random_num = 228;
	  			radiusr = random_num;
	  			random_num = 1832;
	  			colourr = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			radiusr &= 0x001F;
	  			//ili9341_drawpixel(xr, yr, WHITE);
	  			ILI9341_Draw_Hollow_Circle(xr, yr, radiusr*2, colourr);
	  		}
	  		HAL_Delay(1000);

	  //----------------------------------------------------------FILLED CIRCLES EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Filled Circles", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		for(uint32_t i = 0; i < 1000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			uint16_t radiusr = 0;
	  			uint16_t colourr = 0;
	  			random_num = 28;
				xr = random_num;
				random_num = 1328;
				yr = random_num;
				random_num = 228;
				radiusr = random_num;
				random_num = 1832;
				colourr = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			radiusr &= 0x001F;
	  			//ili9341_drawpixel(xr, yr, WHITE);
	  			ILI9341_Draw_Filled_Circle(xr, yr, radiusr/2, colourr);
	  		}
	  		HAL_Delay(1000);

	  //----------------------------------------------------------HOLLOW RECTANGLES EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Rectangles", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		for(uint32_t i = 0; i < 20000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			uint16_t radiusr = 0;
	  			uint16_t colourr = 0;
	  			random_num = 28;
	  							xr = random_num;
	  							random_num = 1328;
	  							yr = random_num;
	  							random_num = 228;
	  							radiusr = random_num;
	  							random_num = 1832;
	  							colourr = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			radiusr &= 0x001F;
	  			//ili9341_drawpixel(xr, yr, WHITE);
	  			ILI9341_Draw_Hollow_Rectangle_Coord(xr, yr, xr+radiusr, yr+radiusr, colourr);
	  		}
	  		HAL_Delay(1000);

	  //----------------------------------------------------------FILLED RECTANGLES EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Filled Rectangles", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);

	  		for(uint32_t i = 0; i < 20000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			uint16_t radiusr = 0;
	  			uint16_t colourr = 0;
	  			random_num = 28;
	  							xr = random_num;
	  							random_num = 1328;
	  							yr = random_num;
	  							random_num = 228;
	  							radiusr = random_num;
	  							random_num = 1832;
	  							colourr = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			radiusr &= 0x001F;
	  			//ili9341_drawpixel(xr, yr, WHITE);
	  			ILI9341_Draw_Rectangle(xr, yr, radiusr, radiusr, colourr);
	  		}
	  		HAL_Delay(1000);

	  //----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE

	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Slow draw by selecting", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("and adressing pixels", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);


	  		x = 0;
	  		y = 0;
	  		while (y < 240)
	  		{
	  		while ((x < 320) && (y < 240))
	  		{

	  			if(x % 2)
	  			{
	  				ILI9341_Draw_Pixel(x, y, BLACK);
	  			}

	  			x++;
	  		}

	  			y++;
	  			x = 0;
	  		}

	  		x = 0;
	  		y = 0;


	  		while (y < 240)
	  		{
	  		while ((x < 320) && (y < 240))
	  		{

	  			if(y % 2)
	  			{
	  				ILI9341_Draw_Pixel(x, y, BLACK);
	  			}

	  			x++;
	  		}

	  			y++;
	  			x = 0;
	  		}
	  		HAL_Delay(2000);

	  //----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Random position and colour", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("500000 pixels", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Fill_Screen(WHITE);


	  		for(uint32_t i = 0; i < 500000; i++)
	  		{
	  			uint32_t random_num = 0;
	  			uint16_t xr = 0;
	  			uint16_t yr = 0;
	  			random_num = 28;
	  							xr = random_num;
	  							random_num = 1328;
	  							yr = random_num;
	  							random_num = 228;
	  			uint16_t color = random_num;

	  			xr &= 0x01FF;
	  			yr &= 0x01FF;
	  			ILI9341_Draw_Pixel(xr, yr, color);
	  		}
	  		HAL_Delay(2000);

	  //----------------------------------------------------------565 COLOUR EXAMPLE, Grayscale
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("Colour gradient", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("Grayscale", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);


	  		for(uint16_t i = 0; i <= (320); i++)
	  		{
	  			uint16_t Red = 0;
	  			uint16_t Green = 0;
	  			uint16_t Blue = 0;

	  			Red = i/(10);
	  			Red <<= 11;
	  			Green = i/(5);
	  			Green <<= 5;
	  			Blue = i/(10);



	  			uint16_t RGB_color = Red + Green + Blue;
	  			ILI9341_Draw_Rectangle(i, x, 1, 240, RGB_color);

	  		}
	  		HAL_Delay(2000);

	  //----------------------------------------------------------IMAGE EXAMPLE, Snow Tiger
	  		ILI9341_Fill_Screen(WHITE);
	  		ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	  		ILI9341_Draw_Text("RGB Picture", 10, 10, BLACK, 1, WHITE);
	  		ILI9341_Draw_Text("TIGER", 10, 20, BLACK, 1, WHITE);
	  		HAL_Delay(2000);
	  		ILI9341_Draw_Image((const char*)snow_tiger, SCREEN_VERTICAL_2);
	  		ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
	  		HAL_Delay(10000);



	  //user_main();
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 800;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Stepper_direction_Pin|Stepper_signal_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Display_DC_Pin|Display_CS_Pin|CAN_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : Stepper_direction_Pin Stepper_signal_Pin */
  GPIO_InitStruct.Pin = Stepper_direction_Pin|Stepper_signal_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Display_DC_Pin Display_CS_Pin CAN_CS_Pin */
  GPIO_InitStruct.Pin = Display_DC_Pin|Display_CS_Pin|CAN_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE9 PE10 PE11 
                           PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
