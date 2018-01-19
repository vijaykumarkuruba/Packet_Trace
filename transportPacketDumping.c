
//Transport Packet dumping


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"

#define ALLOWED_OPTIONS 2
#define TRUE 1
#define FALSE 0
#define NO_BYTES 0
#define SINGLE_OBJECT 1
#define MICROSECONDS_PER_SECOND 1000000
#define SINGLE_BYTE 1
#define IP_WORD_SIZE 4
#define TCP_WORD_SIZE 4
#define IP_OCTETS 4
#define OCTET_PERIODS 3
#define FIRST_OCTET 24
#define SECOND_OCTET 16
#define THIRD_OCTET 8
#define COPY_MASK 0xFF
#define ORIGINATOR 0
#define RESPONDER 1
#define ENDPOINT_MEMBERS 2

#define MAX_ETH_PKT 1518
#define ETHERNET_HEADER_LENGTH 14
#define IP_HEADER_LENGTH 20
#define META_INFO_LENGTH 12
#define UDP_HEADER_LENGTH 8
#define TCP_HEADER_LENGTH 20
#define BOTH_ADDRESSES 12
#define DECIMAL_IP 2048
#define DECIMAL_TCP 6
#define DECIMAL_UDP 17
#define CONNECTION_START_TS 0

#define TABLE_SIZE 47717
#define ARBITRARY_SHIFT 16

printf("\n transportPacketDumping function \n");
int transportPacketDumping()
{
  struct packetInfo packet;

  while (next_usable_packet(&packet))
  {
    printf("%0.6f ", packet.ts);
    if (packet.tcpheader != NULL)
    {
      createIP(ntohl(packet.ipheader->saddr));
      printf("%u ", ntohs(packet.tcpheader->th_sport));
      createIP(ntohl(packet.ipheader->daddr));
      printf("%u ", ntohs(packet.tcpheader->th_dport));

      printf("T %u %u %u\n", packet.payload, ntohl(packet.tcpheader->th_seq), ntohl(packet.tcpheader->th_ack));
    }

    if (packet.udpheader != NULL)
    {
      createIP(ntohl(packet.ipheader->saddr));
      printf("%u ", ntohs(packet.udpheader->uh_sport));
      createIP(ntohl(packet.ipheader->daddr));
      printf("%u ", ntohs(packet.udpheader->uh_dport));
      printf("U %u\n", packet.payload);
    }
  }

  return TRUE;
}
