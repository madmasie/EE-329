/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE 
 *******************************************************************************
 * @file           : main.c
 * @brief          : keypad configuration and debounced detection of keypresses
 * project         : EE 329 S'23 Assignment A2
 * authors         : Maddie Masiello
 * version         : 1
 * date            : 230413
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
* 230413 - Masiello - initial version
 *******************************************************************************
 * TODO
 * 
 *******************************************************************************
*/


#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported functions prototypes ---------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif
#endif 
