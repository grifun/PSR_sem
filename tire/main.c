#include "motor.h"
#include "network.h"
#include <time.h>
#include <semLib.h>
#include <taskLib.h>
#include <taskUtilLib.h>
#include <stdio.h>
#include <sysLib.h>
#define PRIORITY 100

int main(){
	//www();
	//motorWatcherInit();
	initreceive();
	//connectionListenerInit();
	int listener = taskSpawn("listener", PRIORITY, 0, 4096, (FUNCPTR)connectionListener, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	while(1);
}
