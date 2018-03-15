#include "vector.h"
#include "pln.h"
#include "planet.h"
#include "region.h"
#include "planetListFunctions.h"

extern int LVL;
extern region *regions;
void reset_region(region& r)
{
  // if we are empty, we don't need to do anything
  if (r.numPln == 0) return;

  // otherwise, clear ourselves, then iterate down to the chilluns
  r.mass=0;
  r.com.x=r.com.y=r.com.z=0;
  r.numPln=0;
  clearList(&(r.planets));
  if (r.level != LVL-1)
    for (int i=0;i<8;i++)
    {
      reset_region(regions[r.kids[i]]);
    }
}
