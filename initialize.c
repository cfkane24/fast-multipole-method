#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"

extern int N;
extern int N_overflow;
extern double L;
extern double rho;
extern double r_i;

void Initialize( planet BD[], double scatter, double v, double r_i ){

  int i;
  double mass = 0;
  vector vcm;

  double delx, dely, delz;
  
  vcm.x = 0;
  vcm.y = 0;
  vcm.z = 0;

  for( i=0; i<N; i++ ){

    BD[i].num = i;

    BD[i].pos.x = scatter * 0.5 + (drand48()-0.5) * scatter; //not neg so fits planetRegion
    BD[i].pos.y = scatter * 0.5 + (drand48()-0.5) * scatter; //not neg so fits planetRegion
    BD[i].pos.z = scatter * 0.5 + (drand48()-0.5) * scatter; //not neg so fits planetRegion
    
    BD[i].vel.x = v * (drand48()-.5);
    BD[i].vel.y = v * (drand48()-.5);
    BD[i].vel.z = v * (drand48()-.5);
    
    //BD[i].r = 0.3;
    //BD[i].r = 0.03;
    BD[i].r = r_i;
    BD[i].m = 4/3 * M_PI * rho * BD[i].r * BD[i].r * BD[i].r;

    mass += BD[i].m;
    vcm   = vcm + BD[i].vel * BD[i].m;
  }

  vcm = vcm / mass;

  for( i=0; i<N; i++ ){
    BD[i].vel = BD[i].vel - vcm;
  }
}
