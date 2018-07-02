#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vector.h"
#include "pln.h"
#include "planet.h"
#include "region.h"

extern int LVL, N, numDel;
extern double L;
extern int total_regions;
extern vector BDCOM;
extern int currTotalRegions;

void countRegions(region parent)
{
  int i;
  currTotalRegions++;
  for(i=0; i<8; i++)
  {
    if(parent.child[i] != NULL) countRegions(*(parent.child[i]));
  }
}

int countChildren(region parent)
{
  int i, numChild=0;
  for(i=0; i<8; i++)
  {
    if (parent.child[i] != NULL) numChild++;
  }
  return numChild;
}

void clearList(pln **list)
{
  //printf("!PLF: in clearLists \n");
  pln *curr = *list;
  pln *next;
  //printf("!PLF: Start while loop\n");
  int n=0;
  while(curr!=NULL)
  {
    next = curr->nextPln;
    //		printf("curr: %p  next: %p\n", curr, next);
    free(curr);
    curr = next;
    n++;
    numDel++;
    //		printf("!PLF: # of elements cleared from this list: %d\n", n);
  }
  *list = NULL;
}

void recurse_divide_by_mass(region &r){
  
  int i;
  if( r.mass == 0 ) return;//same thing as saying its empty
  //this is after regions have been populated.
  //need to do it somewhere else
  
  r.com = r.com / r.mass;
  // printf("!popRegions.c/recurse_divide_by_mass: region at level %d has total mass %.2e with com at %.4e %.2e %.2e\n", r.level, r.mass, r.com.x, r.com.y, r.com.z);
  if(r.numPln > 1){
    for(i=0; i<8; i++){
      if(r.child[i]!=NULL) recurse_divide_by_mass(*(r.child[i]));
    }
  }
}

int inThisChild(vector planet_pos, vector region_pos, double region_size)
{    
  double x,y,z;
  
  x=planet_pos.x;
  y=planet_pos.y;
  z=planet_pos.z;

  // if(x > L) x = L - region_size/2.0;
  // if(y > L) y = L - region_size/2.0;
  // if(z > L) z = L - region_size/2.0;

  // if(x < 0) x = region_size/2.0;
  // if(y < 0) y = region_size/2.0;
  // if(z < 0) z = region_size/2.0;
  /*
    If the planet is outsize the bounds, it gets
    put in the box on the outermost edge.
   */
  
  int truth = 0;

  if(x >= region_pos.x && x < (region_pos.x + region_size)){
    if(y >= region_pos.y && y < (region_pos.y + region_size)){
      if(z >= region_pos.z && z < (region_pos.z + region_size)){
	truth = 1;
      }
    }
  }
  return truth;
}


void addKids(region &currentRegion){

  int i=0;
  int j,a,b,c;
  region *newRegion;//pointer to the child region
  double l = currentRegion.size/2.0;
  // whoever came up with this for loop, much more clever than my idea. -CK
  for(a=0; a<2; a++)
  {
    for(b=0; b<2; b++)
    {
      for(c=0; c<2; c++)
      {    
	newRegion=(region *)malloc(sizeof(region));//give it memory
	newRegion->level = currentRegion.level + 1;
	newRegion->size  = currentRegion.size / 2.0;
	newRegion->mass   = 0;
	newRegion->numPln = 0;
	newRegion->com.x  = 0;
	newRegion->com.y  = 0;
	newRegion->com.z  = 0;
	newRegion->location.x = currentRegion.location.x + a*l;
	newRegion->location.y = currentRegion.location.y + b*l;
	newRegion->location.z = currentRegion.location.z + c*l;
	newRegion->planets = NULL;// planet list set to NULL
	
	for(j=0; j<8; j++) newRegion->child[j] = NULL;
	
	currentRegion.child[i] = newRegion;//set pointer equal to newRegion pointer
	total_regions++;
	i++;
      }
    }
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

void pop_level_0(region &octree, planet BD[])
{
  int i;
  vector test;
  vector extreme; // this vector will store the extreme displacement vectors 
                  // from the COM to determine what L needs to be
  vector extreme_planet_num;
  if(octree.level != 0)
  {
    printf("!pop_level_0 working on a region not on level 0\n");
    exit(0);
  }
  
  octree.numPln = 0;
  octree.mass  = 0;
  octree.size = L;
  octree.location.x = BDCOM.x - L/2.0;
  octree.location.y = BDCOM.y - L/2.0;
  octree.location.z = BDCOM.z - L/2.0;
  octree.com.x = 0;
  octree.com.y = 0;
  octree.com.z = 0;
  //printf("---In pop_level_0\n");
  
  for(i=0; i<N; i++)
  {
    octree.numPln++;
     
    octree.mass += BD[i].m;
    octree.com   = octree.com + BD[i].m * BD[i].pos;
        
    addPlanet(i, &octree.planets);    
  }

  octree.com = octree.com/octree.mass;

  for(i=0; i<8; i++) octree.child[i]=NULL;
  
  addKids(octree);
  //printf("---Out of pop_level_0\n");
}

void loopOverRegions(region &current_region, planet BD[]){

  /*
    If the first region is done outside, do not need the 
    BD array.
   */
  //printf("-----In the beginning of loopOverRegions-----\n");
  int i,j,k;
  int counter=0;
  pln *curr;
  pln *next;
  bool freed;

  vector planet_pos;
  vector region_pos;
  double region_size;
  
  //printf("\n\n*****-----***** In a new loopOverRegions call *****-----*****\n\n");
  curr = current_region.planets;//cur points to the planet list of current_region
  counter=0;
  
  while(curr != NULL)
  { // loop over the list of planets in the parent function
    counter++;
    j = curr->plnNum; // number in the array of planets BD[j]
    next = curr->nextPln; // points to next pln
    planet_pos = BD[j].pos;
    
    for(i=0; i<8; i++)
    { // loop over kids of parent region
      region_pos  = current_region.child[i]->location;
      region_size = current_region.child[i]->size;
      
      if( inThisChild(planet_pos, region_pos, region_size) )
      {
	addPlanet(j, &(current_region.child[i]->planets));
	current_region.child[i]->numPln++;
	current_region.child[i]->mass += BD[j].m;
	current_region.child[i]->com = current_region.child[i]->com + BD[j].m * BD[j].pos;
	break; // break from the loop if we found what child it is in
      }
    }
    curr = next;
  }
  // if it is not on the lowest level, check if they need kids
  for(i=0; i<8; i++){
    // currTotalRegions = 0;
    // freed = false;
    // countRegions(octree);
    if(current_region.child[i]->numPln == 0)
    {
      free(current_region.child[i]);
      current_region.child[i]=NULL;
      total_regions--;
    }
    else if(current_region.child[i]->numPln > 1)
    {
      addKids(*current_region.child[i]);
      loopOverRegions(*current_region.child[i], BD);
    }
  }
}
