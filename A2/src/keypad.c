/* USER CODE BEGIN Header */
/*******************************************************************************
 * EE 329 A2 KEYPAD INTERFACE
 *******************************************************************************
 * @file           : keypad.c
 * @brief          : keypad configuration and debounced detection of keypresses
 * project         : EE 329 S'23 Assignment A2
 * authors         : Maddie Masiello, Tatiana Ribay
 * version         : 0.1
 * date            :
 * compiler        : STM32CubeIDE v.1.12.0 Build: 14980_20230301_1550 (UTC)
 * target          : NUCLEO-L4A6ZG
 * clocks          : 4 MHz MSI to AHB2
 * @attention      : (c) 2023 STMicroelectronics.  All rights reserved.
 *******************************************************************************
 * Keypad Wiring 4 ROWS 3 COLS (pinout NUCLEO-L4A6ZG = L496ZG)
 *******************************************************************************
 * REVISION HISTORY
 * 0.1
 *******************************************************************************
 */

 #include "keypad.h"

 /* ---------------------------- Keypad_Config()-------------------------------
  * Keypad_Config() initializes the GPIO pins for the keypad interface.
  * The keypad is a 4x3 matrix with 4 rows and 3 columns.
  * The rows are configured as inputs with pull-down resistors,
  * and the columns are configured as outputs.
  * The function enables the GPIOF clock and sets the appropriate mode, type,
  * speed, and pull-up/pull-down settings for the pins.
  * -------------------------------------------------------------------------- */
 void Keypad_Config(void)
 {
    // Enable Clocks
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
 
    // Configure PE2, PE4, PE5 (Columns) as Output
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
 
    // Configure PD3, PD4, PD5, PD6 (Rows) as Input with Pull-Down
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
 }
 
 /* ---------------------------- Keypad_IsAnyKeyPressed()-----------------------
  * Keypad_IsAnyKeyPressed() checks if any key on the keypad is pressed.
  * It drives all columns high and checks if any row is high.
  * If a key is pressed, it returns TRUE; otherwise, it returns FALSE.
  * No debounce, just looking for a key twitch.
  * -------------------------------------------------------------------------- */
 int Keypad_IsAnyKeyPressed(void)
 {
    // drive all COLUMNS HI; see if any ROWS are HI
    // return true if a key is pressed, false if not
    // currently no debounce here - just looking for a key twitch
    COL_PORT->BSRR = COL_PINS;                  // set all columns HI
    for (uint16_t idx = 0; idx < SETTLE; idx++) // let it settle
       ;
    if ((ROW_PORT->IDR & ROW_PINS) != 0) // got a keypress!
       return (TRUE);
    else
       return (FALSE); // nope.
 }
 
 /* ---------------------------- Keypad_WhichKeyIsPressed()---------------------
  * Keypad_WhichKeyIsPressed() detects and encodes a pressed key at {row,col}
  * It assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE.
  * It verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
  * determines which key is pressed, and returns the encoded key ID.
  * -------------------------------------------------------------------------- */
 int Keypad_WhichKeyIsPressed(void)
 {
    int8_t iRow = 0, iCol = 0;
    int8_t bGotKey = 0;
    int iKey = NO_KEYPRESS;
 
    const uint16_t colMap[NUM_COLS] = {GPIO_PIN_2, GPIO_PIN_4, GPIO_PIN_5};
    const uint16_t rowMap[NUM_ROWS] = {GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3};
 
    // Step 1: Set all columns high
    COL_PORT->BSRR = COL_PINS;
 
    for (iRow = 0; iRow < NUM_ROWS; iRow++)
    {
       if (ROW_PORT->IDR & rowMap[iRow])
       {
          // Step 2: Pull all columns low
          COL_PORT->BRR = COL_PINS;
 
          // Step 3: Raise one column at a time
          for (iCol = 0; iCol < NUM_COLS; iCol++)
          {
             COL_PORT->BSRR = colMap[iCol];
             if (ROW_PORT->IDR & rowMap[iRow])
             {
                bGotKey = 1;
                break;
             }
             COL_PORT->BRR = colMap[iCol]; // Reset column
          }
          if (bGotKey)
             break;
       }
    }
 
    if (bGotKey)
    {
       iKey = (iRow * NUM_COLS) + iCol + 1;
       if (iKey == KEY_ZERO)
          iKey = CODE_ZERO;
    }
 
    return iKey;
 }