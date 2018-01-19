#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

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
#define ARBITRARY_HASH 0x45D9F3B


struct metaInfo {
  unsigned int seconds, microseconds;
  unsigned short caplen, ignored;
};

struct packetInfo {
  struct metaInfo meta;
  unsigned char packet[MAX_ETH_PKT];
  struct ether_header *ethh;
  struct iphdr *ipheader;
  struct tcphdr *tcpheader;
  struct udphdr *udpheader;
  unsigned short ip_hdr_len;
  unsigned short tcp_hdr_len;
  unsigned int payload;
  double ts;
};

struct connectionNode{
  double start_ts, current_ts;
  unsigned char protocol;
  unsigned int endpoint_ips[ENDPOINT_MEMBERS];
  unsigned int endpoint_pkts[ENDPOINT_MEMBERS];
  unsigned int endpoint_bytes[ENDPOINT_MEMBERS];
  unsigned short endpoint_ports[ENDPOINT_MEMBERS];

  // Used only in calculating RTT of TCP packets
  unsigned int endpoint_seq[ENDPOINT_MEMBERS];
  char seqSet[ENDPOINT_MEMBERS];  // will equal 0 FALSE or 1 TRUE
  char rttCalculated[ENDPOINT_MEMBERS]; // will equal 0 FALSE or 1 TRUE
  double firstTimestamp[ENDPOINT_MEMBERS];
  double lastTimestamp[ENDPOINT_MEMBERS];


  struct connectionNode *next;
};

static FILE *traceFile= fopen("sample-A.dmp","rb");

static struct connectionNode *connectionTable[TABLE_SIZE];
void *zmalloc(unsigned int size);
int createIP(unsigned int ip);
struct connectionNode *lookup(struct packetInfo packet);
void setUpTable();
int addPacket(struct packetInfo packet);
void insertNode(struct packetInfo packet);
int checkAck(struct connectionNode *nodepointer, struct packetInfo packet);
int updateSeq(struct connectionNode *nodepointer, struct packetInfo packet);
void calculateRTT(struct connectionNode *nodepointer, struct packetInfo packet);
void updateNode(struct connectionNode *nodepointer, struct packetInfo packet);
int transportPacketDumping();
int printConnectionSummaries();
int calculateRoundTripTimes();
int safeFRead(void *ptr, size_t size, size_t nobj, FILE *stream);
int printConnection(struct connectionNode *connection);
int printRTT(struct connectionNode * connection);
unsigned short next_packet(struct packetInfo *pkts);
int next_usable_packet(struct packetInfo *pkts);
int next_usable_packet_tcp(struct packetInfo *pkts);
int populateConnectionTable();
int populateTCPTable();
int depopulateTable();
