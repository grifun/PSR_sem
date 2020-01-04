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
#define SERVER_MAX_CONNECTIONS  20000

int FINISHED = 0;
struct sockaddr_in serverAddr,src,mngAddr;
struct sockaddr_in my_addr;//my adress
int sockd;
struct timeval tval;
int yes = 1;



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
    while( (chr = fgetc(source) ) != EOF)
        switch(chr) {
          case '&':
            fputc('\n',tunnel);
            for(int i=0; i<100; i++) {
                fprintf(tunnel, "{x: %d, y: %d},",i, i+1);
            } break;
          case '@':
            fputc('\n',tunnel);
            for(int i=0; i<100; i++) {
                fprintf(tunnel, "{x: %d, y: %d},",i, 99-i);
            } break;
          default:
            fputc(chr, tunnel);
        }
      fclose(source);
    
    /*
    fprintf(tunnel, 
    "<html>\
    <head>\
      <title style='background-color: yellow;'>Motion Control</title>\
    </head>\
    <body onload='setTimeout(function(){location.reload()}, 1000);'\
          style='background-color: yellowgreen;'>>\
      <script>document.write(Date.now());</script>\
      <h1>Motion control</h1>\
    </body>\
    <footer>Created by Tomas Kasl and Zdenek Syrovy</footer>\
    </html>");
    */
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
  //inet_pton(AF_INET,ip, &serverAddr);
  
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