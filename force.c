#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"
#include "intVec.h"
#include "wellSepCoM.h"
#include "displist.h"
#include "getArrayIndex.h"
#include "timing.h"

extern double G;
extern int collision_pair_1[];
extern int collision_pair_2[];
extern int collision_number;
extern int collision_check;
extern int LVL;
/**********************************************
  Finds net force on planet "myplanet" from region "regionToTest"
 **********************************************/

void forceMagic(region regionToTest, planet &myplanet, planet *BD, double alpha){
  //assuming implementation in recover.c, N is number of planets
  int i,j,a;
  double k;
  double rad;
  vector F;
  int n;
  int regionBox;
  intVec my, other;
  pln *currPln;
  vector newCOM;
  double newMass;
  planet current;
  int lev = regionToTest.level;
  
  //printf("%d",regionToTest.level);
  if(regionToTest.level == (LVL-1) || (regionToTest.numPln==1)){
    
    currPln = regionToTest.planets;
    while (currPln != NULL){
      //space(lev); //printf("! Remaining list of planets in this region: "); displist(currPln);
      current = BD[currPln->plnNum];
      rad     = radius(myplanet.pos, current.pos);//can this be replaced by looking at .num?
   
      if (rad > 0){ // very crude check to make sure it's not the same planet pulling on itself

	if( collision_check ){
	  if( rad < 0.8*(myplanet.r + current.r) && (myplanet.num > current.num )){//this actually decides if things hit.
	    collision_pair_1[collision_number] = myplanet.num;
	    collision_pair_2[collision_number] = current.num;
	    collision_number++;
	  }
	}
	
        myplanet.acc = myplanet.acc + (current.pos - myplanet.pos) * G * BD[currPln->plnNum].m / (rad*rad*rad) ;//Direct force calculation
	//F = (current.pos - myplanet.pos) * G * myplanet.m * BD[currPln->plnNum].m / (rad*rad*rad) ;//Direct force calculation
        //myplanet.acc = myplanet.acc + F/myplanet.m;
      }
      currPln = currPln->nextPln;
    }
  }
  
  else{
    //		intVec my = getIJKVec(n, myplanet.pos);  //intVec to store coordinates of region containing myplanet
    //k = 0;
   

    k = wellSepCoM(myplanet, regionToTest);
    //printf("%f %f\n",k,alpha*alpha);
    //printf("%f , %f\n",k,alpha*alpha);
    if( k > (alpha*alpha) ){
      n = pow(2, regionToTest.level);
      //printf("!i = %d, j = %d, k = %d\n",regionToTest.i, regionToTest.j, regionToTest.k);
      regionBox = regionToTest.i*n*n + regionToTest.j*n + regionToTest.k;
      //printf("!planet %d on level %d is in box %d, region %d\n", myplanet.num, regionToTest.level, myplanet.level[regionToTest.level] - getArrayIndex(regionToTest.level,0), regionBox);
      if( myplanet.level[regionToTest.level] - getArrayIndex(regionToTest.level, 0) == regionToTest.i*n*n + regionToTest.j*n + regionToTest.k ){
	//the planet is in the same region as the box
	//must subtract out the contrubition to theforce from
	newCOM  = (regionToTest.mass * regionToTest.com - myplanet.m * myplanet.pos) / (regionToTest.mass - myplanet.m);
	newMass = regionToTest.mass - myplanet.m;
	rad = radius(myplanet.pos, newCOM);

	myplanet.acc = myplanet.acc + newMass * (newCOM - myplanet.pos)  * G / (rad*rad*rad);
      }
      else{
	//space(lev); //printf("! region is well-separated, computing force via multipole\n");
	rad = radius(myplanet.pos,regionToTest.com);
	// F =  G * myplanet.m * regionToTest.mass * (regionToTest.com-myplanet.pos) / (rad*rad*rad);
	myplanet.acc = myplanet.acc + regionToTest.mass * (regionToTest.com-myplanet.pos)  * G / (rad*rad*rad);
	//printf("!-----multipole force calculation performed\n");
	//printf("!-----Region we are looking at is in level %d\n",regionToTest.level);
	//printf("!-----Distance between planet and region = %f\n", rad);
	//printf("!-----Distance to be considered well separated is %f\n", alpha*alpha);
	//myplanet.acc = myplanet.acc + F/myplanet.m;
      }
    }
    else{
      //space(lev); //printf("! region is not well-separated, descending to children\n");
      for(int l=0;l<8;l++){
	if(regions[regionToTest.kids[l]].numPln == 0){
	  continue;
	}
	else{
	  forceMagic(regions[regionToTest.kids[l]],myplanet,BD,alpha);
        }
      }
    }
  }
}	
