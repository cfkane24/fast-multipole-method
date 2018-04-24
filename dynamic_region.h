typedef struct region_placeholder
{
  int level;
  int numPln;
  double size;
  double mass;
  vector com;
  vector location;
  pln *planets;
  region_placeholder *child[8];
} dynamic_region;

extern dynamic_region octree;
