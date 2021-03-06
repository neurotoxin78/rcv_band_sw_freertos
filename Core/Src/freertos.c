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
#include "display.h"
#include "usbd_cdc_if.h"
#include "si5351.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define f_multiplier 4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern volatile uint8_t buttonPressed[5];
uint8_t buttonNumber = 0;
/* Band */
uint32_t max_freq;
uint32_t min_freq;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for encoderTask */
osThreadId_t encoderTaskHandle;
const osThreadAttr_t encoderTask_attributes = {
  .name = "encoderTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonsTask */
osThreadId_t buttonsTaskHandle;
const osThreadAttr_t buttonsTask_attributes = {
  .name = "buttonsTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint32_t hsl_to_rgb(uint8_t h, uint8_t s, uint8_t l);
extern volatile unsigned long ulHighFrequencyTimerTicks;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartEncoderTask(void *argument);
void StartButtonsTask(void *argument);

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

typedef struct {
	const char *name;
	const uint16_t *step; // Min. frequency value for the band (unit 0.01Hz)
} Step;

Step step[8] = {
		{ " 1Hz ", 1 }, // Step
		{ " 10Hz", 10 },
		{ "100Hz", 100 },
		{ "500Hz", 500 },
		{ " 1kHz", 1000 },
		{ " 5kHz", 5000 },
		{ "10kHz", 10000 },
		{ "100k", 10000 }
};
const int lastStep = (sizeof step / sizeof(Step)) - 1;

typedef struct {
	const char *name;
	const uint32_t *minFreq; // Min. frequency value for the band (unit 0.01Hz)
	const uint32_t *maxFreq; // Max. frequency value for the band (unit 0.01Hz)
} Band;

Band band[20] = {
		{"LMW", 100000, 1700000},
		{"SW1", 1700000, 3500000},
		{"SW2", 3500000, 4000000},
		{"SW3", 4000000, 7000000},
		{"40m", 7000000, 7300000},
		{"41m", 7300000, 9000000},
		{"31m", 9000000, 10000000},
		{"30m", 10000000, 11000000},
		{"25m", 11000000, 14000000},
		{"20m", 14000000, 14350000},
		{"19m", 15000000, 17000000},
		{"16m", 17000000, 18000000},
		{"15m", 18000000, 20000000},
		{"13m", 20000000, 21350000},
		{"SW14", 21350000, 22000000},
		{"SW15", 22350000, 24980000},
		{"12m", 24880000, 24990000},
		{"SW17", 24990000, 26000000},
		{"SW18", 26000000, 28000000},
		{"10m", 28000000, 30000000}
};
const int lastBand = (sizeof band / sizeof(Band)) - 1;
extern uint32_t current_freq;
extern uint16_t current_step;
extern uint8_t current_band;
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
	//vTaskList(taskListStatus);
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

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of encoderTask */
  encoderTaskHandle = osThreadNew(StartEncoderTask, NULL, &encoderTask_attributes);

  /* creation of buttonsTask */
  buttonsTaskHandle = osThreadNew(StartButtonsTask, NULL, &buttonsTask_attributes);

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
	current_freq = band[current_band].minFreq;
	si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_8MA);
	si5351_EnableOutputs(1 << 0);
	/* Timer*/
	for (;;) {
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		osDelay(250);
	}
  /* USER CODE END StartDefaultTask */
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
	/*  */
	max_freq = band[current_band].maxFreq;
	min_freq = band[current_band].minFreq;
	current_freq = min_freq;
	/* */
	//ST7735_FillScreenFast(ST7735_BLACK);
	displayBackgrounds();
	displayBand(band[current_band].name);
	displayStep(step[current_step].name);
	displayFrequency(current_freq);
	/* */
	for (;;) {
		int currCounter = __HAL_TIM_GET_COUNTER(&htim3);
		uint16_t step_value = step[current_step].step;
		currCounter = 32767 - ((currCounter - 1) & 0xFFFF) / 2;

		if (currCounter != prevCounter) {
			if (currCounter > prevCounter) {
				if (current_freq != min_freq) {
					current_freq -= step_value;
				} else {
					current_freq = max_freq;
				}
				si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
				displayFrequency(current_freq);
				osDelay(5);
			} else if (currCounter < prevCounter) {
				if (current_freq != max_freq) {
					current_freq += step_value;
				} else {
					current_freq = min_freq;
				}
				si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
				displayFrequency(current_freq);
				osDelay(5);
			} else {

			}
			prevCounter = currCounter;
		}

		osDelay(5);
	}
  /* USER CODE END StartEncoderTask */
}

/* USER CODE BEGIN Header_StartButtonsTask */
/**
 * @brief Function implementing the buttonsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartButtonsTask */
void StartButtonsTask(void *argument)
{
  /* USER CODE BEGIN StartButtonsTask */
	/* Infinite loop */
	for (;;) {
		/* BUTTON */
		//printf("Step %d\n\r", current_step);
		if (buttonPressed[buttonNumber]) {
			buttonPressed[buttonNumber] = 0;
			if (buttonNumber == 0) {
				if (current_step != lastStep) {
					current_step++;
					displayStep(step[current_step].name);
				} else {
					current_step = 0;
					displayStep(step[current_step].name);
				}
			}
			if (buttonNumber == 1) {
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
				if (current_band != lastBand) {
					current_band++;
					displayBand(band[current_band].name);
					max_freq = band[current_band].maxFreq;
					min_freq = band[current_band].minFreq;
					current_freq = min_freq;
					displayFrequency(current_freq);
					si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
				} else {
					current_band = 0;
					displayBand(band[current_band].name);
					max_freq = band[current_band].maxFreq;
					min_freq = band[current_band].minFreq;
					current_freq = min_freq;
					displayFrequency(current_freq);
					si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
				}
			}
			if (buttonNumber == 2) {
							HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
							if (current_band != 0) {
								current_band--;
								displayBand(band[current_band].name);
								max_freq = band[current_band].maxFreq;
								min_freq = band[current_band].minFreq;
								current_freq = min_freq;
								displayFrequency(current_freq);
								si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
							} else {
								current_band = lastBand;
								displayBand(band[current_band].name);
								max_freq = band[current_band].maxFreq;
								min_freq = band[current_band].minFreq;
								current_freq = min_freq;
								displayFrequency(current_freq);
								si5351_SetupCLK0(current_freq * f_multiplier, SI5351_DRIVE_STRENGTH_4MA);
							}
						}
		}
		osDelay(1);
	}
  /* USER CODE END StartButtonsTask */
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

