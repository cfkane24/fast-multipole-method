typedef struct region_placeholder
{
  int level;
  int i,j,k;
  int numPln;
  double mass;
  vector com;
  pln *planets;
  region_placeholder *child[8];//array of pointers that point to a variable of type region
  /* 
     the last entry is an array of pointers.
     each pointer points to the child of the region.
     to find the children, now all we have to do is 
     go to the end of this array. Each child will
     also have its own array pointing to its children.
     So to recurse down, just follow pointers.
  */
     
} region;

extern region *regions;
  
