#include "vector.h"
#include "planet.h"

extern int N;
extern double G;

void Exact_Force( planet BD[] ){

  int i,j;
  double rad;
  vector F;

  for( i=0; i<N; i++ ){
    for( j=i+1; j<N; j++){

      rad = radius( BD[i].pos, BD[j].pos );
      F   = G * BD[j].m * BD[i].m * (BD[j].pos-BD[i].pos) / (rad*rad*rad);
      BD[i].acc = BD[i].acc + F/BD[i].m;
      BD[j].acc = BD[j].acc - F/BD[j].m;
    }
  }
}
