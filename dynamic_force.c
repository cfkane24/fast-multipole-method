#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"
#include "intVec.h"
#include "displist.h"
#include "getArrayIndex.h"
#include "dynamic_region.h"
#include "dynamic_wellSepCom.h"
#include "timing.h"

extern double G;
extern int collision_pair_1[];
extern int collision_pair_2[];
extern int collision_number;
extern int collision_check;
extern int LVL;

void dynamic_forceMagic(dynamic_region regionToTest, planet &myplanet, planet *BD, double alpha){

  int i;
  double k;
  double rad;
  pln *curr_pln;
  planet curr_planet;

  //printf("!region is on level %d with %d planets\n", regionToTest.level, regionToTest.numPln);
  if(regionToTest.level == (LVL-1) || regionToTest.numPln == 1){
    curr_pln = regionToTest.planets;
    while(curr_pln != NULL){//loop through planet list
      curr_planet = BD[curr_pln->plnNum];
      rad = radius(myplanet.pos, curr_planet.pos);
      
      if (rad > 0){ //very crude check to make sure it's not the same planet pulling on itself
	if( collision_check ){
	  if( rad < 0.8*(myplanet.r + curr_planet.r) && (myplanet.num > curr_planet.num )){//this actually decides if things hit.
	    collision_pair_1[collision_number] = myplanet.num;
	    collision_pair_2[collision_number] = curr_planet.num;
	    collision_number++;
	  }
	}
	myplanet.acc = myplanet.acc + (curr_planet.pos - myplanet.pos) * G * curr_planet.m / (rad*rad*rad);
      }
      curr_pln = curr_pln->nextPln;
    }
  }

  else{
    k = wellSepCom(myplanet, regionToTest);
    if(k>(alpha*alpha)){
      rad = radius(myplanet.pos, regionToTest.com);
      myplanet.acc = myplanet.acc + regionToTest.mass * (regionToTest.com-myplanet.pos) * G / (rad*rad*rad);
    }

    else{
      for(i=0; i<8; i++){
	if(regionToTest.child[i]->planets != NULL){
	  dynamic_forceMagic(*regionToTest.child[i], myplanet, BD, alpha);
	}
	
	/*if(regionToTest.child[i]->numPln == 0){//child is empty
	  continue;
	}
	else{
	  dynamic_forceMagic(*regionToTest.child[i], myplanet, BD, alpha);
	}
	*/
      }
    }
  }
}
