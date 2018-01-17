
// creation of IP

int createIP(unsigned int ip)
{
  int i;
  unsigned char IPAddress[IP_OCTETS];
if(ip<0){
return FALSE;
}

  IPAddress[0] = (ip >> FIRST_OCTET) & COPY_MASK;
  IPAddress[1] = (ip >> SECOND_OCTET) & COPY_MASK;
  IPAddress[2] = (ip >> THIRD_OCTET) & COPY_MASK;
  IPAddress[3] = ip & COPY_MASK;

  for (i = 0; i < IP_OCTETS; i++)
  {
    printf("%u", IPAddress[i]);
    if (i != OCTET_PERIODS)
      printf(".");
    else
      printf(" ");
  }
return TRUE;
}
