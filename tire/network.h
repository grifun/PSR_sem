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

#define BASE_DUTY 0xB0/64
#define SERVER_PORT     80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20
#define MANAGEMENT_PORT 42420
#define WEBPAGE "webpage.html"
#define PRIORITY 100
#define ifreq_offsetof(x)  offsetof(struct ifreq, x)

short* PosHistory;
short* PosHistorySWAP;
short* DesPosHistory;
short* DesPosHistorySWAP;
short* PWMHistory;
short* PWMHistorySWAP;
unsigned timemark = 0;

int FINISHED = 0;
int sockd;
int yes = 1;
struct sockaddr_in serverAddr, src, mngAddr, my_addr;
struct timeval tval;

/** function called by thread responsible for updating web server */
void www();
void serve(int fd);
/** initiation of Listener*/
void connectionListenerInit();
/** receiving packets from wheel */
void connectionListener();
void initreceive();
