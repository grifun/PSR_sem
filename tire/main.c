/**
 *\file tire/main.c
 * starting point for tire
 */
 
#include "motor.h"
#include "network.h"
#include <time.h>
#include <semLib.h>
#include <taskLib.h>
#include <taskUtilLib.h>
#include <stdio.h>
#include <sysLib.h>
#include "global.h"

/** network listener priority */
#define PRIORITY 5

int main(){
	desiredPosition = 0;
	motorInit();
	motorWatcherInit();
	connectionListenerInit();
	printf("initialization succesful \n");
	taskSpawn("listener", PRIORITY, 0, 4096, (FUNCPTR)connectionListener, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	printf("waiting for position info \n");
	taskSpawn("pid",PRIORITY-1, 0, 4096, (FUNCPTR)PID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	taskSpawn("www",PRIORITY, 0, 4096, (FUNCPTR)www, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}
