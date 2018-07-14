#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "pln.h"
#include "vector.h"
#include "planet.h"
#include "region.h"
#include "popRegions.h"

extern int LVL;

void resetOctree(region& parent_region){

  int i;
  //printf("Parent is on level %d\n", parent_region.level);
  if( parent_region.planets != NULL ) clearList(&parent_region.planets);
    
  //if(parent_region.child[0] != NULL && parent_region.level != LVL-1){
  for(i=0; i<8; i++){
    if(parent_region.child[i]!=NULL)
    { // it has kids to erase
      resetOctree(*parent_region.child[i]);
      free(parent_region.child[i]);
      parent_region.child[i]=NULL;
    }
  }
}

