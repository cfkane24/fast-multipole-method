#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "planet.h"
#include "pln.h"
#include "dynamic_region.h"
#include "dynamic_force.h"
#include "exactForce.h"

extern int N;
extern double dt;
extern double alpha;
extern int collision_check;
extern int collision_number;
/****************************************/
void Reset_Accelerations( planet BD[] ){
  int i;
  for(i=0;i<N;i++){
    BD[i].acc.x = 0;
    BD[i].acc.y = 0;
    BD[i].acc.z = 0;
  }
}

/****************************************/
/*        Leapfrog Integrator           */
/****************************************/

/****************************************/

void Position_Half_Step( planet BD[] ){

  int i;
  for( i=0; i<N; i++ ){
    BD[i].pos = BD[i].pos + BD[i].vel*dt/2;
  }
}

/****************************************/

void Velocity_Full_Step( planet BD[] ){
  
  int i;
  for( i=0; i<N; i++ ){
    BD[i].vel = BD[i].vel + BD[i].acc*dt;
  }
}

/****************************************/

void leapfrog( planet BD[], int method )
{
  int i;
  Position_Half_Step(BD);
  Reset_Accelerations(BD);

  collision_check  = 1;
  collision_number = 0;
  if( method == 0 ){
    Exact_Force(BD);
  }
  
  else{
    for( i=0; i<N; i++ ){
      //printf("!-----Doing force calculation for planet %d\n",i);
      dynamic_forceMagic(octree, BD[i], BD, alpha);
    }
  }
  collision_check = 0;

  Velocity_Full_Step(BD);
  Position_Half_Step(BD);
}


/****************************************/
/*         Omelyan Integrator           */
/****************************************/

double eps = 0.1786178958448;
double lam = -0.2123418310626;
double chi = -0.06626458266982;

//Link to the paper I got this from
//https://arxiv.org/pdf/cond-mat/0110585.pdf

/****************************************/

void Position_Step_1( planet BD[] ){

  int i;
  for(i=0;i<N;i++){
    BD[i].pos = BD[i].pos + BD[i].vel*eps*dt;
  }
}

/****************************************/

void Velocity_Step_1(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].vel = BD[i].vel + BD[i].acc*(1-2*lam)*dt/2;
  }
}

/****************************************/

void Position_Step_2(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].pos = BD[i].pos + BD[i].vel*chi*dt;
  }
}

/****************************************/

void Velocity_Step_2(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].vel = BD[i].vel + BD[i].acc*lam*dt;
  }
}

/****************************************/

void Position_Step_3(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].pos = BD[i].pos + BD[i].vel*(1-2*(chi+eps))*dt;
  }
}

/****************************************/

void Velocity_Step_3(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].vel = BD[i].vel + BD[i].acc*lam*dt;
  }
}

/****************************************/

void Position_Step_4(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].pos = BD[i].pos + BD[i].vel*chi*dt;
  }
}

/****************************************/

void Velocity_Step_Final(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].vel = BD[i].vel + BD[i].acc*(1-2*lam)*dt/2;
  }
}

/****************************************/

void Position_Step_Final(planet BD[]){
  int i;
  for(i=0;i<N;i++){
    BD[i].pos = BD[i].pos + BD[i].vel*eps*dt;
  }
}

/****************************************/

void omelyan( planet BD[], int method )
{
  int i;

  Position_Step_1(BD);
  Reset_Accelerations(BD);


  if( method == 0 ){
    Exact_Force(BD);
  }
  
  else{
    for(i=0; i<N; i++){
      dynamic_forceMagic(octree, BD[i], BD, alpha);
    }
  }
    
  Velocity_Step_1(BD);
  Position_Step_2(BD);
  Reset_Accelerations(BD);

  if( method == 0 ){
    Exact_Force(BD);
  }
  
  else{
    for(i=0; i<N; i++){
      dynamic_forceMagic(octree, BD[i], BD, alpha);
    }
  }
    
  Velocity_Step_2(BD);
  Position_Step_3(BD);
  Reset_Accelerations(BD);
  
  if( method == 0 ){
    Exact_Force(BD);
  }
  
  else{
    for(i=0; i<N; i++){
      dynamic_forceMagic(octree, BD[i], BD, alpha);
    }
  }
    
  Velocity_Step_3(BD);
  Position_Step_4(BD);
  Reset_Accelerations(BD);

  collision_check  = 1;
  collision_number = 0;
  if( method == 0 ){
    Exact_Force(BD);
  }
  
  else{
    
    for(i=0; i<N; i++){
      dynamic_forceMagic(octree, BD[i], BD, alpha);
    }
  }
  collision_check = 0;
  Velocity_Step_Final(BD);
  Position_Step_Final(BD);
}
