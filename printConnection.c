

// Print connection

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


