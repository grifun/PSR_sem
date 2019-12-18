#include "network.h"

int FINISHED = 0;
struct sockaddr_in serverAddr,src,mngAddr;
struct sockaddr_in my_addr;//my adress
int sockd;
struct timeval tval;
int yes = 1;
char ip[15] = "192.168.202.242";
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

  if (setsockopt(sockd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
      printf("failed to set reuse\n");
      return;
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
    int chr;
   // load html file
    FILE *source = fopen(WEBPAGE, "r");
    if(source == NULL){
        perror("incorrect html source");
    }
   // send all his contents
    while( (chr = fgetc(source) ) != EOF)
        fputc(chr, tunnel);
    
    fclose(source);
    fclose(tunnel);
}

//TODO somehow finish this function
void changeIP() {
	 struct ifreq ifr;
	    const char * name = "eth1";
	    int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	    strncpy(ifr.ifr_name, name, IFNAMSIZ);

	    ifr.ifr_addr.sa_family = AF_INET;
	    inet_pton(AF_INET, "10.12.0.1", ifr.ifr_addr.sa_data + 2);
	    ioctl(fd, SIOCSIFADDR, &ifr);

	    inet_pton(AF_INET, "255.255.0.0", ifr.ifr_addr.sa_data + 2);
	    ioctl(fd, SIOCSIFNETMASK, &ifr);

	    ioctl(fd, SIOCGIFFLAGS, &ifr);
	    strncpy(ifr.ifr_name, name, IFNAMSIZ);
	    ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);

	    ioctl(fd, SIOCSIFFLAGS, &ifr);

  //task spawn connectionListener
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
    char buf[2000];
    s = socket(AF_INET, SOCK_DGRAM, 0);
    socklen_t srclen=sizeof(src);
    buf[MAX_BUF-1] = 0;
    printf("in answererTask\n");
    ssize_t status;
    while(!FINISHED){
        status = recvfrom(s,buf,MAX_BUF,0,(struct sockaddr*)&src,&srclen);
        if(status < 0){
            //printf("ERROR: %s\n", strerror(errno));
            continue;
        }
        if (buf[0] == 'P' && buf[1] == 'W' && buf[2] == 'N' && buf[3] == 'S' && buf[4] == 'T' && buf[5] == 'N') {
            //TODO musime to poslat do PIDčka
            printf("mame OWN! packen \n");
        }
    }
    return;
}
