#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pln.h"
#include "vector.h"
#include "planet.h"
#include "region.h"
#include "displist.h"

extern int N, LVL, numDel;
extern region *regions;//FIXME Include region.h?

void add(int num, pln **list)
{
  pln *newPln;//"Make a pointer to a pln, call it 'newPln'"
  newPln=(pln *)malloc(sizeof(pln));//"Allocate a pln's worth of memory to the space pointed to by 'newPln'"
  newPln->plnNum = num;//Follow 'newPln' to the pln it points to, set plnNum of that pln to the value num
  newPln->nextPln = *list;//Follow 'newPln' to the pln it points to, set next of that pln to the pointer to the beginning of the old list
  *list = newPln;//Set this pointer to the pointer 'newPln'
}

void clearList(pln **list)
{
  //printf("!PLF: in clearLists \n");
  pln *curr = *list;
  pln *next;
  //printf("!PLF: Start while loop\n");
  int n=0;
  while(curr!=NULL)
  {
    next = curr->nextPln;
    //		printf("curr: %p  next: %p\n", curr, next);
    free(curr);
    curr = next;
    n++;
    numDel++;
    //		printf("!PLF: # of elements cleared from this list: %d\n", n);
  }
  *list = NULL;
}

void popLists(planet BD[])
{

  int i,j;
  for(i=0; i<N; i++)
  {
    for(j=0; j<LVL; j++)
    {
//      printf("! Planet %d marked as a member of region %d:%d ",i,j,BD[i].level[j]);
      add(i, &(regions[BD[i].level[j]].planets));
//      displist(regions[BD[i].level[j]].planets);
    }	
  }
}
