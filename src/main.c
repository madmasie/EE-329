/* Project A1: EE 329 - Leverages given code to implement two separate solutions: 
a binary count to 15 using LEDs and an instruction execution timer. */

/* Instructions:
(a) LED Counter:
    1. Connect 4 LEDs to GPIO pins PC0, PC1, PC2, and PC3. (series LED and 560 Ω R).
    Use data on the LED datasheet to calculate the expected current from a VOH output then measure
    the actual GPIO output voltage, LED voltage, and LED ON current. 

    2. Write code in main() that repeatedly 
    counts from 0 to 15 and displays the binary count on the 4 LEDs. Use a calibrated software delay 
    (empty ‘for’ loop) between counts to make the LED blinks visible. Attach an oscilloscope to the LSb LED 
    to measure the for loop software delay for a few different delay settings and estimate the delay per for loop iteration. 

(b) Measure Execution Timing:
    1.Further leverage ExecutionTime.c, specifically the TestFunction() function, to add instruction execution 
    timing to your main() routine. Code it so that the execution timing starts after 3 iterations through 
    the count to 15 on the LEDs then repeats continuously. 

    2. Set up an oscilloscope or logic analyzer to capture
    the pulses on PC0 and PC1 to measure the durations of (a) the call to TestFunction() and (b) the 
    instruction inside TestFunction(). Notice that PC0 is toggled before and after the call to TestFunction()
    and PC1 is toggled before and after the arithmetic instruction inside the function. 

    3. Change var_type and TestFunction(num) to test the combinations listed in Table A1(b). 

    4. Run the code iteratively with the prescribed
    settings to complete the table. To assess the delay entering the function, measure the interval from PC0
    going high to PC1 going high. This ‘function call latency’ will not vary for different arithmetic
    instructions inside the function, so use the simple ‘test_var = num’ case to fill row 1 of the table. 

    Hints: Modify ExecutionTime.c to call TestFunction() repeatedly to ease oscilloscope analysis. Consider 
    using a typedef declaration to facilitate quick switching of var_type between tests.
    */


// file ExecutionTime.c sample code for A1 
#include "main.h"
#include <math.h>

typedef uint32_t var_type;  //used to test
void SystemClock_Config(void);
var_type TestFunction(var_type num);
 
void main(void)  {
  var_type main_var;
 
  HAL_Init();
  SystemClock_Config();
 
  // configure GPIO pins PC0, PC1 for: 
  // output mode, push-pull, no pull up or pull down, high speed
  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIOCEN); //enable clock for GPIOC

  
  GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 |
                    GPIO_MODER_MODE2 | GPIO_MODER_MODE3);   // Clear mode bits
    
    
  GPIOC->MODER |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 |
                    GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0); // Set as output (01)

  // Set PC0–PC3 output type to push-pull
  GPIOC->OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 |
                     GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3); // 0 = push-pull


   // Disable pull-up/pull-down resistors on PC0–PC3
   GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |
                     GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3); // 00 = no pull        
  
  //sets speed of PC0-PC3 to high speed
  //00 = low speed, 01 = medium speed, 10 = high speed, 11 = very high speed
  GPIOC->OSPEEDR |= ((3 << GPIO_OSPEEDR_OSPEED0_Pos) |  // PC0 high speed
                   (3 << GPIO_OSPEEDR_OSPEED1_Pos) |  // PC1 high speed
                   (3 << GPIO_OSPEEDR_OSPEED2_Pos) |  // PC2 high speed
                   (3 << GPIO_OSPEEDR_OSPEED3_Pos));  // PC3 high speed (11)

  //reset the output pins to 0, LEDs off
  GPIOC->BRR = (GPIO_PIN_0 | GPIO_PIN_1); // preset PC0, PC1 to 0

  // time the test function call using PC0 
  GPIOC->BSRR = (GPIO_PIN_0);             // turn on PC0
  main_var = TestFunction(15);            // call test function
  GPIOC->BRR = (GPIO_PIN_0);              // turn off PC0
  
  //loop counting 0 to 15
  for (int repear = 0; repear < 3, repeat++)  //for loop to repeat 3x
    for (int i = 0; i <16, i++) {
        GPIOC->ODF = (GPIOC->ODR & 0x0F) | (i & 0x0F); //set output to PC0-P3
        for (volatile int j = 0; j < 100000; j++); // delay loop
    }
}


//part b
//infinite loop to repeat the function call and measure execution time
  while (1) {      // infinite loop to avoid program exit
    GPIOC->BSRR = GPIO_PIN_0;     //set PC0 high to start timing
    main_var = TestFunction(15);  //call
    GPIOC->BRR = GPIO_PIN_0;      //set PC0 low to stop timing
}
 
var_type TestFunction(var_type num) {
 
  var_type test_var;  				// local variable
 
  GPIOC->BSRR = (GPIO_PIN_1);             // turn on PC1
  /* USER insert test function here e.g. test_var = num; */
  GPIOC->BRR = (GPIO_PIN_1);              // turn off PC1
  
  return test_var;
}


