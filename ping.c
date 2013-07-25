#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdint.h>

/*
 *  USAGE: ping IP_ADDRESS
 * 
*/

void ping(struct sockaddr*, int);

int main(int argc, char* argv[]) {

  int sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  
  if ( sock < 0 ) {
    perror("socket");
    exit(1);

  } 
  if ( argc != 2 ) {
    fprintf(stderr,"Use the correct argument, dumbass. (ping IP-ADDR)\n");
    exit(1);
  }

  char* host = argv[1];


  struct addrinfo *ai;
  if (getaddrinfo ( host, NULL, NULL, &ai ) < 0 ) { 
    perror("getaddrinfo");
    exit(1);
  }

  ping(ai->ai_addr, sock);

  struct icmp message;
  struct sockaddr_storage sendr; //like tumblr, fuck you
  int fromlen = sizeof(sendr);
  if (recvfrom(sock, &message, sizeof(message),
        0, (struct sockaddr*) &sendr, &fromlen ) < 0) {
    perror("recvfrom");
    exit(1);
  }

  printf("%d\n",message.icmp_hun.ih_idseq.icd_id);

  
}


void ping (struct sockaddr* ai_addr, int sock) {
  struct icmp message;
  message.icmp_type = ICMP_ECHO;        // 8
  message.icmp_code = ICMP_ECHOREPLY;  // 0
  message.icmp_cksum = 0;
  message.icmp_hun.ih_idseq.icd_id = 1337; // we r so leet
  message.icmp_hun.ih_idseq.icd_seq = 0;
  memset(&message.icmp_dun, 0, sizeof(message.icmp_dun));

  uint32_t sum = 0;
  int i;
  for (i = 0; i < sizeof(message)/2; i++) { //this assumes even message size
    sum += *((uint16_t *)&message+i);
  }

  message.icmp_cksum = (uint16_t) ~(sum + (sum >> 16));

  if ( sendto(sock, &message, sizeof(message), 
        0, ai_addr, sizeof(*ai_addr)) < 0) {
    perror("sendto");
  } 
}
