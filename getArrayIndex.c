#include <math.h>
#include <stdio.h>

int getArrayIndex( int level, int box ){
 
  //this will take the planet box at a level
  //and return where to put it in the region = array
   
  int i;
  int TOT = 0;
  for( i=0; i<level; i++ ){//because only want the boxes in previous levels
    TOT += pow(8,i);
  }
  return TOT + box;
}
