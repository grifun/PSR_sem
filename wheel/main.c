#include <taskLib.h>
#include <stdio.h>
#include <kernelLib.h>
#include <semLib.h>
#include <intLib.h>
#include <iv.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


#include <xlnx_zynq7k.h>

#include "motor.h"
#include "network.h"

void sender() {
	struct timespec tim, tim2;
	tim.tv_sec  = 0;
	tim.tv_nsec = 100000000L;
	while(1){
		//printf("position: %d \n", position);
		sendPacket(position);
		nanosleep(&tim , &tim2);
	}
}


int main(int argc, char[][] argv) {
	motorWatcherInit();
	networkInit(argv[1]);
	sender();
    
    motorWatcherCleanup()
	return 0;
}