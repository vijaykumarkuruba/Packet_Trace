
// Packet_trace.c
// Code for the Packet_trace

// This program relies on a single, static hash table for parts -s and
// -t. It has a large starting size of almost 50k, but it means that the load
// favot for the hash table is small, keeping the time complexity of lookup
// and insert at about O(1).


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/if_ether.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip.h>

#include "struct.h"


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
#define ARBITRARY_HASH 0x45D9F3B

//static FILE *traceFile;
static char *traceFilePath;


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
int case_ip,function_ip;
char* function_ip1;



#ifndef PACKET
int main(int argc,char *argv[])
{
	
        for(int i=1;i<argc;i++)
	    {
		
		if (i==1)
		{
			case_ip=atoi(argv[i]);
		}
		else
		{
			if (case_ip!=1)
				function_ip=atoi(argv[i]);
			else
				function_ip1=argv[i];
		}
	}
	if (case_ip!=1)
		printf("%d\t%d\n",case_ip,function_ip);
	else
		printf("%d\t%s\n",case_ip,function_ip1);

     int k=1;
    
     int optionCount = 0;
     int option;
     short rflag, pflag, sflag, tflag;
     rflag = FALSE;
     pflag = FALSE;
     sflag = FALSE;
     tflag = FALSE;

    switch(case_ip)
      {
        case 5:
	           int ip;
	           createIP(function_ip);
               printf("\n");
               break;

		case 2 :{
		       struct packetInfo pkts;
		       memset(&pkts,125,sizeof(struct packetInfo));
		       next_packet(&pkts);
		       printf("\n");
		       break;
			}
		case 3 :{
		      struct packetInfo pkts;
		      memset(&pkts,25,sizeof(struct packetInfo));
		      next_usable_packet(&pkts);
		      break;
		      }
		case 4:{
		     double start_ts=123.255, current_ts=0;
		     unsigned char protocol=127;
		     struct connectionNode connection;
		     memset(&connection,25,sizeof(struct connectionNode));
		     printRTT(&connection);
		     printf("\n");
		     break;
		     }
			 
         case 6 :
		     k=2;
             break;
         default :
             printf("\nPlease Enter the correct option... \n");
             break;
         case 1 :
		
			  while ((option = getopt(argc, argv, "r:pst ")) != -1)
			  {
		        
				switch (option)
				{
				  case 'r':
					traceFilePath = optarg;

					if (traceFilePath == NULL)
					{
						printf("%s", "Missing file argument.\n");
						exit(EXIT_FAILURE);
					}
					else
					{
					  rflag = TRUE;
					  optionCount++;
					  break;
					}

				  case 'p':
					pflag = TRUE;
					optionCount++;
					break;

				  case 's':
					sflag = TRUE;
					optionCount++;
					break;

				  case 't':
					tflag = TRUE;
					optionCount++;
					break;

				  case ' ':
					perror("No mode option input, exiting");
					break;

				  default:
					perror("Invalid command line argument");
					exit(EXIT_FAILURE);
				}
			  }

			  if (optionCount > ALLOWED_OPTIONS)
			  {
				perror("Only 1 command option is allowed");
				exit(EXIT_FAILURE);
			  }

			  if (optionCount < ALLOWED_OPTIONS)
			  {
				perror("Insufficient number of mode options");
				exit(EXIT_FAILURE);
			  }

			  if (rflag)
			  { 
				if ((traceFile = fopen(traceFilePath, "rb")) == NULL)
				{ 
				  perror("File open failed");
				  exit(EXIT_FAILURE);
				}
			  }
			  else
			  {
				perror("No file argument. Exiting");
				exit(EXIT_FAILURE);
			  }

			  if (pflag)
				transportPacketDumping();

			  if (sflag)
				printConnectionSummaries();

			  if (tflag)
				calculateRoundTripTimes(); 
                          break; 
                     
      }
  

return TRUE;

}
#endif


//static FILE *traceFile;
/* Ensures bytes are read safely from the trace */
int safeFRead(void *ptr, size_t size, size_t nobj, FILE *stream)
{ 

  unsigned int readSuccess;
  
  printf("\n Executed SafeRead  function \n");
  if ((readSuccess = fread(ptr, size, nobj, stream)) < 0)
  { 
    perror("Reading failed."); 
    exit(EXIT_FAILURE);
  }
  if (readSuccess == NO_BYTES)
    return NO_BYTES;
  else
    return readSuccess;
}


/* Allocates and clear memory of a parameterized size */
void *zmalloc(unsigned int size)
{
  void *p;

  if ((p = (void *)malloc(size)) == NULL)
  {
    perror("Memory allocation failed, exiting program");
    exit(EXIT_FAILURE);
  }

  memset(p, 0x0, size);
  return p;
}






int next_usable_packet_tcp(struct packetInfo *pkts)
{
  
  while (next_packet(pkts))
  {
    if (pkts->ethh == NULL)
      continue;

    if (pkts->ethh->ether_type != DECIMAL_IP)
      continue;

    if (pkts->ipheader == NULL)
      continue;

    if (pkts->ipheader->protocol != DECIMAL_TCP)
      continue;

    if (pkts->tcpheader == NULL)
      continue;

    return TRUE;
  }
  return FALSE;
}


int printConnectionSummaries()
{
  struct connectionNode *printPointer;
  int i;
  populateConnectionTable();

  for (i = 0; i < TABLE_SIZE; i++)
    if (connectionTable[i] != NULL)
    {
      printPointer = connectionTable[i];
      while (printPointer != NULL)
      {
        printConnection(printPointer);
        printPointer = printPointer->next;
      }
    }
  depopulateTable();

  return TRUE;
}

int populateConnectionTable()
{
  struct packetInfo packet;

  while (next_usable_packet(&packet))
    addPacket(packet);

  return TRUE;
}

int populateTCPTable()
{
  struct packetInfo packet;
  while (next_usable_packet_tcp(&packet))
    addPacket(packet);

  return TRUE;
}





int printRTT(struct connectionNode * connection)
{
  createIP(connection->endpoint_ips[ORIGINATOR]);
  printf("%u ", connection->endpoint_ports[ORIGINATOR]);
  createIP(connection->endpoint_ips[RESPONDER]);
  printf("%u ", connection->endpoint_ports[RESPONDER]);

  if (connection->rttCalculated[ORIGINATOR])
    printf("%0.6f ", connection->lastTimestamp[ORIGINATOR] - connection->firstTimestamp[ORIGINATOR]);
  else if (connection->seqSet[ORIGINATOR])
    printf("? ");
  else
    printf("- ");

  if (connection->rttCalculated[RESPONDER])
    printf("%0.6f", connection->lastTimestamp[RESPONDER] - connection->firstTimestamp[RESPONDER]);
  else if (connection->seqSet[RESPONDER])
    printf("?");
  else
    printf("-");

  printf("\n");

return TRUE;
}

int calculateRoundTripTimes()
{
  int i;
  struct connectionNode *printPointer;

  populateTCPTable();

  for (i = 0; i < TABLE_SIZE; i++)
    if (connectionTable[i] != NULL)
    {
      printPointer = connectionTable[i];
      while (printPointer != NULL)
      {
        printRTT(printPointer);
        printPointer = printPointer->next;
      }
    }
  depopulateTable();

  return TRUE;
}

// ############    HASHTABLE OPERATIONS     ############

unsigned int hash(struct packetInfo packet)
{
  unsigned int key, addressSum;

  addressSum = ntohl(packet.ipheader->daddr) + ntohl(packet.ipheader->saddr) + packet.ipheader->protocol;

  if (packet.ipheader->protocol == DECIMAL_UDP)
    addressSum += (ntohs(packet.udpheader->uh_sport) + ntohs(packet.udpheader->uh_dport));
  else
    addressSum += (ntohs(packet.tcpheader->th_sport) + ntohs(packet.tcpheader->th_dport));

  key = ((addressSum >> ARBITRARY_SHIFT) ^ addressSum) * ARBITRARY_HASH;
  return key % TABLE_SIZE;
}

struct connectionNode *lookup(struct packetInfo packet)
{
  struct connectionNode *nodepointer;

  for (nodepointer = connectionTable[hash(packet)]; nodepointer != NULL; nodepointer = nodepointer->next)
  {
    if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[ORIGINATOR])
    {
      if (ntohl(packet.ipheader->daddr) == nodepointer->endpoint_ips[RESPONDER])
        if (packet.ipheader->protocol == nodepointer->protocol)
        {
          switch (nodepointer->protocol)
          {
            case DECIMAL_UDP:
              if (ntohs(packet.udpheader->uh_sport) == nodepointer->endpoint_ports[ORIGINATOR])
                if (ntohs(packet.udpheader->uh_dport) == nodepointer->endpoint_ports[RESPONDER])
                  return nodepointer;
              break;
            case DECIMAL_TCP:
              if (ntohs(packet.tcpheader->th_sport) == nodepointer->endpoint_ports[ORIGINATOR])
                if (ntohs(packet.tcpheader->th_dport) == nodepointer->endpoint_ports[RESPONDER])
                  return nodepointer;
              break;
          }
        }
      }
    else if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[RESPONDER])
    {
      if (ntohl(packet.ipheader->daddr) == nodepointer->endpoint_ips[ORIGINATOR])
        if (packet.ipheader->protocol == nodepointer->protocol)
        {
          switch (nodepointer->protocol)
          {
            case DECIMAL_UDP:
              if (ntohs(packet.udpheader->uh_sport) == nodepointer->endpoint_ports[RESPONDER])
                if (ntohs(packet.udpheader->uh_dport) == nodepointer->endpoint_ports[ORIGINATOR])
                  return nodepointer;
              break;
            case DECIMAL_TCP:
              if (ntohs(packet.tcpheader->th_sport) == nodepointer->endpoint_ports[RESPONDER])
                if (ntohs(packet.tcpheader->th_dport) == nodepointer->endpoint_ports[ORIGINATOR])
                  return nodepointer;
              break;
          }
        }
    }
  }
  return NULL;
}

struct connectionNode *createNode(struct packetInfo packet)
{
  struct connectionNode *newNode = (struct connectionNode *)zmalloc(sizeof(struct connectionNode));

  newNode->start_ts = packet.ts;
  newNode->current_ts = newNode->start_ts;
  newNode->protocol = packet.ipheader->protocol;

  newNode->endpoint_ips[ORIGINATOR] = ntohl(packet.ipheader->saddr);
  newNode->endpoint_ips[RESPONDER] = ntohl(packet.ipheader->daddr);

  if (newNode->protocol == DECIMAL_TCP)
  {
    newNode->endpoint_ports[ORIGINATOR] = ntohs(packet.tcpheader->th_sport);
    newNode->endpoint_ports[RESPONDER] = ntohs(packet.tcpheader->th_dport);

    // creating new node for connection with payload (ALLMAN)

    if (packet.payload != 0)
    {
      newNode->endpoint_seq[ORIGINATOR] = ntohl(packet.tcpheader->th_seq);
      newNode->firstTimestamp[ORIGINATOR] = packet.ts;
    }
  }
  else
  {
    newNode->endpoint_ports[ORIGINATOR] = ntohs(packet.udpheader->uh_sport);
    newNode->endpoint_ports[RESPONDER] = ntohs(packet.udpheader->uh_dport);
  }

  newNode->endpoint_pkts[ORIGINATOR] = 1;
  newNode->endpoint_bytes[ORIGINATOR] = packet.payload;
  newNode->next = NULL;

  return newNode;
}

int addPacket(struct packetInfo packet)
{
  struct connectionNode *lookpointer;
  lookpointer = lookup(packet);

  if (lookpointer == NULL)
    insertNode(packet);
  else
    updateNode(lookpointer, packet);
return TRUE;
}

void insertNode(struct packetInfo packet)
{
  if (connectionTable[hash(packet)] == NULL)
    connectionTable[hash(packet)] = createNode(packet);
  else
  {
    struct connectionNode *nodepointer;
    nodepointer = connectionTable[hash(packet)];

    while (nodepointer->next != NULL)
      nodepointer = nodepointer->next;
    nodepointer->next = createNode(packet);
  }

}

void updateNode(struct connectionNode *nodepointer, struct packetInfo packet)
{
  if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[ORIGINATOR])
  {
    nodepointer->endpoint_pkts[ORIGINATOR]++;
    nodepointer->endpoint_bytes[ORIGINATOR] += packet.payload;
  }
  else
  {
    nodepointer->endpoint_pkts[RESPONDER]++;
    nodepointer->endpoint_bytes[RESPONDER] += packet.payload;
  }

  if (packet.ipheader->protocol == DECIMAL_TCP)
  {
    updateSeq(nodepointer, packet);
    if (checkAck(nodepointer, packet))
    {
      calculateRTT(nodepointer, packet);
    }

  }
  nodepointer->current_ts = packet.ts;
}

int updateSeq(struct connectionNode *nodepointer, struct packetInfo packet)
{
  if (packet.payload > 0)
  {
    if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[ORIGINATOR])
    {
      if (!(nodepointer->seqSet[ORIGINATOR]))
      {
        nodepointer->firstTimestamp[ORIGINATOR] = packet.ts;
        nodepointer->endpoint_seq[ORIGINATOR] = ntohl(packet.tcpheader->th_seq);
        nodepointer->seqSet[ORIGINATOR] = TRUE;
        return TRUE;
      }
    }
    else if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[RESPONDER])
    {
      if (!(nodepointer->seqSet[RESPONDER]))
      {
        nodepointer->firstTimestamp[RESPONDER] = packet.ts;
        nodepointer->endpoint_seq[RESPONDER] = ntohl(packet.tcpheader->th_seq);
        nodepointer->seqSet[RESPONDER] = TRUE;
        return TRUE;
      }
    }
  }
  return FALSE;
}

int checkAck(struct connectionNode *nodepointer, struct packetInfo packet)
{
  if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[RESPONDER] &&
  ntohl(packet.ipheader->daddr) == nodepointer->endpoint_ips[ORIGINATOR])
  {
    if (nodepointer->seqSet[ORIGINATOR])
      if (ntohl(packet.tcpheader->th_ack) > nodepointer->endpoint_seq[ORIGINATOR])
        return TRUE;
  }
  else if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[ORIGINATOR] &&
  ntohl(packet.ipheader->daddr) == nodepointer->endpoint_ips[RESPONDER])
  {
    if (nodepointer->seqSet[RESPONDER])
      if (ntohl(packet.tcpheader->th_ack) > nodepointer->endpoint_seq[RESPONDER])
        return TRUE;
  }

  return FALSE;
}


void calculateRTT(struct connectionNode *nodepointer, struct packetInfo packet)
{
  if (!(nodepointer->rttCalculated[ORIGINATOR]))
  {
    // verifies that the packet is from the responder endpoint
    // if (nodepointer->endpoint_ips[ORIGINATOR] == ntohl(packet.ipheader->daddr))
    if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[RESPONDER])
    {
      nodepointer->rttCalculated[ORIGINATOR] = TRUE;
      nodepointer->lastTimestamp[ORIGINATOR] = packet.ts;
    }
  }
  else if (!(nodepointer->rttCalculated[RESPONDER]))
  {
    // if (nodepointer->endpoint_ips[RESPONDER] == ntohl(packet.ipheader->saddr))
    if (ntohl(packet.ipheader->saddr) == nodepointer->endpoint_ips[ORIGINATOR])
    {
      nodepointer->rttCalculated[RESPONDER] = TRUE;
      nodepointer->lastTimestamp[RESPONDER] = packet.ts;
    }
  }
}



