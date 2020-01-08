/**
   \file wheel/motor.h
 */
/**
 * motor related interface
 */


volatile unsigned irq_count;
volatile char a, b, prev_a, prev_b;
volatile int position;

/**
 * interupt handler for engine
 */
void motorWatcher();
/**
 * initiation of all relevant registers and constants
 */
void motorWatcherInit();
/**
 * disables and disconnects
 */
void motorWatcherCleanup();
