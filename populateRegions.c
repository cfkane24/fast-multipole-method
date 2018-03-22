#include <stdio.h>
#include <math.h>
#include "getArrayIndex.h"
#include "vector.h"
#include "pln.h"
#include "region.h"
#include "planet.h"

extern int LVL, N;
extern double L;

void clamp(int &a, int &b)
{
   if (a<0) a=0;
   if (a>=b) a=b-1;
}

void recurse_divide_by_mass(region &r){
  int i;
  
  if( r.mass == 0 ) return;//same thing as saying its empty
  //this is after regions have been populated.
  //need to do it somewhere else
  
  r.com = r.com / r.mass;
  if( r.level != LVL-1 ){
    for(i=0; i<8; i++){
      recurse_divide_by_mass(regions[r.kids[i]]);
    }
  }
}

int findRegionNumber(int index, int index_array[], int &index_array_counter){

  int i;
  int the_final_index;

  if(index_array_counter == 0){
    the_final_index = 0;
  }

  else{//loop through and see if it is new or old
    for(i=0; i<index_array_counter; i++){
      if(index == index_array[i]){
	break;//found an index that matches current one
      }
    }
    if(i==index_array_counter){//this is a new index
      index_array_counter++;
      the_final_index = index_array_counter;
    }
    else{
      the_final_index = i;
    }
  }
  return the_final_index;
}

//===============================
void populateRegions(planet BD[], int totalIndeces){
  
  int i,j;//here i will represent the level and j is planet number
  int a,b,c;//a b c are how many rows, columns, or slabs something is
  int n,k;
  int z,sum;
  int plnt_num;
  int index;
  int index_array[1000];
  int index_array_counter;
  double plnt_mass;
  vector plnt_COM;
  
  //sum now declared globally, called totalIndeces
  
  
  for( i=0; i<LVL; i++ ){
    //printf("!POPREG: working on level %d\n",i);
    index_array_counter=0;
    for( j=0; j<1000; j++ ){//neg means no index there
      index_array[j] = -1;
    }
    n = pow(2,i);//n is number of boxes per side		
    for( j=0; j<N; j++ ){
      
      a = BD[j].pos.x*n/L;
      b = BD[j].pos.y*n/L;
      c = BD[j].pos.z*n/L;
      clamp(a,n);
      clamp(b,n);
      clamp(c,n);
      z = a + b*n + c*n*n;
      index = getArrayIndex(i,z);
      BD[j].level[i] = findRegionNumber(index, index_array, index_array_counter);
      //so essentially, on level i, I know I am in the BD[j].level[i]th region
      //in the linked list. Start counting at zero.
      
      //BD[j].level[i] = index;//planet is in box 'index' on level 'i'
      //have a counter that counts which region we are looking at?
      //loop through this counter?
      //or match an index with 
      if(regions[index].numPln == 0){	
	regions[index].mass  = 0;
	regions[index].com.x = 0;
	regions[index].com.y = 0;
	regions[index].com.z = 0;
      }//set them to zero so you can add to itself
       //resets the value. seeing box for first time
      
      regions[index].mass = regions[index].mass + BD[j].m;
      regions[index].com  = regions[index].com + BD[j].m * BD[j].pos;
      regions[index].numPln++;
      
    }
    /*
      check here to see if the children have planets in them
      if(yes) make the children of this region
      if(no) do not make data structure for the children of this region
     */

    /*
      or make all children before hand
      if(yes) keep
      if(no) delete this from the list
    */
  }
  recurse_divide_by_mass(regions[0]);
}
