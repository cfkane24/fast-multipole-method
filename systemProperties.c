#include "vector.h"
#include "planet.h"

extern int N;
extern double G;

/**********************************************/

double Energy( planet BD[] ){

  double KE = 0,PE = 0;
  int i,j;
  
  for( i=0; i<N; i++ ){
    KE += 0.5 * BD[i].m * ( BD[i].vel * BD[i].vel );
  }

  for( i=0; i<N; i++){
    for( j=i+1; j<N; j++ ){
      PE += BD[i].m * BD[j].m / radius( BD[i].pos, BD[j].pos );
    }
  }
  return KE - G*PE;
}

/*********************************************/

double Momentum( planet BD[] ){

  int i;
  vector p;
  p.x = 0;
  p.y = 0;
  p.z = 0;
  
  for( i=0; i<N; i++ ){
    p = p + BD[i].m * BD[i].vel;
  }

  return Magnitude( p );
}

/********************************************/

double Mass( planet BD[] ){
  
  int i;
  double mass = 0.0;
  for( i=0; i<N; i++ ){
    mass += BD[i].m;
  }
  return mass;
}
