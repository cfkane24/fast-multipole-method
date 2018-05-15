#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"
#include "collisions.h"
#include "initialize.h"
#include "exactForce.h"
#include "systemProperties.h"
#include "integrators.h"
#include "popRegions.h"
#include "force.h"
#include "resetOctree.h"
#include "timing.h"

//UNITS
//Mass   : Earth Mass
//Length : AU (astronomical unit)
//Time   : YEAR
//fix planet sizes, way too big in these units.
//or does this not matter?

int LVL;
int numDel;
int totalIndeces;
int collision_check;
int collision_number;
int collision_pair_1[30000];
int collision_pair_2[30000];
double alpha;


int N; //number of planets    
int a; //number of collisions
double dt; //time step
double G  = 4*M_PI*M_PI; //Newton's constant 
double L; //inital grid size
double rho = 20; //density of planets. Will be making this a planet property eventually
vector com; //center of mass to test my functions

int total_regions;
region octree;

void showOctree(region parent){
  int i;
  int j=0;
  printf("!P: level=%d, #planets=%d\n", parent.level, parent.numPln);
  for(i=0; i<8; i++){
    if(parent.child[i] != NULL){
      j+=parent.child[i]->numPln;
      printf("!***C: level=%d, #planets=%d\n", parent.child[i]->level, parent.child[i]->numPln);
    }
  }
  printf("!***Diff in planet num = %d\n",j-parent.numPln);

  for(i=0;i<8;i++){
    if(parent.child[i] != NULL){
      showOctree(*(parent.child[i]));
    }
  }
}


int main(int nParam, char **paramList){
  
  char var[9], val[9];//Placeholders to be used when reading from config.txt
  FILE *config=fopen("config.txt", "r");

  double v, scatter;
  int fSkip;

  while( fscanf(config, "%s %s", var, val) != EOF)
    {
      if( strcmp(var, "LVL") == 0)       LVL     = atoi(val);
      if( strcmp(var, "N") == 0)         N       = atoi(val);
      if( strcmp(var, "dt") == 0)        dt      = atof(val);
      if( strcmp(var, "frameskip") == 0) fSkip   = atoi(val);
      if( strcmp(var, "v_i") == 0)       v       = atof(val);
      if( strcmp(var, "L") == 0)         L       = atof(val);
      if( strcmp(var, "scatter") == 0)   scatter = atof(val);
      if( strcmp(var, "alpha") == 0)     alpha   = atof(val);
    }

  int fil=1;
  while(fil<nParam-1)
    {
      printf("!Reading command line params  ..  fil:%d  pL:%s  pL+1:%s \n", fil, paramList[fil], paramList[fil+1]);
      if( strcmp(paramList[fil], "LVL") == 0)       LVL     = atoi(paramList[fil+1]);
      if( strcmp(paramList[fil], "N") == 0)         N       = atoi(paramList[fil+1]);
      if( strcmp(paramList[fil], "dt") == 0)        dt      = atof(paramList[fil+1]);
      if( strcmp(paramList[fil], "frameskip") == 0) fSkip   = atoi(paramList[fil+1]);
      if( strcmp(paramList[fil], "v_i") == 0)       v       = atof(paramList[fil+1]);
      if( strcmp(paramList[fil], "L") == 0)         L       = atof(paramList[fil+1]);
      if( strcmp(paramList[fil], "scatter") == 0)   scatter = atof(paramList[fil+1]);
      if( strcmp(paramList[fil], "alpha") == 0)     alpha   = atof(paramList[fil+1]);
      fil+=2;
    }
  /*
  if( nParam != 9 ) {printf("\n!Usage: ./<this> <LVL> <N> <dt> <frameskip> <initial velocity> <box width> <initial conditions width> <alpha>\n\n"); exit(0);}

  LVL            = atoi(paramList[1]);
  N              = atoi(paramList[2]);
  dt             = atof(paramList[3]);
  int fSkip      = atoi(paramList[4]);
  double v       = atof(paramList[5]);
  L              = atof(paramList[6]);
  double scatter = atof(paramList[7]);
  alpha          = atof(paramList[8]);
  */
  planet BD[N];

  int i;
  
  totalIndeces = 0;
  int frame    = 0;
  int method   = 1;
  
  double t;
  double M_init, E_init, P_init;
  
  if(method == 1){ printf("!method = 1. Using multipole method\n"); }
  else if(method == 0){ printf("!method = 0. Using exact force\n"); }
  else{ printf("!Invalid entry for force method. Please choose either 0 or 1\n"); exit(0); }

  /*
  for(i=0; i<LVL; i++){
    totalIndeces += pow(8,i);
  }
  
  regions=(region*)malloc(sizeof(region)*totalIndeces);

  findKids(LVL);
  */
  
  Initialize(BD, scatter, v);
  printf("---Done intializing planets\n");
  octree.numPln = 0;
  octree.level  = 0;
  octree.size   = L;
  octree.location.x = 0;
  octree.location.y = 0;
  octree.location.z = 0;
  
  M_init = Mass(BD);
  E_init = Energy(BD);
  P_init = Momentum(BD);
  printf("---Done everything before the time loop\n");
  for( t=0; 1; t+=dt ){

    //    printf("t=%f\n",t);
    if(t!=0) resetOctree(octree);//releases memory of all children of octree
    //printf("---Done with resetOctree\n");
    pop_level_0(octree, BD);
    //printf("---Done with pop_level_0\n");
    total_regions=1;
    loopOverRegions(octree, BD);//creates octree
    //showOctree(octree);
    //printf("!---There are %d regions in the octree\n", total_regions);
    //printf("!---These %d regions take up %ld bytes\n", total_regions,total_regions*sizeof(octree));
    //printf("!---Done with loopOverRegions\n");
    recurse_divide_by_mass(octree);
    //printf("!---Done with recurse_divide_by_mass\n");
    //reset_region(regions[0]);
    //populateRegions(BD, totalIndeces);
    //popLists(BD);
    
    omelyan(BD,method);
    //leapfrog(BD,method);
    //printf("!---Done with integration\n");
    //leapfrog(BD,method);
    
    a += collisionCheck(BD);
    //printf("!---Done with collisionCheck\n");
    if( frame % fSkip == 0 ){
      printf("T -0.8 0.82\nE = %.2f : P = %.2f : # Collisions = %d : t = %.2f : Planets = %d : Mass = %.2f\n", Energy( BD ), Momentum( BD ), a, t, N, Mass( BD ) );
      for( i=0; i<N; i++ ){
	printf("c3 %e %e %e %e\n", BD[i].pos.x, BD[i].pos.y, BD[i].pos.z, BD[i].r);
      }
      printf("F\n"); 
    }
    frame++;
  }
}
