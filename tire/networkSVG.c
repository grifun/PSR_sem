/**
\file tire/network.c
*/
#include "network.h"
unsigned timemark = 0;
int FINISHED = 0;
int sockd;
int yes = 1;
struct sockaddr_in serverAddr, src, mngAddr, my_addr;
struct timeval tval;
/** creates a connection to web server and maintains it*/
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
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
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
    if ((newFd = accept(s, (struct sockaddr *) &clientAddr, &sockAddrSize)) == ERROR)
    {
      perror("www accept");
      close(s);
      return;
    }
    taskSpawn("service", PRIORITY+2, 0, 4096, (FUNCPTR)serve, newFd, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}
/**
* creates web page to be seen on ip adress same as adress of wheel.
**/
void serve(int fd) {
	printf("Serving a HTML page\n");
    FILE *tunnel = fdopen(fd, "w");
    fprintf(tunnel, "HTTP/1.0 200 OK\r\n\r\n");
    fprintf(tunnel, "<?xml version='1.0'?>\n<!DOCTYPE svg PUBLIC '-//W3C//DTD SVG 1.0//EN' 'http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd'>\n<html style='background-color: yellowgreen;'>\n<head>\n  <h1 align='center' style='color: red; font-size: 9pc;'><u>Motion Ownage</u></h1>\n  <h3 style='color: red; font-size: 2pc;'>Graphs:</h3>\n</head>\n<body>\n<svg width='950' height='400' xmlns='http://www.w3.org/2000/svg' style='background:white;'>\n  <g transform='translate(170,200) scale(1.5)'>\n    <!-- Now Draw the main X and Y axis -->\n    <g style='stroke-width:2; stroke:black'>\n      <!-- X Axis -->\n      <path d='M 0 0 L 500 0 Z'/>\n      <!-- Y Axis -->\n      <path d='M 0 -120 L 0 120 Z'/>\n    </g>\n    <g style='fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:30'>\n      <text style='fill:black; stroke:none' x='-1' y='120' >-10000</text>\n      <text style='fill:black; stroke:none' x='-1' y='0' >0</text>\n      <text style='fill:black; stroke:none' x='-1' y='-100' >10000</text>\n      <g style='text-anchor:middle'>\n	<text style='fill:black; stroke:none' x='100' y='22' >-400</text>\n	<text style='fill:black; stroke:none' x='200' y='22' >-300</text>\n	<text style='fill:black; stroke:none' x='300' y='22' >-200</text>\n	<text style='fill:black; stroke:none' x='400' y='22' >-100</text>\n<text style='fill:green; stroke:none' x='120' y='125' >Actual position</text>\n<text style='fill:red; stroke:none' x='420' y='125' >Desired position</text>      </g>\n    </g>\n    <polyline\n	      points='");
    int i;
    int moment=timemark;
    for(i=0; i<moment; i++) {
        fprintf(tunnel, " %d, %d\n", i, PosHistory[i]);
    };
    fprintf(tunnel, "'\n          style='stroke:red; stroke-width: 2; fill : none;'/>\n    <polyline\n	      points='");
    for(i=0; i<moment; i++)  {
        fprintf(tunnel, " %d, %d\n", i, DesPosHistory[i]);
    };
    fprintf(tunnel, "'\n	      style='stroke:green; stroke-width: 2; fill : none;'/>\n  </g>\n</svg>\n<svg width='900' height='400' xmlns='http://www.w3.org/2000/svg' style='background:white;'>\n    <g transform='translate(120,200) scale(1.5)'>\n    <!-- Now Draw the main X and Y axis -->\n    <g style='stroke-width:2; stroke:black'>\n      <!-- X Axis -->\n      <path d='M 0 0 L 500 0 Z'/>\n      <!-- Y Axis -->\n      <path d='M 0 -100 L 0 100 Z'/>\n    </g>\n    <g style='fill:none; stroke:#B0B0B0; stroke-width:1; stroke-dasharray:2 4;text-anchor:end; font-size:30'>\n      <text style='fill:black; stroke:none' x='-1' y='100' >-100</text>\n      <text style='fill:black; stroke:none' x='-1' y='0' >0</text>\n      <text style='fill:black; stroke:none' x='-1' y='-100' >100</text>\n      <g style='text-anchor:middle'>\n	<text style='fill:black; stroke:none' x='100' y='22' >-400</text>\n	<text style='fill:black; stroke:none' x='200' y='22' >-300</text>\n	<text style='fill:black; stroke:none' x='300' y='22' >-200</text>\n	<text style='fill:black; stroke:none' x='400' y='22' >-100</text> \n <text style='fill:blue; stroke:none' x='250' y='125' >PWM percentage</text>\n      </g>\n    </g>\n    <polyline\n	      points='");
    for(i=0; i<moment; i++) {
    	  fprintf(tunnel, " %d, %d\n", i, PWMHistory[i]);
    };
    fprintf(tunnel, "'\n	      style='stroke:blue; stroke-width: 2; fill : none;'/>\n  </g>\n</svg>\n  <footer style='font-size: 1pc;'>\n    By Tomas Kasl and Zdenek Syrovy<br>\n    FEL CTU 2020\n  </footer>\n</body>\n</html>");
    printf("serving done\n");
    fclose(tunnel);
}
/**
* init all necessary adresses socket etc. to be able to receive packets, prepares memory for graphs data
*/
void connectionListenerInit(){
    PosHistory = malloc(2000*sizeof(short));
    PosHistorySWAP = malloc(2000*sizeof(short));
    DesPosHistory = malloc(2000*sizeof(short));
    DesPosHistorySWAP = malloc(2000*sizeof(short));
    PWMHistory = malloc(2000*sizeof(short));
    PWMHistorySWAP = malloc(2000*sizeof(short));

    if ((sockd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("Socket creation error\n");
        return;
    }
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(MANAGEMENT_PORT);
    memset(&(my_addr.sin_zero), '\0', 8);
    if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        printf("failed to set reuse\n");
        return;
    }
    tval.tv_usec = 10000;
    if (setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO,&tval,sizeof(tval)) < 0) {
        printf("failed to set timeout\n");
        return;
    }

    if (bind(sockd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        printf("failed to bind socket \n");
        return;
    }
}
/**
* Listen to socket and waits till packet with information about position arrives, after it does change desired position to position that arrived from wheel.
*/
void connectionListener(){
    char buffer[11];
    socklen_t srclen=sizeof(src);
    buffer[10] = 0;
    printf("in answererTask\n");
    ssize_t status;
    while(!FINISHED){
        status = recvfrom(sockd,buffer,11,0,(struct sockaddr*)&src,&srclen);
        if(status < 0){//timed-out
            continue;
        }
        if (buffer[0] == 'P' && buffer[1] == 'W' && buffer[2] == 'N' && buffer[3] == 'S' && buffer[4] == 'T' && buffer[5] == 'N') {
            memcpy(&desiredPosition, buffer+6, 4);
            /**
             * save the measured data into data containers used for plotting
            */
            PosHistory[timemark] = position;
            DesPosHistory[timemark] = desiredPosition;
            //left or right
            if (direction > 1)
                PWMHistory[timemark] = -speed;
            else	
                PWMHistory[timemark] = speed;
            
            if(timemark > 999){
                PosHistorySWAP[timemark-1000] = position;
                DesPosHistorySWAP[timemark-1000] = desiredPosition;
                if (direction > 1)
                	PWMHistorySWAP[timemark-1000] = -speed;
                else
                	PWMHistorySWAP[timemark-1000] = speed; 
        	}  		
            if(timemark == 1999) { //we got to the end of the container, we need to swap them
              timemark = 999;
              short *tmpptr = PosHistory;
              PosHistory = PosHistorySWAP;
              PosHistorySWAP = tmpptr;
              tmpptr = DesPosHistory;
              DesPosHistory = DesPosHistorySWAP;
              DesPosHistorySWAP = tmpptr;
              tmpptr = PWMHistory;
              PWMHistory = PWMHistorySWAP;
              PWMHistorySWAP = tmpptr;
            }
            timemark++;
        }
    }
    return;
}