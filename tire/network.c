#include "network.h"

short PosHistory[2000];
short PosHistorySWAP[2000];
short DesPosHistory[2000];
short DesPosHistorySWAP[2000];
short PWMHistory[2000];
short PWMHistorySWAP[2000];
short timemark = 0;

int FINISHED = 0;
struct sockaddr_in serverAddr,src,mngAddr;
struct sockaddr_in my_addr;//my adress
int sockd;
struct timeval tval;
int yes = 1;
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
  
  inet_pton(AF_INET,ip, &serverAddr);
  
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

  if (listen(s, SERVER_MAX_CONNECTIONS) == <0)
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
    
    /* The client connected from IP address inet_ntoa(clientAddr.sin_addr)
       and port ntohs(clientAddr.sin_port).

       Start a new task for each request. The task will parse the request
       and sends back the response.

       Don't forget to close newFd at the end */
    int service = taskSpawn("service", PRIORITY+2, 0, 4096, (FUNCPTR)serve, newFd, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

void serve(int fd) {
    FILE *tunnel = fdopen(fd, "w");
    fprintf(tunnel, "HTTP/1.0 200 OK\r\n\r\n");
    //fprintf(tunnel, "Current time is %ld.\n");
    int chr;
   // load html file
    
    FILE *source = fopen(WEBPAGE, "r");
    if(source == NULL){
        perror("incorrect html source");
    }
   // send all his contents
    while( (chr = fgetc(source) ) != EOF) {
        switch(chr) {
          case '&':
            fputc('\n',tunnel);
            for(int i=0; i<timemark; i++) {
                fprintf(tunnel, "{x: %d, y: %d},",i, PosHistory[i]);
            } break;
          case '@':
            fputc('\n',tunnel);
            for(int i=0; i<timemark; i++) {
                fprintf(tunnel, "{x: %d, y: %d},",i, DesPosHistory[i]);
            } break;
          case '$':
            fputc('\n',tunnel);
            for(int i=0; i<timemark; i++) {
                fprintf(tunnel, "{x: %d, y: %d},",i, PWMHistory[i]);
            } break;
          default:
            fputc(chr, tunnel);
        }
    }
    fclose(source);
    fclose(tunnel);
}

void initreceive(){
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
	    tval.tv_usec = 1000000;
	    if (setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO,&tval,sizeof(tval)) < 0) {
	        printf("failed to set timeout\n");
	        return;
	    }

	    if (bind(sockd, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
	        printf("failed to bind socket \n");
	        return;
 }
}

void connectionListener(){
    char buffer[11];
    s = socket(AF_INET, SOCK_DGRAM, 0);
    socklen_t srclen=sizeof(src);
    buffer[10] = 0;
    printf("in answererTask\n");
    ssize_t status;
    while(!FINISHED){
        status = recvfrom(s,buf,MAX_BUF,0,(struct sockaddr*)&src,&srclen);
        if(status < 0){
            //printf("ERROR: %s\n", strerror(errno));
            continue;
        }
        if (buffer[0] == 'P' && buffer[1] == 'W' && buf[fer2] == 'N' && buffer[3] == 'S' && buffer[4] == 'T' && buffer[5] == 'N') {
            //TODO musime to poslat do PIDčka
            memcpy(&desiredPosition, *(int *) buffer[6], 4);
            printf("prijat desiredPosition %d\n", desiredPosition);
            
            PosHistory[timemark] = position;
            DesPosHistory[timemark] = desiredPosition;
            //TODO speed * +- direction
            PWMHistory[timemark] = speed;
            if(timemark > 999){
                PosHistorySWAP[timemark-999] = position;
                DesPosHistory[timemark-999] = desiredPosition;
                //TODO speed * +- direction
                PWMHistory[timemark-999] = speed;
            }
            if(timemark == 1998) {
              timemark = 998;
              short *tmpptr = PosHistory;
              PosHistory = PosHistorySWAP;

              tmpptr = DesPosHistory;
              PosHistory = DesPosHistorySWAP;

              tmpptr = PWMHistory;
              PosHistory = PWMHistorySWAP;
            }
            timemark++;
        }
    }
    return;
}
