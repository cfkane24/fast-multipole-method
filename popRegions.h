void clearList(pln **list);
void recurse_divide_by_mass(region &r);
int inThisChild(vector planet_pos, vector region_pos, double region_size);
vector assignLocation(int i, region currentRegion);
void addKids(region &currentRegion);
void addPlanet(int num, pln **list);
void loopOverPlanets(planet BD[]);
void pop_level_0(region &octree, planet BD[]);
void loopOverRegions(region &current_region, planet BD[]);
