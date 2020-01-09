/**
\file tire/network.h
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sockLib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <taskLib.h>
#include "global.h"

#define SERVER_PORT     80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  200
#define MANAGEMENT_PORT 42420
#define PRIORITY 5
#define ifreq_offsetof(x)  offsetof(struct ifreq, x)

short* PosHistory;
short* PosHistorySWAP;
short* DesPosHistory;
short* DesPosHistorySWAP;
short* PWMHistory;
short* PWMHistorySWAP;
short* tmpptr;

int maxval;
SEM_ID sem;
struct timespec tim, tim2;

/** function called by thread responsible for updating web server */
void www();
void serve(int fd);
/** initiation of Listener*/
void connectionListenerInit();
/** receiving packets from wheel */
void connectionListener();
void initreceive();
