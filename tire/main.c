#include "motor.h"
#include "network.h"
#include <time.h>
#include <semLib.h>
#include <taskLib.h>
#include <taskUtilLib.h>
#include <stdio.h>
#include <sysLib.h>
#include "global.h"

// network listener priority
#define LISTENER_PRIORITY 100
// motor rotation priority
#define PID_PRIORITY 150

int main(){
	//www();
	desiredPosition = 0;
	motorInit();
	motorWatcherInit();
	initreceive();
	taskSpawn("listener", PRIORITY, 0, 4096, (FUNCPTR)connectionListener, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	taskSpawn("pid",LISTENER_PRIORITY, 0, 4096, (FUNCPTR)PID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	//rotate(50, LEFT);
	while(1);
}
