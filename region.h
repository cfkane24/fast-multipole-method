typedef struct region_placeholder
{
  int level;//constant
  int numPln;//dynamic
  double size;//constant
  double mass;//dynamic
  vector com;//dynamic
  vector location;//constant
  pln *planets;//dynamic
  region_placeholder *child[8];//dynamic
} region;

extern region octree;
