#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"

void displist(pln *curr)
{
  //printf ("(");
  while (curr != NULL)
  {
    //printf("%d ",curr->plnNum);
    curr=curr->nextPln;
  }
  //printf(")\n");
}
