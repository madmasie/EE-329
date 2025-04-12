/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE 
 *******************************************************************************
 * @file           : keypad.h
 * @brief          : 
 * project         : EE 329 S'23 Assignment 2
 * authors         : Maddie Masiello - mmasiell@calpoly.edu
 * version         : 0.1
 * date            : 
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC) 
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * 
 *******************************************************************************
 * REVISION HISTORY
 * 0.1 
 *******************************************************************************
 * TODO
 * 
 *******************************************************************************
*/
   
#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"

// ------------------------ KEYPAD Defines ------------------------
#define COL_PORT GPIOF
#define COL_PINS (GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14)

#define ROW_PORT GPIOF
#define ROW_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)

// Settle time for GPIO read delay loop
#define SETTLE 1900  // ~20ms debounce (5.5us * 1900)

// Bit 0 for shifting logic
#define BIT0_COL GPIO_PIN_12
#define BIT0_ROW GPIO_PIN_0

#define NUM_COLS 3
#define NUM_ROWS 4

#define NO_KEYPRESS 0x0
#define KEY_ZERO 11
#define CODE_ZERO 0xF

// ------------------------ Function Prototypes ------------------------
void Keypad_Config(void);
int Keypad_IsAnyKeyPressed(void);
int Keypad_WhichKeyIsPressed(void);

#endif /* INC_KEYPAD_H_ */
