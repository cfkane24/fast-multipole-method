#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "getArrayIndex.h"
#include "vector.h"
#include "pln.h"
#include "region.h"
#include "planet.h"
#include "dynamic_region.h"

extern int LVL, N;
extern double L;
extern int total_regions;

void dynamic_recurse_divide_by_mass(dynamic_region &r){
  int i;
  
  if( r.mass == 0 ) return;//same thing as saying its empty
  //this is after regions have been populated.
  //need to do it somewhere else
  
  r.com = r.com / r.mass;
  if( r.level != LVL-1 ){
    for(i=0; i<8; i++){
      dynamic_recurse_divide_by_mass(*(r.child[i]));
    }
  }
}

int inThisChild(vector planet_pos, vector region_pos, double region_size){

    
  double x,y,z;
  
  x=planet_pos.x;
  y=planet_pos.y;
  z=planet_pos.z;

  if(x > L) x = L - region_size/2.0;
  if(y > L) y = L - region_size/2.0;
  if(z > L) z = L - region_size/2.0;

  if(x < 0) x = region_size/2.0;
  if(y < 0) y = region_size/2.0;
  if(z < 0) z = region_size/2.0;
  /*
    If the planet is outsize the bounds, it gets
    put in the box on the outermost edge.
   */
  
  int truth;

  if(x > region_pos.x && x < (region_pos.x + region_size)){
    if(y > region_pos.y && y < (region_pos.y + region_size)){
      if(z > region_pos.z && z < (region_pos.z + region_size)){
	truth = 1;
      }
      else{
	truth = 0;
      }
    }
    else{
      truth = 0;
    }
  }
  else{
    truth = 0;
  }
  return truth;
}

vector assignLocation(int i, dynamic_region currentRegion){

  double l = currentRegion.size/2.0;
  vector d = currentRegion.location;
  
  if(i==0){//bottom, left, front
    d = d;
  }
  else if(i==1){//top, left, front
    d.x += l;
  }
  else if(i==2){//top, left, back
    d.x += l;
    d.z += l;
  }
  else if(i==3){//bottom, right, front
    d.y += l;
  }
  else if(i==4){//top, right, front
    d.x += l;
    d.y += l;
  }
  else if(i==5){//bottom, right, back
    d.y += l;
    d.z += l;
  }
  else if(i==6){//bottom, left, back
    d.z += l;
  }
  else if(i==7){//top, right, back
    d.x += l;
    d.y += l;
    d.z += l;
  }
  else{
    printf("Looping over more than 8 children, somethin is messed up\n"); exit(0);
  }
  return d;
}

void addKids(dynamic_region &currentRegion){

  int i;
  dynamic_region *newRegion;//pointer to the child region
  
  for(i=0;i<8;i++){
    newRegion=(dynamic_region *)malloc(sizeof(dynamic_region));//give it memory
    newRegion->level = currentRegion.level + 1;
    newRegion->size  = currentRegion.size / 2.0;
    newRegion->location = assignLocation(i, currentRegion);
    newRegion->mass   = 0;
    newRegion->numPln = 0;
    newRegion->com.x  = 0;
    newRegion->com.y  = 0;
    newRegion->com.z  = 0;
    currentRegion.child[i] = newRegion;//set pointer equal to newRegion pointer
    total_regions++;
    //if I do *child[i] is it the value the pointer child points to?
    //I just want child[i] to point to newRegion
    //so if I set the pointers equal, then they point in the same place?
  }
}

void addPlanet(int num, pln **list)
{
  pln *newPln;
  newPln=(pln *)malloc(sizeof(pln));
  newPln->plnNum = num;
  newPln->nextPln = *list;//set the pointer equal to the location of the beginning of the list
  *list = newPln;
}

//===============================
void loopOverPlanets(planet BD[]){
}

void pop_level_0(dynamic_region &octree, planet BD[]){
  int i;
  octree.numPln = 0;
  
  //printf("---In pop_level_0\n");
  for(i=0; i<N; i++){
    octree.numPln++;
        
    octree.mass += BD[i].m;
    octree.com   = octree.com + BD[i].m * BD[i].pos;

    addPlanet(i, &octree.planets);    
  }

  addKids(octree);
  //printf("---Out of pop_level_0\n");
}

void loopOverRegions(dynamic_region current_region, planet BD[]){

  /*
    If the first region is done outside, do not need the 
    BD array.
   */
  //printf("-----In the beginning of loopOverRegions-----\n");
  int i,j;
  int counter=0;
  pln *curr;
  pln *next;

  vector planet_pos;
  vector region_pos;
  double region_size;
  
  dynamic_region *kid;//pointer to a child

  curr = current_region.planets;//cur points to the planet list of current_region
  //printf("-----Right before while loop\n");
  
  while(curr != NULL){//loop over the list of planets in the parent function
   
    j    = curr->plnNum;//number in the array of planets BD
    next = curr->nextPln;//points to next planet

    //somehow plnNum gets taken from random memory but curr->nextPln is not null?
    //printf("-------j=%d\n",j);
    planet_pos = BD[j].pos;
    //printf("-------About to put planets in the kids\n");
    for(i=0; i<8; i++){//loop over kids of parent region
      //printf("---------Parent region has %d planet(s) in it\n", current_region.numPln);
      //printf("---------Looking at child %d on level %d\n",i, current_region.child[i]->level);
      kid = current_region.child[i];//kid now points to *child[i]
      
      region_pos  = kid->location; //(*kid).pos
      region_size = kid->size;//(*kid).size
      //printf("---------Child %d is at (%.3f, %.3f, %.3f) and is %.3f big\n",i,region_pos.x, region_pos.y, region_pos.z, region_size);
      if(inThisChild(planet_pos, region_pos, region_size)){
	addPlanet(j, &(kid->planets));
	kid->numPln++;
	kid->mass += BD[j].m;
	kid->com = kid->com + BD[j].m * BD[j].pos;
	//printf("-----------Planet was in child %d\n",i);
	break;//break from the loop if we found what child it is in
      }
      //printf("---------Done with  child %d\n",i);
    }
    //printf("-------Found the planet\n");
    curr = next;
    
    //printf("-------curr=next is done\n");
    counter++;
  }
  //printf("-----About to loop over the kids-----\n");
  for(i=0; i<8; i++){
    kid = current_region.child[i];
    //printf("Kid on level %d, has %d planets in it\n", kid->level, kid->numPln);
    if(kid->numPln != 0 && kid->level != LVL-1){
      //if it has planets in it AND it is not on the lowest level
      addKids(*kid);
      loopOverRegions(*kid, BD);
    }
  }
}
