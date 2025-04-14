/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE
 *******************************************************************************
 * @file           : main.c
 * @brief          : keypad configuration and debounced detection of keypresses
 * project         : EE 329 S'23 Assignment 2
 * authors         : Maddie Masiello - mmasiell@calpoly.edu
 * version         : 0.1
 * date            :
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * KEYPAD PLAN :
 * set columns as outputs, rows as inputs w pulldowns
 * loop:
 * drive all columns HI read all rows
 * if any row N is HI
 *    set all columns LO
 *    drive each column M HI alone
 *    read row N until HI 🡪 pressed key loc’n = N, M
 * key value = 3N+M+1 for 1..9,special case for *,0,#
 *******************************************************************************
 * KEYPAD WIRING 4 ROWS 3 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
 *      peripheral – Nucleo I/O
 * keypad 1  COL 2 - PF13= CN10- 2 - OUT
 * keypad 2  ROW 1 - PF0 = CN9 -21 - IN, PD ON
 * keypad 3  COL 1 - PF12= CN7 -20 - OUT
 * keypad 4  ROW 4 - PF3 = CN8 -14 - IN, PD ON
 * keypad 5  COL 3 - PF14= CN10- 8 - OUT
 * keypad 6  ROW 3 - PF2 = CN9 -17 - IN, PD ON
 * keypad 7  ROW 2 - PF1 = CN9 -19 - IN, PD ON
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 230318 bfd  created, wires in breadboard, no keypad
 * 0.2 230410 bfd  added keypad, debounce code, tested operational
 * 0.3 230413 bfd  refactored to share in lab manual, tested op’l
 *******************************************************************************
 * TODO
 * convert to separable module for portability
 *******************************************************************************
 * 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */
/* USER CODE END Header */

#include "main.h"
#include "keypad.h"

void SystemClock_Config(void);
void Led_Config(void);


/* ---------------------------- Keypad_WhichKeyIsPressed()---------------------
 * This function detects and encodes a pressed key at {row,col}
  * It assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE.
  * It verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
  * determines which key is pressed, and returns the encoded key ID.
  * -------------------------------------------------------------------------- */
int Keypad_DebouncedKey(void)
{
  volatile int key1 = Keypad_WhichKeyIsPressed();
  HAL_Delay(20); // 20 ms debounce
  volatile int key2 = Keypad_WhichKeyIsPressed();
  if (key1 == key2)
    return key1;
  else
    return NO_KEYPRESS;
}
/* ---------------------------- main()-----------------------------------------
 * main() initializes the system, configures the GPIO for the keypad,
 * and enters an infinite loop to check for key presses. 
 * It uses the Keypad_WhichKeyIsPressed() function to determine which key is 
 * pressed. The pressed key is displayed on the LED display, confugured 
 * to show a binary value.
 * -------------------------------------------------------------------------- */

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  Keypad_Config(); // Setup keypad GPIO
  COL_PORT->BSRR = COL_PINS;

  Led_Config(); // Configure LED GPIO


  /* ---------------------------- infinite while loop-----------------------
  * Infinite loop to check for key presses and display on LEDs
  * The loop continuously checks if any key is pressed using the 
  * Keypad_IsAnyKeyPressed() function. If a key is pressed,
  * it determines which key is pressed using the
  * Keypad_WhichKeyIsPressed() function. The pressed key is then displayed
  * on the LED display by setting the corresponding bits in GPIOC->ODR.
  * If no key is pressed, the LED display is turned off.
  * -------------------------------------------------------------------------- */
  volatile int last_key = NO_KEYPRESS;
  while (1) { 
    if (Keypad_IsAnyKeyPressed())
    { 
      if (Keypad_WhichKeyIsPressed() > 0 && Keypad_WhichKeyIsPressed() < 16){ 
        GPIOC->ODR = Keypad_WhichKeyIsPressed(); 
      } 
      else 
        GPIOC->ODR = (0x0); 
      }
}

// --- LED Display Configuration ---
void Led_Config(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

  GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |
                    GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
  GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 |
                   GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);

  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 |
                     GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
  GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |
                    GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
  GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 |
                     GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3);

  GPIOC->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure. */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif