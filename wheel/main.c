/**
   \file wheel_main.c
 * Global variables for entire wheel
 */
#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>
#include <semLib.h>
#include <intLib.h>
#include <iv.h>
#include <unistd.h>
#include <time.h>
#include <xlnx_zynq7k.h>
#include "motor.h"
#include "network.h"
/**
 * sending period
 */
#define NANO_WAIT 100000000L
#define SEC_WAIT 0
#define TIRE_ADDRESS "192.168.202.205"
/**
 * sends periodicly packet to ip adress specified in main program argument
 */
void sender() {
	struct timespec tim, tim2;
	tim.tv_sec  = SEC_WAIT;
	tim.tv_nsec = NSEC_WAIT;
	while(1){
		//printf("position: %d \n", position);
		sendPacket(position);
		nanosleep(&tim , &tim2);
	}
}

/**
 * Initiate all relevant constants and sends periodicly packet
 */
int main() {
	motorWatcherInit();
	networkInit(TIRE_ADDRESS);
	sender();
    
    motorWatcherCleanup();
	return 0;
}
