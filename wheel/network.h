#include <inetLib.h>
#include <sockLib.h>
#include <string.h> 
#include <stdio.h> 

#include "network.h"

#define MANAGEMENT_PORT 42420

void networkInit(char* ip);
void sendPacket(int position);