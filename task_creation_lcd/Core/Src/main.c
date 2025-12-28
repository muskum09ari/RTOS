/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  ******************************************************************************
  */

//Remember third party remove exclude,include third party incude,protable->>gcc->>arm,exclude sysmem in properties.
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void lcd_init();
void cmd(unsigned char C);
void data(unsigned char D);
void display(char *ptr);
static void myTask2(void* Parameter);
static void myTask1(void* Parameter);

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
	   TaskHandle_t myTask1Handle;
	   TaskHandle_t myTask2Handle;

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
  /* USER CODE BEGIN 2 */

  HAL_Delay(100);
  lcd_init();

    xTaskCreate(myTask1,"T1",256,NULL,1,&myTask1Handle);// function name,dummyname,size,para to pass,priority//handler has info about task so handler name is myTask1Handle
    xTaskCreate(myTask2,"T2",256,NULL,1,&myTask2Handle);
    vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {cmd(0x80);

	 display("CAN");
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RS_Pin_Pin|EN_Pin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, D0_Pin_Pin|D1_Pin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D2_Pin_Pin|D3_Pin_Pin|D4_Pin_Pin|D5_Pin_Pin
                          |D6_Pin_Pin|D7_Pin_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RS_Pin_Pin EN_Pin_Pin */
  GPIO_InitStruct.Pin = RS_Pin_Pin|EN_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D0_Pin_Pin D1_Pin_Pin */
  GPIO_InitStruct.Pin = D0_Pin_Pin|D1_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : D2_Pin_Pin D3_Pin_Pin D4_Pin_Pin D5_Pin_Pin
                           D6_Pin_Pin D7_Pin_Pin */
  GPIO_InitStruct.Pin = D2_Pin_Pin|D3_Pin_Pin|D4_Pin_Pin|D5_Pin_Pin
                          |D6_Pin_Pin|D7_Pin_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void lcd_init()
	{
	    HAL_Delay(10);
	    cmd(0x30);
	    cmd(0x38);
	    cmd(0x06);
	    cmd(0x0C);
	    HAL_Delay(10);
	}

	void cmd(unsigned char C)
	{
	    HAL_GPIO_WritePin(GPIOA, RS_Pin_Pin,0);
	    HAL_GPIO_WritePin(GPIOC, D0_Pin_Pin,(C&0x01));
	    HAL_GPIO_WritePin(GPIOC, D1_Pin_Pin,(C&0x02));
	    HAL_GPIO_WritePin(GPIOB, D2_Pin_Pin,(C&0x04));
	    HAL_GPIO_WritePin(GPIOB, D3_Pin_Pin,(C&0x08));
	    HAL_GPIO_WritePin(GPIOB, D4_Pin_Pin,(C&0x10));
	    HAL_GPIO_WritePin(GPIOB, D5_Pin_Pin,(C&0x20));
	    HAL_GPIO_WritePin(GPIOB, D6_Pin_Pin,(C&0x40));
	    HAL_GPIO_WritePin(GPIOB, D7_Pin_Pin,(C&0x80));
	    HAL_GPIO_WritePin(GPIOA, EN_Pin_Pin,1);
	    HAL_Delay(1);
	    HAL_GPIO_WritePin(GPIOA, EN_Pin_Pin, 0);
	    HAL_Delay(1);
	}

	void data(unsigned char D)
	{
	    HAL_GPIO_WritePin(GPIOA,RS_Pin_Pin, 1);
	    HAL_GPIO_WritePin(GPIOC,D0_Pin_Pin,(D&0x01));
	    HAL_GPIO_WritePin(GPIOC, D1_Pin_Pin,(D&0x02));
	    HAL_GPIO_WritePin(GPIOB, D2_Pin_Pin,(D&0x04));
	    HAL_GPIO_WritePin(GPIOB, D3_Pin_Pin,(D&0x08));
	    HAL_GPIO_WritePin(GPIOB, D4_Pin_Pin,(D&0x10));
	    HAL_GPIO_WritePin(GPIOB, D5_Pin_Pin,(D&0x20));
	    HAL_GPIO_WritePin(GPIOB, D6_Pin_Pin,(D&0x40));
	    HAL_GPIO_WritePin(GPIOB, D7_Pin_Pin,(D&0x80));
	    HAL_GPIO_WritePin(GPIOA, EN_Pin_Pin,1);
	    HAL_Delay(1);
	    HAL_GPIO_WritePin(GPIOA, EN_Pin_Pin, 0);
	    HAL_Delay(1);
	}

	void display(char *ptr)
	{
	    while (*ptr != '\0')
	    {
	        data(*ptr++);
	        HAL_Delay(2);
	    }
	}

	static void myTask1(void* Parameter)
	{
	    while (1)
	    {
	        cmd(0x80);  // Line 1
	        display("Task 1 Running");
	        vTaskDelay(1000);
	    }
	}

	static void myTask2(void* Parameter)
	{
	    while (1)
	    {
	        cmd(0xC0);  // Line 2
	        display("Task 2 Running");
	        vTaskDelay(3000);
	    }
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

#ifdef  USE_FULL_ASSERT
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
