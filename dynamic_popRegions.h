void dynamic_recurse_divide_by_mass(dynamic_region &r);
int inThisChild(vector planet_pos, vector region_pos, double region_size);
vector assignLocation(int i, dynamic_region currentRegion);
void addKids(dynamic_region &currentRegion);
void addPlanet(int num, pln **list);
void loopOverPlanets(planet BD[]);
void pop_level_0(dynamic_region &octree, planet BD[]);
void loopOverRegions(dynamic_region current_region, planet BD[]);
