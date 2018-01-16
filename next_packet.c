

//next packet

unsigned short next_packet(struct packetInfo *pkts)
{
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
