
#include <stdio.h>
#include <stdlib.h>

static void error(char * msg)
{
  fprintf(stderr,"LSD Error: %s\n",msg);
  exit(EXIT_FAILURE);
}

