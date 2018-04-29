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

  int i, in_box;
  double k;
  double rad;
  pln *curr_pln;
  planet curr_planet;
  double newMass;
  vector newCOM;
  vector da;

  da.x=da.y=da.z=0;

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
	da = (curr_planet.pos - myplanet.pos) * G * curr_planet.m / (rad*rad*rad);
	if(sqrt(da*da) > 150) printf("!1___da = (%.3f, %.3f, %.3f)\n", da.x, da.y, da.z);

	myplanet.acc = myplanet.acc + da;// + (curr_planet.pos - myplanet.pos) * G * curr_planet.m / (rad*rad*rad);
      }
      curr_pln = curr_pln->nextPln;
    }
  }

  else{
    k = wellSepCom(myplanet, regionToTest);
    if( k > (alpha*alpha) ){
      curr_pln = regionToTest.planets;
      in_box = 0;
      while(curr_pln != NULL ){
	if(curr_pln->plnNum == myplanet.num){
	  in_box = 1;
	  break;
	}
	curr_pln = curr_pln->nextPln;
      }
      if( in_box == 1 ){
	//the planet is in the same region as the box
	//must subtract out the contrubition to theforce from
	newCOM  = (regionToTest.mass * regionToTest.com - myplanet.m * myplanet.pos) / (regionToTest.mass - myplanet.m);
	newMass = regionToTest.mass - myplanet.m;
	rad = radius(myplanet.pos, newCOM);

	da = newMass * (newCOM - myplanet.pos)  * G / (rad*rad*rad);
	if(sqrt(da*da) > 150){
	  printf("!2___da = (%.3f, %.3f, %.3f)\n", da.x, da.y, da.z);
	  printf("!___**Original region level = %d\n", regionToTest.level);
	  printf("!___**Original region mass = %.3f\n", regionToTest.mass);
	  printf("!___**Planet mass = %.3f\n", myplanet.m);
	  printf("!___**Region mass = %.3f\n", newMass);
	  printf("!___**Region com = (%.3f, %.3f, %.3f)\n", newCOM.x, newCOM.y, newCOM.z);
	  printf("!___**Planet loc = (%.3f, %.3f, %.3f)\n", myplanet.pos.x, myplanet.pos.y, myplanet.pos.z);
	  printf("!___**rad = %.3f\n", rad);
	}
	myplanet.acc = myplanet.acc + da;//+ newMass * (newCOM - myplanet.pos)  * G / (rad*rad*rad);
      }
      else{
	rad = radius(myplanet.pos,regionToTest.com);
	da = regionToTest.mass * (regionToTest.com-myplanet.pos)  * G / (rad*rad*rad);
	if(sqrt(da*da) > 150) printf("!3___da = (%.3f, %.3f, %.3f)\n", da.x, da.y, da.z);

	
	myplanet.acc = myplanet.acc + da;//regionToTest.mass * (regionToTest.com-myplanet.pos)  * G / (rad*rad*rad);
      }
    }
    /*
    if(k>(alpha*alpha)){
      rad = radius(myplanet.pos, regionToTest.com);
      myplanet.acc = myplanet.acc + regionToTest.mass * (regionToTest.com-myplanet.pos) * G / (rad*rad*rad);
    }
    */
    else{
      for(i=0; i<8; i++){
	if(regionToTest.child[i]->planets != NULL){
	  dynamic_forceMagic(*regionToTest.child[i], myplanet, BD, alpha);
	}
      }
    }
  }
}
