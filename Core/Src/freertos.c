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
#include "rtc.h"
#include "bands.h"
#include "usbd_cdc_if.h"
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
uint8_t *taskListStatus[1024];
uint8_t *taskRunStatus[1024];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
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
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l);
extern volatile unsigned long ulHighFrequencyTimerTicks;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartDisplayTask(void *argument);
void StartEncoderTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void) {
	//ulHighFrequencyTimerTicks = 0UL;
}

__weak unsigned long getRunTimeCounterValue(void) {
	return ulHighFrequencyTimerTicks;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook(void) {
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	 to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
	 task. It is essential that code added to this hook function never attempts
	 to block in any way (for example, call xQueueReceive() with a block time
	 specified, or call vTaskDelay()). If the application makes use of the
	 vTaskDelete() API function (as this demo application does) then it is also
	 important that vApplicationIdleHook() is permitted to return to its calling
	 function, because it is the responsibility of the idle task to clean up
	 memory allocated by the kernel to any task that has since been deleted. */
	vTaskList(taskListStatus);
}

/* USER CODE END 2 */

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
  EncoderQueueHandle = osMessageQueueNew (1, sizeof(uint16_t), &EncoderQueue_attributes);

  /* creation of ButtonQueue */
  ButtonQueueHandle = osMessageQueueNew (1, sizeof(uint16_t), &ButtonQueue_attributes);

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
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */
  char buff[10];
	for (;;) {
		//CDC_Transmit_FS(taskListStatus, sizeof(taskListStatus));
		sprintf(buff, "%lu", ulHighFrequencyTimerTicks);
		CDC_Transmit_FS(buff, sizeof(buff));
		osDelay(5000);
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
	osStatus_t taskMessageHandler;
	int32_t counter = 0;
	int16_t button = 0;
	int32_t last_counter = 0;
	int16_t max_index = 5;
	ST7735_FillScreenFast(ST7735_BLACK);
	displayBand(last_counter);
	setBand(last_counter);
	for (;;) {
		//Encoder Rotate
		taskMessageHandler = osMessageQueueGet(EncoderQueueHandle, &counter, 0,
				0);
		if (taskMessageHandler == osOK)
		{
			if (counter == 2) {
				if (last_counter != max_index - 1) {
					last_counter += 1;
					counter = 0;
				} else {
					last_counter = 0;
					counter = 0;
				}
				displayBand(last_counter);
				setBand(last_counter);
			} else if (counter == 1) {
				if (last_counter != 0) {
					last_counter -= 1;
					counter = 0;
				} else {
					last_counter = max_index - 1;
					counter = 0;
				}
				displayBand(last_counter);
				setBand(last_counter);
			} else {

			}
		}
		//Button click
		osMessageQueueGet(ButtonQueueHandle, &button, 0, 0);
		if (button) {

		}
		osDelay(1000);
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
			} else if (currCounter < prevCounter) {
				counter = 2;
				osMessageQueuePut(EncoderQueueHandle, &counter, 0, 0);
			} else {

			}
			prevCounter = currCounter;
		}
		// Button
		if (buttonPressed[buttonNumber]) {
			buttonPressed[buttonNumber] = 0;
			osMessageQueuePut(ButtonQueueHandle, &buttonSend, 0, 0);
		}
		osDelay(500);
	}
  /* USER CODE END StartEncoderTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// Fast hsl2rgb algorithm: https://stackoverflow.com/questions/13105185/fast-algorithm-for-rgb-hsl-conversion
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l) {
	if (l == 0)
		return 0;

	volatile uint8_t r, g, b, lo, c, x, m;
	volatile uint16_t h1, l1, H;
	l1 = l + 1;
	if (l < 128)
		c = ((l1 << 1) * s) >> 8;
	else
		c = (512 - (l1 << 1)) * s >> 8;

	H = h * 6;              // 0 to 1535 (actually 1530)
	lo = H & 255;           // Low byte  = primary/secondary color mix
	h1 = lo + 1;

	if ((H & 256) == 0)
		x = h1 * c >> 8;          // even sextant, like red to yellow
	else
		x = (256 - h1) * c >> 8;  // odd sextant, like yellow to green

	m = l - (c >> 1);
	switch (H >> 8) {       // High byte = sextant of colorwheel
	case 0:
		r = c;
		g = x;
		b = 0;
		break; // R to Y
	case 1:
		r = x;
		g = c;
		b = 0;
		break; // Y to G
	case 2:
		r = 0;
		g = c;
		b = x;
		break; // G to C
	case 3:
		r = 0;
		g = x;
		b = c;
		break; // C to B
	case 4:
		r = x;
		g = 0;
		b = c;
		break; // B to M
	default:
		r = c;
		g = 0;
		b = x;
		break; // M to R
	}

	return (((uint32_t) r + m) << 16) | (((uint32_t) g + m) << 8)
			| ((uint32_t) b + m);
}
/* USER CODE END Application */

