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
 * main section of the program section of the program
 */

/// sending packet period constants

#define NANO_WAIT 100000000L
#define SEC_WAIT 0
/**
 * sends periodicly packet to ip adress specified in main program argument
 */
void sender() {
	struct timespec tim, tim2;
	tim.tv_sec  = SEC_WAIT;
	tim.tv_nsec = NANO_WAIT;
	while(1){
		//printf("position: %d \n", position);
		sendPacket(position);
		nanosleep(&tim , &tim2);
	}
}

/**
 * Initiate all relevant constants and sends periodicly packet
 */
int main(int argc, char *argv[]) {
	motorWatcherInit();
	networkInit(argv[1]);
	sender();
    
    motorWatcherCleanup();
	return 0;
}
