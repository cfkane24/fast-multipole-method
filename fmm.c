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
int collision_check;
int collision_number;
int collision_pair_1[30000];
int collision_pair_2[30000];
double alpha;
float alphasq;
long int proclist[100];

int N;                   // initial number of planets    
int a;                   // number of inelastic collisions
double dt;               // time step
double r_i;              // initial planet radius
double G  = 4*M_PI*M_PI; // Newton's constant 
double L;                // octree size, dynamically chosen
double rho = 20;         // density of planets

int total_regions;
int currTotalRegions;

vector BDCOM; // center of mass of bodies

region octree;


vector getCoM(planet *BD)
{
  vector com; com.x=0; com.y=0; com.z=0;
  int i; double mTot=0;
  for(i=0; i<N; i++)
  {
    com = com + BD[i].pos*BD[i].m;
    mTot += BD[i].m;
  }
  com = com / mTot;
  
  return com;
}


void setL(planet *BD, vector com)
{
	//Set L based on particle locations, choosing twice the largest single coordinate relative to center of mass (assuming CoM was properly set to 0,0,0 in initialize.c
	//FIXME what do we do if particle leave region 0? Maybe make a new list separate from the region list to keep track of those?
	//FIXME Should do magnitudes
	int i;
	double xmax=0, ymax=0, zmax=0;
	for(i=0; i<N; i++)
	{
	  if( abs(BD[i].pos.x - com.x) > xmax) xmax = abs(BD[i].pos.x - com.x);
	  if( abs(BD[i].pos.y - com.y) > ymax) ymax = abs(BD[i].pos.y - com.y);
	  if( abs(BD[i].pos.z - com.z) > zmax) zmax = abs(BD[i].pos.z - com.z);
	}
	L=xmax;
	if(ymax > L) L=ymax;
	if(zmax > L) L=zmax;

	L*=4;//Before this line, L was the largest single coordinate of a particle. We will center region 0 at 0,0,0 and we want it to be sixteen times as large on every side as the furthest particle
}


void showOctree(region parent)
{
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


int main(int nParam, char **paramList)
{  
  char var[100], val[100];//Placeholders to be used when reading from config.txt
  FILE *config=fopen("config.txt", "r");

  double v, scatter, r_i;
  int fSkip;

  while( fscanf(config, "%s %s", var, val) != EOF)
  {
    if( strcmp(var, "LVL") == 0)       LVL     = atoi(val);
    if( strcmp(var, "N") == 0)         N       = atoi(val);
    if( strcmp(var, "dt") == 0)        dt      = atof(val);
    if( strcmp(var, "frameskip") == 0) fSkip   = atoi(val);
    if( strcmp(var, "v_i") == 0)       v       = atof(val);
    if( strcmp(var, "scatter") == 0)   scatter = atof(val);
    if( strcmp(var, "alpha") == 0)     alpha   = atof(val);
    if( strcmp(var, "r_i") == 0)       r_i     = atof(val);
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
    if( strcmp(paramList[fil], "scatter") == 0)   scatter = atof(paramList[fil+1]);
    if( strcmp(paramList[fil], "alpha") == 0)     alpha   = atof(paramList[fil+1]);
    if( strcmp(paramList[fil], "r_i") == 0)       r_i     = atof(paramList[fil+1]);
    fil+=2;
  }

  planet BD[N];
  
  int i;
  
  int frame    = 0;
  int method   = 1;
  
  double t;
  double M_init, E_init, P_init;
  double M_curr, E_curr, P_curr;
  
  if(method == 1){ printf("!method = 1. Using multipole method\n");}
  else if(method == 0){ printf("!method = 0. Using exact force\n");} 
  else {printf("!Invalid entry for force method. Please choose either 0 or 1\n"); exit(0);}

  alphasq = alpha*alpha;
 
  for(i=0;i<100;i++) proclist[i]=0;
  
  Initialize(BD, scatter, v, r_i);
  printf("!Done intializing planets\n");

  BDCOM = getCoM(BD);
  setL(BD, BDCOM);
  printf("!fmm.c: L = %1.3e\n", L);
  
  octree.numPln = 0;
  octree.level  = 0;
  octree.size   = L;
  
  octree.location.x = BDCOM.x-L/2.0;
  octree.location.y = BDCOM.y-L/2.0;
  octree.location.z = BDCOM.z-L/2.0;
  
  M_init = Mass(BD);
  E_init = Energy(BD);
  P_init = Momentum(BD);

  M_curr = M_init;
  E_curr = E_init;
  P_curr = P_init;
  
  printf("!fmm.c: Done everything before the time loop\n");
  for( t=0; 1; t+=dt )
  {  
    if(t!=0) resetOctree(octree);
    pop_level_0(octree, BD);
    
    total_regions=1;
    loopOverRegions(octree, BD);
    //showOctree(octree);
    
    recurse_divide_by_mass(octree);

    omelyan(BD, method);
    //leapfrog(BD, method);
    
    a += collisionCheck(BD);
        
    if( frame % fSkip == 0 )
    {
      if( frame % 1 == 0 )
      {
	E_curr = Energy(BD);
	P_curr = Momentum(BD);
	M_curr = Mass(BD);
      }
      
      printf("T -0.8 0.82\nE(0) = %.2f : E(t) = %.2f : P = %.2f : t = %.2f : Planets = %d : Mass = %.2f\n", E_init, E_curr, P_curr, t, N, M_curr);
      
      for( i=0; i<N; i++ ) printf("c3 %e %e %e %e\n", BD[i].pos.x, BD[i].pos.y, BD[i].pos.z, BD[i].r);
      
      printf("F\n");   
    }
    frame++;
  }
}
