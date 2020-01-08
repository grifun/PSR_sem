/**
\file tire/network.c
*/
#include "network.h"

short* PosHistory;
short* PosHistorySWAP;
short* DesPosHistory;
short* DesPosHistorySWAP;
short* PWMHistory;
short* PWMHistorySWAP;
unsigned timemark = 0;

int FINISHED = 0;
struct sockaddr_in serverAddr,src,mngAddr;
struct sockaddr_in my_addr;//my adress
int sockd;
struct timeval tval;
int yes = 1;
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
    fprintf(tunnel, "<!DOCTYPE HTML>\n<html style='background-color: yellowgreen;'>\n<head>\n  <h1 align='center' style='color: red; font-size: 9pc;'><u>Motion Ownage</u></h1>\n  <h3 style='color: red; font-size: 2pc;'>Graphs:</h3>\n<script type='text/javascript' id='myHtml'>\nsetTimeout(function(){location.reload()}, 1000);\nwindow.onload = function () {\n  var chart = new CanvasJS.Chart('chartContainer', {\n	animationEnabled: false,  \n	title:{\n		text: 'Motor position'\n	},\n	axisY: {\n		title: 'Position',\n		valueFormatString: '#0,,.',\n		suffix: '',\n	},\n	data: [{\n    name: 'Desired position',\n    showInLegend: true,\n		yValueFormatString: '#,### Units',\n		xValueFormatString: '####',\n		type: 'spline',\n		dataPoints: [ //&\n		]\n	},\n  {\n    name: 'Actual position',\n    showInLegend: true,\n		yValueFormatString: '#,### Units',\n		xValueFormatString: '####',\n		type: 'spline',\n		dataPoints: [");
    int i;
    for(i=0; i<timemark; i++) {
        fprintf(tunnel, "{x: %d, y: %d},",i,PosHistory[i]);
    };
    fprintf(tunnel, "]\n	},\n  {\n    name: 'Desired position',\n    showInLegend: true,\n		yValueFormatString: '#,### Units',\n		xValueFormatString: '####',\n		type: 'spline',\n		dataPoints: [");
    for(i=0; i<timemark; i++) {
        fprintf(tunnel, "{x: %d, y: %d},",i,DesPosHistory[i]);
    };
    fprintf(tunnel, "]	}]});chart.render();\nvar chart2 = new CanvasJS.Chart('chartContainer2', {\n	animationEnabled: false,  \n	title:{\n		text: 'PMW level'\n	},\n	axisY: {\n		title: 'PMW level',\n		valueFormatString: '#0,,.',\n		suffix: '',\n	},\n	data: [{\n    name: 'PMW',\n		yValueFormatString: '#,### Units',\n		xValueFormatString: '####',\n		type: 'spline',\n		dataPoints: [");
    for(i=0; i<timemark; i++) {
    	fprintf(tunnel, "{x: %d, y: %d},",i, PWMHistory[i]);
    };
    fprintf(tunnel, "]\n	}]\n});\nchart2.render();\n}\n</script>\n</head>\n<body>\n<div align='center' id='chartContainer' style='height: 370px; width: 97%; margin: 2pc'></div>\n<div id='chartContainer2' style='height: 370px; width: 97%; margin: 2pc;'></div>\n<script src='https://canvasjs.com/assets/script/canvasjs.min.js'> </script>\n\n<footer style='font-size: 1pc;'>\n  By Tomas Kasl and Zdenek Syrovy<br>\n  FEL CTU 2020\n</footer>\n</body>\n</html>\n");
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
    int yes=1; 
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
                DesPosHistory[timemark-1000] = desiredPosition;
                if (direction > 1)
                	PWMHistory[timemark-1000] = -speed;
                else
                	PWMHistory[timemark-1000] = speed; 
        	}  		
            if(timemark == 1999) { //we got to the end of the container, we need to swap them
              timemark = 999;
              short *tmpptr = PosHistory;
              PosHistory = PosHistorySWAP;
              tmpptr = DesPosHistory;
              DesPosHistory = DesPosHistorySWAP;
              tmpptr = PWMHistory;
              PWMHistory = PWMHistorySWAP;
            }
            timemark++;
        }
    }
    return;
}
