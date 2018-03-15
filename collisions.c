#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"

extern int N;
extern int collision_number;
extern int collision_pair_1[];
extern int collision_pair_2[];
extern double rho;

/********************************************/

void compactify(planet BD[]){
  for(int i=0; i<N; i++){
    if (BD[i].m==0){
      N--;
      BD[i].pos = BD[N].pos;
      BD[i].vel = BD[N].vel;
      BD[i].m   = BD[N].m;
      BD[i].r   = BD[N].r;
    }
  }
}

/********************************************/

//void Bounce(planet BD[], int i, int j){
void bounce(planet &BD1, planet &BD2){

  double m1, m2;
  vector r1, r2, r, r_hat;
  vector v1, v2, vcm, delta_v;

  m1 = BD1.m;
  m2 = BD2.m;
  
  r1 = BD1.pos;
  r2 = BD2.pos;

  v1 = BD1.vel;
  v2 = BD2.vel;
  
  
  vcm = ( m1 * v1 + m2 * v2 ) / ( m1 + m2 );

  //printf("!vx = %f, vy = %f, vz = %f\n",vcm.x, vcm.y, vcm.z);
  
  v1 = v1 - vcm;
  v2 = v2 - vcm;

  delta_v = v2 - v1;
  
  r = r2 - r1;//vector pointing from one center to another
  //printf("!rx = %f, ry = %f, rz = %f\n",r.x, r.y, r.z);
  r_hat = r / Magnitude(r);
  
  v1 = v1 + ( 2 * delta_v * r_hat ) / ( 1 + m1 / m2 ) * r_hat;
  v2 = v2 - ( 2 * delta_v * r_hat ) / ( 1 + m2 / m1 ) * r_hat;

  v1 = v1 + vcm;
  v2 = v2 + vcm;

  BD1.vel = v1;
  BD2.vel = v2;
  
}

/********************************************/

//void Stick(planet BD[], int i, int j){
void stick(planet &BD1, planet &BD2){

  BD1.pos = (BD1.m*BD1.pos + BD2.m*BD2.pos)/(BD1.m + BD2.m); 
  BD1.vel = (BD1.m*BD1.vel + BD2.m*BD2.vel)/(BD1.m + BD2.m);
  BD1.m   = (BD1.m + BD2.m);
  BD1.r   = pow( BD1.m/rho, 1.0/3 );
  //don't shift planets yet, since if planet N-1 also collides this breaks things. 
  //just mark planet j as "to be deleted" by setting its mass to zero.
  //we can then compactify the planet list later.
  BD2.m=0;  
}

/********************************************/

int collisionCheck(planet BD[]){

  int i,j;
  int ndel = 0;
  double rad;
  
  for(int k=0; k<collision_number; k++){
    i = collision_pair_1[k];
    j = collision_pair_2[k];
    if(i != j){
      rad = radius(BD[i].pos, BD[j].pos);
      if( rad < 0.8*(BD[i].r + BD[j].r)){
	if(drand48() > 0.5){
	  stick(BD[i], BD[j]);
	  ndel++;
	}
	else{
	  bounce(BD[i], BD[j]);
	}
      }
    }
  }
  if( ndel>0 ) compactify(BD);
  return ndel;
}
