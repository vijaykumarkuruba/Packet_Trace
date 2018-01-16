

//Transport Packet dumping

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
