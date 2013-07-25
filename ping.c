#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

/*
 *  USAGE: ping IP_ADDRESS
 * 
*/
int main(int argc, char* argv[]) {
  if ( argc != 2 ) {
    fprintf(stderr,"Use the correct argument, dumbass. (ping IP-ADDR)\n");
    exit(1);
  }

  char* host = argv[1];
  int sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  
  if ( sock < 0 ) {
    perror("socket");
    exit(1);

  }

  struct addrinfo *ai;
  if (! getaddrinfo ( host, NULL, NULL, &ai ) ) { 
    printf(inet_ntoa(((struct sockaddr_in*)ai->ai_addr)->sin_addr));
  }
}
