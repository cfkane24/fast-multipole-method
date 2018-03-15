#include <math.h>
#include <stdio.h>
#include "vector.h"
#include "pln.h"
#include "region.h"
#include "getArrayIndex.h"

void findKids(int maxLvl)
{
  int lvl, count;
  int pside, cside;
  int iP, jP, kP, iC, jC, kC;
  int parentBox, parentIndex, childBox, childIndex;//b for "boxes per side", _P for Parent, _C for Child, boxOfLvl for the box number at a given level, ind for index in the large array
  
  for(lvl=0; lvl<maxLvl; lvl++)
  {
    pside=pow(2,lvl);//number of boxes a side
    //printf("!TREE: Start level %d\n",lvl);
    for(iP=0; iP<pside; iP++)//Loop over parent boxes in the i direction
    {
      for(jP=0; jP<pside; jP++)//j direction
      {
	for(kP=0; kP<pside; kP++)//k direction
	{
	  parentBox = pside*pside*iP + pside*jP + kP;
	  parentIndex = getArrayIndex(lvl, parentBox);
	  regions[parentIndex].level = lvl;
	  regions[parentIndex].i = iP;
	  regions[parentIndex].j = jP;
	  regions[parentIndex].k = kP;
	  //printf("!TREE: Region %d:%d,%d,%d/%d: \n",lvl,iP,jP,kP,parentIndex);
          count=0;
	  for(iC=iP*2; iC<=iP*2+1; iC++)//Loop over child boxes in i direction
	  {
	    for(jC=jP*2; jC<=jP*2+1; jC++)//j direction
	    {
	      for(kC=kP*2; kC<=kP*2+1; kC++)//k direction
	      {
    		cside=pow(2,lvl+1); // figure out how many boxes are in each row in the child level
		childIndex = getArrayIndex(lvl+1,cside*cside*iC + cside*jC + kC);
		
		regions[parentIndex].kids[count] = childIndex;
		//printf("!TREE:   Child %d is %d:%d,%d,%d/%d\n",count,lvl+1,iC,jC,kC,childIndex);
		count++;
	      }
	    }
	  }
	}
      }
    }
  }
}
/*
   int main()
   {
   findKids(2);


   }*/
