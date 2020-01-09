#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

#include <unistd.h>

#define WEBPAGE "webpage.html"
#define SERVER_PORT     20000 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20

int FINISHED = 0;
struct sockaddr_in serverAddr,src,mngAddr;
struct sockaddr_in my_addr;//my adress
int sockd;
struct timeval tval;
int yes = 1;


void serve(int fd) {
	printf("Serving a HTML page\n");
    FILE *tunnel = fdopen(fd, "w");
    fprintf(tunnel, "HTTP/1.0 200 OK\r\n\r\n");
    fprintf(tunnel, "<?xml version='1.0'?>\n<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.0//EN' 'http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd'>\n<html style='background-color: yellowgreen;'>\n<head>\n  <h1 align='center' style='color: red; font-size: 9pc;'><u>Motion Ownage</u></h1>\n  <h3 style='color: red; font-size: 2pc;'>Graphs:</h3>\n</head>\n<body>\n<svg width='950' height='400' xmlns='http://www.w3.org/2000/svg' style='background:white;'>\n  <g transform='translate(170,200) scale(1.5)'>\n    <!-- Now Draw the main X and Y axis -->\n    <g style='stroke-width:2; stroke:black'>\n      <!-- X Axis -->\n      <path d='M 0 0 L 500 0 Z'/>\n      <!-- Y Axis -->\n      <path d='M 0 -120 L 0 120 Z'/>\n    </g>\n    <g style='fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:30'>\n      <text style='fill:black; stroke:none' x='-1' y='120' >-10000</text>\n      <text style='fill:black; stroke:none' x='-1' y='0' >0</text>\n      <text style='fill:black; stroke:none' x='-1' y='-100' >10000</text>\n      <g style='text-anchor:middle'>\n	<text style='fill:black; stroke:none' x='100' y='22' >-400</text>\n	<text style='fill:black; stroke:none' x='200' y='22' >-300</text>\n	<text style='fill:black; stroke:none' x='300' y='22' >-200</text>\n	<text style='fill:black; stroke:none' x='400' y='22' >-100</text>\n<text style='fill:green; stroke:none' x='120' y='125' >Actual position</text>\n<text style='fill:red; stroke:none' x='420' y='125' >Desired position</text>      </g>\n    </g>\n    <polyline\n	      points='");
    int i;
    for(i=0; i<500; i+=10) {
        fprintf(tunnel, " %d, %d\n",i, -2*i/100);
    };
    fprintf(tunnel, "'\n          style='stroke:red; stroke-width: 2; fill : none;'/>\n    <polyline\n	      points='");
    for(i=0; i<500; i+=10)  {
        fprintf(tunnel, " %d, %d\n",i, -(3+i)/3);
    };
    fprintf(tunnel, "'\n	      style='stroke:green; stroke-width: 2; fill : none;'/>\n  </g>\n</svg>\n<svg width='900' height='400' xmlns='http://www.w3.org/2000/svg' style='background:white;'>\n    <g transform='translate(120,200) scale(1.5)'>\n    <!-- Now Draw the main X and Y axis -->\n    <g style='stroke-width:2; stroke:black'>\n      <!-- X Axis -->\n      <path d='M 0 0 L 500 0 Z'/>\n      <!-- Y Axis -->\n      <path d='M 0 -100 L 0 100 Z'/>\n    </g>\n    <g style='fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:30'>\n      <text style='fill:black; stroke:none' x='-1' y='100' >-100</text>\n      <text style='fill:black; stroke:none' x='-1' y='0' >0</text>\n      <text style='fill:black; stroke:none' x='-1' y='-100' >100</text>\n      <g style='text-anchor:middle'>\n	<text style='fill:black; stroke:none' x='100' y='22' >-400</text>\n	<text style='fill:black; stroke:none' x='200' y='22' >-300</text>\n	<text style='fill:black; stroke:none' x='300' y='22' >-200</text>\n	<text style='fill:black; stroke:none' x='400' y='22' >-100</text> \n <text style='fill:blue; stroke:none' x='250' y='125' >PWM percentage</text>\n      </g>\n    </g>\n    <polyline\n	      points='");
    for(i=0; i<500; i+=10)  {
    	  fprintf(tunnel, " %d, %d\n",i, -(3+i) / 2);
    };
    fprintf(tunnel, "'\n	      style='stroke:blue; stroke-width: 2; fill : none;'/>\n  </g>\n</svg>\n  <footer style='font-size: 1pc;'>\n    By Tomas Kasl and Zdenek Syrovy<br>\n    FEL CTU 2020\n  </footer>\n</body>\n</html>");
    printf("serving done\n");
    fclose(tunnel);
}

void www()
{
  int s;
  int newFd;
  struct sockaddr_in clientAddr;
  int sockAddrSize;

  sockAddrSize = sizeof(struct sockaddr_in);
  bzero((char *) &serverAddr, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  
  s=socket(AF_INET, SOCK_STREAM, 0);
  if (s<0)
  {
    printf("Error: www: socket(%d)\n", s);
    return;
  }
  
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    printf("failed to set reuse\n");
    return;
  }

  if (bind(s, (struct sockaddr *) &serverAddr, sockAddrSize) == -1)
  {
    printf("Error: www: bind\n");
    return;
  }

  if (listen(s, SERVER_MAX_CONNECTIONS) < 0)
  {
    perror("www listen");
    close(s);
    return;
  }

  printf("www server running\n");

  while(1)
  {
    /* accept waits for somebody to connect and the returns a new file descriptor */
    if ((newFd = accept(s, (struct sockaddr *) &clientAddr, &sockAddrSize)) < 1)
    {
      perror("www accept");
      close(s);
      return;
    }
    serve(newFd);
    /* The client connected from IP address inet_ntoa(clientAddr.sin_addr)
       and port ntohs(clientAddr.sin_port).

       Start a new task for each request. The task will parse the request
       and sends back the response.

       Don't forget to close newFd at the end */
    
    //task create serve


  }
}


int main() {
    www();
}