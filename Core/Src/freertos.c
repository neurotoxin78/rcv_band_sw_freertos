/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "st7735.h"
#include "fonts.h"
#include "tim.h"
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
/* USER CODE BEGIN Variables */
extern volatile uint8_t buttonPressed[5];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for displayTask */
osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = {
  .name = "displayTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for encoderTask */
osThreadId_t encoderTaskHandle;
const osThreadAttr_t encoderTask_attributes = {
  .name = "encoderTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for EncoderQueue */
osMessageQueueId_t EncoderQueueHandle;
const osMessageQueueAttr_t EncoderQueue_attributes = {
  .name = "EncoderQueue"
};
/* Definitions for ButtonQueue */
osMessageQueueId_t ButtonQueueHandle;
const osMessageQueueAttr_t ButtonQueue_attributes = {
  .name = "ButtonQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartDisplayTask(void *argument);
void StartEncoderTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of EncoderQueue */
  EncoderQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &EncoderQueue_attributes);

  /* creation of ButtonQueue */
  ButtonQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &ButtonQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of displayTask */
  displayTaskHandle = osThreadNew(StartDisplayTask, NULL, &displayTask_attributes);

  /* creation of encoderTask */
  encoderTaskHandle = osThreadNew(StartEncoderTask, NULL, &encoderTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
	for (;;) {
		osDelay(1);
	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
 * @brief Function implementing the displayTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void *argument)
{
  /* USER CODE BEGIN StartDisplayTask */
	/* Infinite loop */
	uint16_t EncoderReceiveResult = 0;
	uint16_t ButtonReceiveResult = 0;
	int32_t counter = 0;
	int16_t button = 0;
	int32_t last_counter = 0;
	int16_t max_index = 5;
	char buff[16];
	char bbuff[8];
	for (;;) {
		//Encoder Rotate
		EncoderReceiveResult = osMessageQueueGet(EncoderQueueHandle, &counter, 0, 0);
		snprintf(buff, sizeof(buff), "%02d", counter);
		ST7735_WriteString(0, 60, buff, Font_7x10, ST7735_GREEN, ST7735_BLACK);
		if(counter == 1) {
			if(last_counter != max_index -1)
			{
				last_counter += 1;
				counter = 0;
			} else
			{
				last_counter = 0;
				counter = 0;
			}
			displayBand(last_counter);
		}else if (counter == 2){
			if(last_counter != 0)
			{
				last_counter -= 1;
				counter = 0;
			} else
			{
				last_counter = max_index -1;
				counter = 0;
			}
			displayBand(last_counter);
		}else
		{

		}

		//Button click
		ButtonReceiveResult = osMessageQueueGet(ButtonQueueHandle, &button, 0, 0);
		if (button)
		{
			snprintf(bbuff, sizeof(bbuff), "%01d", button);
			ST7735_WriteString(80, 60, bbuff, Font_7x10, ST7735_CYAN, ST7735_BLACK);
			button = 0;
			osDelay(500);
			ST7735_WriteString(80, 60, "     ", Font_7x10, ST7735_CYAN, ST7735_BLACK);
		}

		osDelay(100);
	}
  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartEncoderTask */
/**
 * @brief Function implementing the encoderTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartEncoderTask */
void StartEncoderTask(void *argument)
{
  /* USER CODE BEGIN StartEncoderTask */
	/* Infinite loop */
	char buff[16];
	int32_t prevCounter = 0;
	int32_t counter = 0;
	uint8_t buttonNumber = 0;
	uint8_t buttonSend = 1;
	for (;;) {
		int currCounter = __HAL_TIM_GET_COUNTER(&htim3);
		currCounter = 32767 - ((currCounter - 1) & 0xFFFF) / 2;
		if (currCounter != prevCounter) {
			if (currCounter > prevCounter) {
				counter = 1;
				osMessageQueuePut(EncoderQueueHandle, &counter, 0, 0);
			} else if (currCounter < prevCounter){
				counter = 2;
				osMessageQueuePut(EncoderQueueHandle, &counter, 0, 0);
			}
			else
			{

			}
			snprintf(buff, sizeof(buff), "%06d", currCounter);
			ST7735_WriteString(90, 46, buff, Font_7x10, ST7735_GREEN,
					ST7735_BLACK);
			prevCounter = currCounter;
		}
		// Button
		if(buttonPressed[buttonNumber]) {
			buttonPressed[buttonNumber] = 0;
			//char buff[16];
			//snprintf(buff, sizeof(buff), "BUTTON %d", buttonNumber);
			//ST7735_WriteString(80, 60, buff, Font_7x10, ST7735_RED, ST7735_BLACK);
			osMessageQueuePut(ButtonQueueHandle, &buttonSend, 0, 0);
		}
		osDelay(100);
	}
  /* USER CODE END StartEncoderTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

