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
 * The function does not return any value.
 * -------------------------------------------------------------------------- */
void Keypad_Config(void)  {

    // Enable GPIOF clock (used for both ROWs and COLs)
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOFEN;

    //Configure Columns: PF12, PF13, PF14 as OUTPUT
    GPIOF->MODER &= ~(GPIO_MODER_MODE12 | GPIO_MODER_MODE13 | 
                      GPIO_MODER_MODE14);
    GPIOF->MODER |=  (GPIO_MODER_MODE12_0 | GPIO_MODER_MODE13_0 |
                      GPIO_MODER_MODE14_0);  // Output mode

    GPIOF->OTYPER &= ~(GPIO_OTYPER_OT12 | GPIO_OTYPER_OT13 | GPIO_OTYPER_OT14);
    GPIOF->OSPEEDR |= (GPIO_OSPEEDR_OSPEED12 | GPIO_OSPEEDR_OSPEED13 |
                       GPIO_OSPEEDR_OSPEED14); // High speed
    GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPD12 | GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD14); 

    // --- Configure Rows: PF0, PF1, PF2, PF3 as INPUT with Pull-Down ---
    GPIOF->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | 
                      GPIO_MODER_MODE2 | GPIO_MODER_MODE3); // Input mode
    GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | 
                      GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
    GPIOF->PUPDR |=  (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 | 
                      GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1); // Pull-down
}

/* ---------------------------- Keypad_IsAnyKeyPressed()-----------------------
 * Keypad_IsAnyKeyPressed() checks if any key on the keypad is pressed.
 * It drives all columns high and checks if any row is high.
 * If a key is pressed, it returns TRUE; otherwise, it returns FALSE.
 * No debounce, just looking for a key twitch.
 * -------------------------------------------------------------------------- */
int Keypad_IsAnyKeyPressed(void) {
   COL_PORT->BSRR = COL_PINS;         	         // set all columns HI
   for ( uint16_t idx=0; idx<SETTLE; idx++ )   	// let it settle
      ;
   if ((ROW_PORT->IDR & ROW_PINS) != 0 )        // got a keypress!
      return( TRUE );                           
   else
      return( FALSE );                          // nope.
}
 
/* ---------------------------- Keypad_WhichKeyIsPressed()---------------------
 * Keypad_WhichKeyIsPressed() detects and encodes a pressed key at {row,col}
 * It assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE.
 * It verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
 * determines which key is pressed, and returns the encoded key ID.
 * -------------------------------------------------------------------------- */
int Keypad_WhichKeyIsPressed(void) {
   int8_t iRow=0, iCol=0, iKey=0;  // keypad row & col index, key ID result
   int8_t bGotKey = 0;             // bool for keypress, 0 = no press
 
   COL_PORT->BSRR = COL_PINS;                       	 // set all columns HI
   for ( iRow = 0; iRow < NUM_ROWS; iRow++ ) {      	 // check all ROWS
      if ( ROW_PORT->IDR & (BIT0 << iRow) ) {      	 // keypress in iRow!!
         COL_PORT->BRR = ( COL_PINS );            	 // set all cols LO
         for ( iCol = 0; iCol < NUM_COLS; iCol++ ) {   // 1 col at a time
            COL_PORT->BSRR = ( BIT0 << (4+iCol) );     // set this col HI
            if ( ROW_PORT->IDR & (BIT0 << iRow) ) {    // keypress in iCol!!
               bGotKey = 1;
               break;                                  // exit for iCol loop
            }
         }
         if ( bGotKey )
            break;
      }
   }
   //	encode {iRow,iCol} into LED word : row 1-3 : numeric, ‘1’-’9’
   //	                                   row 4   : ‘*’=10, ‘0’=15, ‘#’=12
   //                                    no press: send NO_KEYPRESS 
   if ( bGotKey ) {
      iKey = ( iRow * NUM_COLS ) + iCol + 1;  // handle numeric keys ...
 	if ( iKey == KEY_ZERO )                 //    works for ‘*’, ‘#’ too
         iKey = CODE_ZERO;
 	return( iKey );                         // return encoded keypress
   }
   return( NO_KEYPRESS );                     // unable to verify keypress
}
