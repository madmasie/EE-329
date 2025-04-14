/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE
 *******************************************************************************
 * @file           : keypad.h
 * @brief          : Keypad GPIO pin mapping and constants for a 4x3 matrix keypad
 * project         : EE 329 S'23 Assignment 2
 * authors         : Maddie Masiello - mmasiell@calpoly.edu
 * version         : 0.1
 * date            :
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics. All rights reserved.
 *******************************************************************************
 * KEY MAPPING:
 * Column outputs: GPIOE pins PE2 (COL1), PE4 (COL2), PE5 (COL3)
 * Row inputs:     GPIOD pins PD6 (ROW1), PD5 (ROW2), PD4 (ROW3), PD3 (ROW4)
 *
 * MATRIX LOGIC:
 *     COL1  COL2  COL3
 * R1   1     2     3
 * R2   4     5     6
 * R3   7     8     9
 * R4   *     0     #
 *
 * Encoded key values: 1–9 as-is, '*'=10, '0'=11, '#'=12
 * For LED display, key '0' is remapped to 0xF (CODE_ZERO)
 *
 * REVISION HISTORY
 * 0.1
 *******************************************************************************
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32l4xx_hal.h"

// ------------------------ Boolean Constants ------------------------
#define TRUE 1
#define FALSE 0

// ------------------------ Keypad GPIO Mapping ------------------------
// COL_PORT = GPIOE: PE2 (COL1), PE4 (COL2), PE5 (COL3)
// ROW_PORT = GPIOD: PD6 (ROW1), PD5 (ROW2), PD4 (ROW3), PD3 (ROW4)

#define COL_PORT GPIOE
#define ROW_PORT GPIOD

// Columns (in logical order: COL1, COL2, COL3)
#define COL_PINS (GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5)
#define BIT0_COL GPIO_PIN_2 // First bit for column scanning

// Rows (in logical order: ROW1, ROW2, ROW3, ROW4)
#define ROW_PINS (GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3)
#define BIT0_ROW GPIO_PIN_6 // First bit for row scanning (ROW1 = PD6)

// Compatibility define for legacy use
#define BIT0 BIT0_ROW

// ------------------------ Keypad Geometry ------------------------
#define NUM_COLS 3
#define NUM_ROWS 4

// ------------------------ Timing ------------------------
#define SETTLE 1900 // ~20ms settle delay for signal to stabilize

// ------------------------ Keypad Key Encoding ------------------------
// Encoded values: 1–9 = keys 1–9
// '*' = 10, '#' = 12, '0' = 11 → remapped to 0xF for LED
#define NO_KEYPRESS 0
#define KEY_ZERO 11
#define CODE_ZERO 0xF // 0xF = 15 → 4-bit LED all ON for '0'

// ------------------------ Function Prototypes ------------------------
void Keypad_Config(void);
int Keypad_IsAnyKeyPressed(void);
int Keypad_WhichKeyIsPressed(void);

#endif /* INC_KEYPAD_H_ */
