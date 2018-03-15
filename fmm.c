#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "region.h"
#include "force.h"
#include "collisions.h"
#include "initialize.h"
#include "exactForce.h"
#include "populateRegions.h"
#include "planetListFunctions.h"
#include "resetregion.h"
#include "parentChild.h"
#include "systemProperties.h"
#include "integrators.h"
#include "timing.h"

//UNITS
//Mass   : Earth Mass
//Length : AU (astronomical unit)
//Time   : YEAR
//fix planet sizes, way too big in these units.
//or does this not matter?

int LVL = 6;
int numDel;
int totalIndeces;
int collision_check;
int collision_number;
int collision_pair_1[30000];
int collision_pair_2[30000];
double alpha;
region *regions;

int N; //number of planets    
int a; //number of collisions
double dt; //time step
double G  = 4*M_PI*M_PI; //Newton's constant 
double L; //inital grid size
double rho = 20; //density of planets. Will be making this a planet property eventually
vector com; //center of mass to test my functions


int main(int nParam, char **paramList){

  if( nParam != 8 ) {printf("\n!Usage: ./<this> <N> <dt> <frameskip> <initial velocity> <box width> <initial conditions width> <alpha>\n\n"); exit(0);}

  N              = atoi(paramList[1]);
  dt             = atof(paramList[2]);
  int fSkip      = atoi(paramList[3]);
  double v       = atof(paramList[4]);
  L              = atof(paramList[5]);
  double scatter = atof(paramList[6]);
  alpha          = atof(paramList[7]);

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

  for(i=0; i<LVL; i++){
    totalIndeces += pow(8,i);
  }
  
  regions=(region*)malloc(sizeof(region)*totalIndeces);

  findKids(LVL);
      
  Initialize(BD, scatter, v);

  M_init = Mass(BD);
  E_init = Energy(BD);
  P_init = Momentum(BD);

  for( t=0; 1; t+=dt ){

    reset_region(regions[0]);
        
    populateRegions(BD, totalIndeces);
    
    popLists(BD);
    
    omelyan(BD,method);
    //leapfrog(BD,method);
    
    a += collisionCheck(BD);
   
    if( frame % fSkip == 0 ){
      printf("T -0.8 0.82\nE = %f : P = %f : # Collisions = %d : t = %f : Planets = %d : Mass = %f\n", Energy( BD ), Momentum( BD ), a, t, N, Mass( BD ) );
      for( i=0; i<N; i++ ){
	printf("c3 %e %e %e %e\n", BD[i].pos.x, BD[i].pos.y, BD[i].pos.z, BD[i].r);
      }
      printf("F\n"); 
    }
    frame++;
  }
}
