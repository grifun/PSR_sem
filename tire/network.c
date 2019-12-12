#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sockLib.h>
#include <string.h>

#define SERVER_PORT     80 /* Port 80 is reserved for HTTP protocol */
#define SERVER_MAX_CONNECTIONS  20
#define MANAGEMENT_PORT 42420
void www()
{
  int s;
  int newFd;
  struct sockaddr_in serverAddr;
  struct sockaddr_in clientAddr;
  int sockAddrSize;

  sockAddrSize = sizeof(struct sockaddr_in);
  bzero((char *) &serverAddr, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(SERVER_PORT);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  inet_pton(AF_INET, "192.168.202.242", &serverAddr->sin_addr); //???
  
  s=socket(AF_INET, SOCK_STREAM, 0);
  if (s<0)
  {
    printf("Error: www: socket(%d)\n", s);
    return;
  }

  if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      printf("failed to set reuse\n");
      exit(1);
  }

  if (bind(s, (struct sockaddr *) &serverAddr, sockAddrSize) == ERROR)
  {
    printf("Error: www: bind\n");
    return;
  }

  if (listen(s, SERVER_MAX_CONNECTIONS) == ERROR)
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
    
    //task create serve


  }
}

void serve(int fd) {
    FILE *tunnel = fdopen(fd, "w");
    fprintf(tunnel, "HTTP/1.0 200 OK\r\n\r\n");
    fprintf(tunnel, "Current time is %ld.\n", time(NULL));

    /*load html file*/
    FILE *source = fopen(WEBPAGE, "r");
    if(source == NULL){
        perror("incorrect html source");
    }
    /*send all his contents*/
    while( (chr = fgetc(source) ) != EOF)
        fputc(chr, tunnel);
    
    fclose(source)
    fclose(f);
}

void connectionListenerInit() {
  int sockd;
  struct sockaddr_in mngAddr;
  int sockAddrSize;

  sockAddrSize = sizeof(struct sockaddr_in);
  bzero((char *) &serverAddr, sizeof(struct sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(MANAGEMENT_PORT);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  inet_pton(AF_INET, "192.168.202.242", &serverAddr->sin_addr); //???

  sockd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockd == -1) {
      printf("Socket creation error\n");
      FINISHED = 1;
      return;
  }

  if ((sockd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
      perror("Socket creation error");
      exit(1);
  }

  if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      printf("failed to set reuse\n");
      exit(1);
  }

  struct timeval tval;
  tval.tv_usec = 1000000;
  if (setsockopt(sockd, SOL_SOCKET, SO_RCVTIMEO,&tval,sizeof(tval)) < 0) {
      printf("failed to set timeout\n");
  }
    
  if (bind(sockd, (struct sockaddr *)&src, sizeof(src)) == -1) {
      printf("failed to bind socket \n");
      exit(1);
  }

  //task spawn connectionListener
}

void connectionListener(){
    int i;
    char buf[2000];
    uint32_t srclen=sizeof(src);
    buf[MAX_BUF-1] = 0;
    printf("in answererTask\n");
    while(!FINISHED){
        status = recvfrom(sockd,buf,MAX_BUF,0,(struct sockaddr*)&src,&srclen);
        if(status < 0){
            printf("ERROR: %s\n", strerror(errno));
            continue;
        }
        if (buf[0] == 'O' && buf[1] == 'W' && buf[2] == 'N') {
            //TODO musime to poslat do PIDčka
            printf("mame OWN! packen \n");
        }
    }
    return;
}