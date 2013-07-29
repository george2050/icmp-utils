#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

/*
 *  USAGE: ping IP_ADDRESS
 * 
*/

void ping(struct sockaddr*, int, uint16_t);
struct sockaddr_in pong(int);

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

  uint16_t seq = 0;
  struct timeval beg;
  struct timeval end;
  struct sockaddr_in msg;

  for (; seq < 10; seq++ ) {
    gettimeofday(&beg, NULL);
    ping(ai->ai_addr, sock, seq);
    msg = pong(sock);
    char hostname[128];
    gettimeofday(&end, NULL);
    printf("time=%f ms\n",
        (end.tv_sec-beg.tv_sec)*1000.0 + (end.tv_usec-beg.tv_usec)/1000.0);
    getnameinfo((struct sockaddr *)&msg, sizeof(msg), 
        hostname, sizeof(hostname), NULL, 0, 0); 
    printf("from=%s\n", hostname );
  }
}


void ping (struct sockaddr* ai_addr, int sock, uint16_t seq) {
  struct icmp message;
  message.icmp_type = ICMP_ECHO;        // 8
  message.icmp_code = ICMP_ECHOREPLY;  // 0
  message.icmp_cksum = 0;
  message.icmp_hun.ih_idseq.icd_id = htons(1337); // we r so leet
  message.icmp_hun.ih_idseq.icd_seq = htons(seq);
  memset(&message.icmp_dun, 0, sizeof(message.icmp_dun));

  // Calculate message checksum, which is a ones-complement sum
  // of each word in the 16-bit value, with overflow stored in a 32-bit
  // unsigned int and then the "overflow" added back to the lower 16 bits
  // Fun! (thanks Martin Törnwall @mtornwall)

  uint32_t sum = 0;
  int i;
  for (i = 0; i < sizeof(message)/2; i++) { //this assumes even message size
    sum += *((uint16_t *)&message+i);
  }

  // Finish calculating and set message checksum
  message.icmp_cksum = (uint16_t) ~(sum + (sum >> 16));

  if ( sendto(sock, &message, sizeof(message), 
        0, ai_addr, sizeof(*ai_addr)) < 0) {
    perror("sendto");
  } 
}


struct sockaddr_in pong(int sock){

  struct icmp message;
  struct sockaddr_storage sendr; //like tumblr, fuck you
  int fromlen = sizeof(sendr);
  if (recvfrom(sock, &message, sizeof(message),
        0, (struct sockaddr*) &sendr, &fromlen ) < 0) {
    perror("recvfrom");
    exit(1);
  } else {
    return *((struct sockaddr_in *)&sendr);
  }
}
