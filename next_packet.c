

//next packet

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

printf("\n next packet \n");
unsigned short next_packet(struct packetInfo *pkts)
{
    printf("\n Executed Next Packet function \n");
    memset(pkts, 0x0, sizeof(struct packetInfo));  
    safeFRead(&pkts->meta, sizeof(struct metaInfo), SINGLE_OBJECT, traceFile);
    pkts->meta.seconds = ntohl(pkts->meta.seconds);
    pkts->meta.microseconds = ntohl(pkts->meta.microseconds);
    pkts->meta.caplen = ntohs(pkts->meta.caplen);
    pkts->ts = pkts->meta.seconds + ((double)pkts->meta.microseconds / MICROSECONDS_PER_SECOND);

  if (pkts->meta.caplen == 0)
    return FALSE;
  if (pkts->meta.caplen > MAX_ETH_PKT){
    perror("caplen > maximum ethernet frame. exiting\n");
    exit(EXIT_FAILURE);
  }

  if (safeFRead(pkts->packet, pkts->meta.caplen, SINGLE_OBJECT, traceFile) == 0)
    return TRUE;

  if(pkts->meta.caplen < sizeof(struct ether_header))
    return TRUE;

  pkts->ethh = (struct ether_header *)pkts->packet;
  pkts->ethh->ether_type = ntohs(pkts->ethh->ether_type);

  if(pkts->meta.caplen == sizeof(struct ether_header))
    return TRUE;

  if(pkts->ethh->ether_type != DECIMAL_IP)
    return TRUE;

  pkts->ipheader = (struct iphdr *)(pkts->packet + sizeof(struct ether_header));
  pkts->ip_hdr_len = pkts->ipheader->ihl * IP_WORD_SIZE;

  if(pkts->meta.caplen < sizeof(struct ether_header) + pkts->ip_hdr_len)
  {
    pkts->ipheader = NULL;
    return TRUE;
  }

  pkts->ipheader->tot_len = ntohs(pkts->ipheader->tot_len);

  if (ntohs(pkts->ipheader->protocol == DECIMAL_UDP))
  {
    pkts->udpheader = (struct udphdr *)(pkts->packet + sizeof(struct ether_header) + pkts->ip_hdr_len);
    pkts->tcp_hdr_len = 0;
    if (pkts->meta.caplen < (sizeof(struct ether_header) + sizeof(struct iphdr) + UDP_HEADER_LENGTH))
    {
      pkts->udpheader = NULL;
      return TRUE;
    }
    pkts->payload = ntohs(pkts->udpheader->uh_ulen) - UDP_HEADER_LENGTH;

  }

  if (ntohs(pkts->ipheader->protocol == DECIMAL_TCP))
  {
    pkts->tcpheader = (struct tcphdr *)(pkts->packet + sizeof(struct ether_header) + pkts->ip_hdr_len);

    if (pkts->meta.caplen < (sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct tcphdr)))
    {
      pkts->tcpheader = NULL;
      return TRUE;
    }

    pkts->tcp_hdr_len = pkts->tcpheader->th_off * TCP_WORD_SIZE;

    if (pkts->meta.caplen < (sizeof(struct ether_header) + sizeof(struct iphdr) + pkts->tcp_hdr_len))
    {
      pkts->tcpheader = NULL;
      return TRUE;
    }
    pkts->payload = pkts->ipheader->tot_len - pkts->ip_hdr_len - pkts->tcp_hdr_len;

  }

  return TRUE;
}





int next_usable_packet(struct packetInfo *pkts)
{
  printf("\n Executed Next Usuable Packet function \n");
  while (next_packet(pkts))
  {
    if (pkts->ethh == NULL)
      continue;

    if (pkts->ethh->ether_type != DECIMAL_IP)
      continue;

    if (pkts->ipheader == NULL)
      continue;

    if (pkts->ipheader->protocol != DECIMAL_UDP && pkts->ipheader->protocol != DECIMAL_TCP)
      continue;

    if (pkts->tcpheader == NULL && pkts->udpheader == NULL)
      continue;

    return TRUE;
  }
  return FALSE;
}
