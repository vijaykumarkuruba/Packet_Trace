// depopulate table


int depopulateTable()
{
  int i;
  struct connectionNode *clearpointer, *nodepointer;

  for (i = 0; i < TABLE_SIZE; i++)
  {
    if (connectionTable[i] != NULL)
    {
      nodepointer = connectionTable[i];
      while (nodepointer->next != NULL)
      {
        clearpointer = nodepointer;
        nodepointer = nodepointer->next;
        free(clearpointer);
      }
      free(nodepointer);
    }
  }
  return TRUE;
}
