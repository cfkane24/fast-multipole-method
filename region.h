//#include "vector.h"

typedef struct name
{
  int level;
  int i,j,k;
  int numPln;
  double mass;
  vector com;
  pln *planets;
  int kids[8];//FIXME Will be a list of eight pointers b/c eight kids
} region;

extern region *regions;
