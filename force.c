#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"
#include "timing.h"

extern double G;
extern int collision_pair_1[];
extern int collision_pair_2[];
extern int collision_number;
extern int collision_check;
extern int LVL;
extern long int proclist[];

extern double alpha;
extern float alphasq;

void forceMagic(const region &regionToTest, planet &myplanet, const planet *BD){

  bool process, redoCOM;
  int i;
  double k;
  double radsq,rad;
  pln *curr_pln;
  planet curr_planet;
  process=false;
  redoCOM=false;
  radsq=0;
  vector da;
  vector newCOM;
  double newMass;
  
  if(regionToTest.numPln == 1)
  { // if region has one planet
    if(regionToTest.planets->plnNum == myplanet.num) return; // if it's me, abort
    else process = true; // otherwise mark region for processing
  }
  
  else
  {// check well-separation
    radsq = Magnitude_Sq(regionToTest.com-myplanet.pos); 
    k = radsq/(regionToTest.size*regionToTest.size);
    if(k>alphasq) process = true;
  }
  
  if(process)
  {
// do region
    starttimer(0);
    ++proclist[regionToTest.level];
    if (radsq) rad=sqrt(radsq); 
    else rad = radius(myplanet.pos,regionToTest.com);

    //*******************************
    // Need to decide if things collide or not
    // This collision check code is only used by Chris

    if(collision_check)
    {//only check at the last Omelyan force step
      //if one planet do it
      if(regionToTest.numPln == 1)
      {
	curr_planet=BD[regionToTest.planets->plnNum];
	if( rad < 0.8*(myplanet.r + curr_planet.r) && (myplanet.num > curr_planet.num))
	{
	   collision_pair_1[collision_number] = myplanet.num;
	   collision_pair_2[collision_number] = curr_planet.num;
	   collision_number++;
	}
      }
      
      else if(myplanet.r > alpha*regionToTest.size)
      {
	curr_pln = regionToTest.planets;
	while(curr_pln != NULL)
	{
	  curr_planet=BD[curr_pln->plnNum];
	  if( rad < 0.8*(myplanet.r + curr_planet.r) && (myplanet.num > curr_planet.num))
	  {
	      collision_pair_1[collision_number] = myplanet.num;
	      collision_pair_2[collision_number] = curr_planet.num;
	      collision_number++;
	  }
	  curr_pln = curr_pln->nextPln;
	}
      }
    }

    myplanet.acc += regionToTest.mass * (regionToTest.com-myplanet.pos) * G / (rad*rad*rad);
  }
  
  else
  {// recurse
    for(i=0; i<8; i++)
    {
      if(regionToTest.child[i] != NULL)
      {
	forceMagic(*regionToTest.child[i], myplanet, BD);
      }
    }
  }
}

