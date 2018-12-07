/**
  **********************************************************************************
  *  @file  main.c
  *  @brief Project "Drawing Machine" of the discipline Microprocessed Systems, UFC.
  *         2018 Johnny Alves and Yuri da Cunha.
  **********************************************************************************
  */
#include "main.h"
#include "stm32f0xx_hal.h"

uint16_t servoPin = GPIO_PIN_5;
uint16_t stepPin = GPIO_PIN_9, directionPin = GPIO_PIN_10;
uint8_t instructions[17][10] = {"P1",
				"X0 171",
				"P0",
				"X1 171",
				"Y0 260",
				"X0 171",
				"Y1 31",
				"X1 137",
				"Y1 71",
				"X0 137",
				"Y1 31",
				"X1 137",
				"Y1 97",
				"X0 137",
				"Y1 30",
				"P1",
				"X1 171"
				};

void SystemClock_Config();
static void MX_GPIO_Init();
void interpretInstructions(uint8_t *lineCommand);
void movePen(uint8_t direction);
uint16_t stoi(uint8_t *string);
void step(uint8_t axis, uint8_t direction, uint16_t steps);

/**
  *  @brief  The application entry point.
  *  @retval None.
  */
int main(){
	/* MCU Configuration -------------------------------------------------------- */
	/* Reset of all peripherals, initializes the flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock. */
	SystemClock_Config();

	/* Initialize all configured peripherals. */
	MX_GPIO_Init();

	while (1){
		for (uint16_t i = 0; i < 17; i++){
			interpretInstructions(instructions[i]);
		}

		break;
	}
}

/**
  *  @brief  System clock configuration.
  *  @retval None.
  */
void SystemClock_Config(){
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/* Initializes the CPU, AHB and APB busses clocks. */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}

	/** Initializes the CPU, AHB and APB busses clocks. */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|
			                      RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}

	/** Configure the Systick interrupt time. */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/** Configure the Systick. */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration. */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(){
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable. */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure GPIO pin Output Level. */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

	/* Configure GPIO pins: PA0, PA5, PA9 and PA10. */
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  *  @brief  This function is executed in case of error occurrence.
  *  @param  file: The file name as string.
  *  @param  line: The line in file as a number.
  *  @retval None.
  */
void _Error_Handler(char *file, int line){
	/* User can add his own implementation to report the HAL error return state. */

	while(1){

	}
}

#ifdef  USE_FULL_ASSERT
/**
  *  @brief  Reports the name of the source file and the source line number
  *          where the assert_param error has occurred.
  *  @param  file: Pointer to the source file name.
  *  @param  line: assert_param error line source number.
  *  @retval None.
  */
void assert_failed(uint8_t* file, uint32_t line){
	/* User can add his own implementation to report the file name and line number,
	   tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line). */
}
#endif /* USE_FULL_ASSERT */

/**
  *  @brief  Sends the motion coordinates for the motors, besides regulating the po-
  *          sition of the pen.
  *  @param  lineCommand: Line containing the instructions to be interpreted.
  *  @retval None.
  */
void interpretInstructions(uint8_t *lineCommand){
    if (*lineCommand == 'P'){
        lineCommand++;
        movePen(*lineCommand - '0');
    }

    else {
    	uint8_t axis, direction;
    	uint16_t steps;

        if (*lineCommand == 'X')
            axis = 1;

        else if (*lineCommand == 'Y')
            axis = 0;

        lineCommand++;
        direction = *lineCommand - '0';

        lineCommand += 2;
        steps = stoi(lineCommand);

        step(axis, direction, steps);
    }
}

/**
  *  @brief  Update the pen position.
  *  @param  direction: If true, the pen will be raised; if false, lowered.
  *  @retval None.
  */
void movePen(uint8_t direction){
	if (!direction){
		for (uint16_t i = 0; i < 100; i++){
			HAL_GPIO_WritePin(GPIOA, servoPin, GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(GPIOA, servoPin, GPIO_PIN_RESET);
			HAL_Delay(19);
		}
	}

	else {
		for (uint16_t i = 0; i < 100; i++){
			HAL_GPIO_WritePin(GPIOA, servoPin, GPIO_PIN_SET);
			HAL_Delay(2);
			HAL_GPIO_WritePin(GPIOA, servoPin, GPIO_PIN_RESET);
			HAL_Delay(18);
		}
	}
}

/**
  *  @brief  Converts a string to an integer.
  *  @param  string: The string containing the number.
  *  @retval The integer number.
  */
uint16_t stoi(uint8_t *string){
	uint16_t result = 0, number;

    while (*string){
        number = *string - '0';
        result = result * 10 + number;
        string++;
    }

    return result;
}

/**
  *  @brief  Move the stepper motor.
  *  @param  axis: The engine to be active.
  *  @param  direction: The direction of motor rotation.
  *  @param  steps: The number of steps.
  *  @retval None.
  */
void step(uint8_t axis, uint8_t direction, uint16_t steps){
	HAL_GPIO_WritePin(GPIOA, directionPin, direction);

	for (uint16_t i = 0; i < steps; i++){
		HAL_GPIO_WritePin(GPIOA, stepPin, SET);
		HAL_Delay(0);
		HAL_GPIO_WritePin(GPIOA, stepPin, RESET);
		HAL_Delay(0);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
