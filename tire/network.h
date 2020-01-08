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

void www();
void serve(int fd);
void connectionListenerInit();
void connectionListener();
void initreceive();
