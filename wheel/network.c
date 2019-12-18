

#include <inetLib.h>
#include <sockLib.h>
#include <string.h> 
#include <stdio.h> 

#include "network.h"

#define TARGET_IP "192.168.202.242"
#define MANAGEMENT_PORT 42420

struct sockaddr_in my_addr, target_addr;
int sockd, addrlen;
struct timeval tval;
char buffer[11];

void networkInit(){
    tval.tv_sec = 0;
    tval.tv_usec = 1000000;

    if ((sockd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Socket creation error\n");
        return;
    }

    memset(&target_addr, 0, sizeof(target_addr));
    target_addr.sin_family = AF_INET;
    target_addr.sin_addr.s_addr = inet_addr(TARGET_IP);
    target_addr.sin_port = htons(MANAGEMENT_PORT);

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(MANAGEMENT_PORT);

    int yes=1; 
    if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("failed to set reuse\n");
        return;
    }

    if (setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO,&tval,sizeof(tval)) < 0) {
        printf("failed to set timeout\n");
        return;
    }

    if (bind(sockd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        printf("failed to bind socket \n");
        return;
    }
    strcpy(buffer, "PWNSTN");
    
}

void sendPacket(int position) {
	memcpy(buffer+6, &position, 4);
	printf("sending packet %d \n",*(int*)(buffer+6));
	sendto(sockd, buffer, 11, 0,(struct sockaddr*)&target_addr, sizeof(target_addr));
	
}