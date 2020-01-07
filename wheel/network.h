#include <inetLib.h>
#include <sockLib.h>
#include <string.h> 
#include <stdio.h> 

struct sockaddr_in my_addr, target_addr;
int sockd, addrlen;
struct timeval tval;
char buffer[11];


/**
 * basic Network Interface for sending information about tire
 */

#define MANAGEMENT_PORT 42420

/**
 * function to initiate network related stuff
 * socket, ip adresses   
 * @param ip adress in form of string
 * 
 */
void networkInit(char* ip);

/** Function that sends packet to ip adress specified in networkInit. 
 * @param position is rotation from zero position. Positive when rotated right, negative otherwise
 */
void sendPacket(int position);
