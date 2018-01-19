

// Print connection


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

int printConnection(struct connectionNode *connection)
{
  if (connection != NULL)
  {
    printf("%0.6f %0.6f ", connection->start_ts, (connection->current_ts - connection->start_ts));
    createIP(connection->endpoint_ips[ORIGINATOR]);
    printf("%u ", connection->endpoint_ports[ORIGINATOR]);
    createIP(connection->endpoint_ips[RESPONDER]);
    printf("%u ", connection->endpoint_ports[RESPONDER]);

    if (connection->protocol == DECIMAL_UDP)
      printf("U ");
    else
      printf("T ");

    printf("%u %u ", connection->endpoint_pkts[ORIGINATOR], connection->endpoint_bytes[ORIGINATOR]);

    if (connection->endpoint_pkts[RESPONDER] != NO_BYTES)
    {
      printf("%u %u\n", connection->endpoint_pkts[RESPONDER], connection->endpoint_bytes[RESPONDER]);
      return TRUE;
    }
    else
    {
      printf("? ?\n");
      return TRUE;
    }
  }
  return FALSE;
}


