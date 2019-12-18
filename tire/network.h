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

#define SERVER_PORT     80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20
#define MANAGEMENT_PORT 42420
#define WEBPAGE "webpage.html"
#define MAX_BUF 2000
#define IFNAME "eth0:2"
#define HOST "192.168.1.204"
#define ifreq_offsetof(x)  offsetof(struct ifreq, x)
void www();
void serve(int fd);
void connectionListenerInit();
void connectionListener();
void initreceive();
void changeIP();
