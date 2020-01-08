/**
 *  \file wheel/motor.c
 * Global variables
 */
/** actual position, desired position, and actual speed, needed for communication between network and motor */
volatile int desiredPosition,position, speed;
/** for graphs */
volatile unsigned epoch;
/** LEFT or RIGHT*/
volatile char direction;
