# fast-multipole-method

This code implements the fast multipole method to study N bodies interacting via Newton's Law of Gravitation. The code follows work done in (insert citation here). The file fmm.c (short for fast multipole method) is where the main function is. 

Authors:

Walter Freeman (Syracuse University)

Aaron Hutchins (Syracuse University)

Christopher Kane (SUNY ESF)

Meghan Lentz (Syracuse University)

Patrick Miles (Syracuse University)

*****************************
**     OUTLINE OF CODE     **
*****************************

*****************************
collisions.c

Description: This code contains two possibilities for collisions, perfectly elastic and perfectly innelastic. It is currently set that 50% of the collisions are perfectly elastic and 50% are perfectly inelastic. What determines this will be updated in the future. Collisions that involve bodies breaking into multiple fragments will also be added.

*****************************
exactForce.c

Description: Calculates the acceleration each planet feels using the naive Newton's Law of gravity. Performs all N*(N-1) calculations.

*****************************
fmm.c

Description: This code contains the main file. It initializes everything then runs a for loop over time running the algorithm. It also prints out the text to be fed into anim.

*****************************
force.c

Description: This code implements the fast multipole method to calculate the acceleration each planet feels. 

*****************************
initialize.c

Description: This function give initial positions, velocities, masses, and sizes of the planets. It also puts them all in the center of mass frame.

*****************************
integrators.c

Description: File that includes the leapfrog and omelyan integrators.

*****************************
popRegions.c

Description: Finds which planets are in which region. Also determines the properties of each region including the number of planets, the total mass, and the center of mass.

*****************************
resetOctree.c

Description: Frees memory of the octree except for the level zero region.

*****************************
systemProperties.c

Description: Contains functions to determine the total energy, momentum, and mass of the system of planets.

*****************************
timing.c 

Description: Contains functions to measure timing of any part of our code. Must be inserted manually.

*****************************
vector.c

Description: Contains the overloaded operators for the vector data type.

*****************************
wellSepCom.c

Description: Determines how many box lengths away a region is from a planet. This is passed to the force.c file to determine if it should do the multipole calculation or recurse deeper in the tree.







