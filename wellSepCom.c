#include <math.h>
#include <stdio.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"

/*
Notes: could scale (aka multiply) alpha by some factor of how concentrated the center of mass is?
	-> Do this via comparing separations of CoM and the CoM's of each child region?
*/

extern double L;

double wellSepCom(planet currPlanet, region tempReg)
{
  int l = tempReg.level;//level of the region we are looking at
  double beta = L/pow(2,l);//L is edge length of box on level 0. Beta is box length on level l.
  //box length at level l
  double beta2 = beta*beta;//Use beta^2 for cheaper math. 
  vector tempCoM = tempReg.com;
  vector sep = currPlanet.pos - tempCoM;//this is the vector ( [x1-x2],[y1-y2],[z1-z2] )
  //so sep^2 is the squared distance
 
  
  //printf("!Determining whether a planet at (%.2f,%.2f,%.2f) is distant from a region at (%.2f,%.2f,%.2f) with length %f : ",currPlanet.pos.x,currPlanet.pos.y,
  //currPlanet.pos.z,tempCoM.x,tempCoM.y,tempCoM.z,beta);

  return sep*sep/beta2;
}
