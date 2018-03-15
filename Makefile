gravity : collisions.o displist.o exactForce.o fmm.o force.o getArrayIndex.o \
		initialize.o integrators.o intVec.o parentChild.o planetListFunctions.o \
	systemProperties.o populateRegions.o resetregion.o timing.o vector.o wellSepCoM.o
	g++ -o gravity collisions.o displist.o exactForce.o fmm.o force.o \
		getArrayIndex.o initialize.o integrators.o intVec.o parentChild.o \
		planetListFunctions.o systemProperties.o populateRegions.o \
		resetregion.o timing.o vector.o wellSepCoM.o

collisions.o : collisions.c vector.h planet.h
	g++ -c collisions.c

displist.o : displist.c vector.h planet.h pln.h region.h
	g++ -c displist.c

exactForce.o: exactForce.c vector.h planet.h
	g++ -c exactForce.c

fmm.o : fmm.c vector.h planet.h pln.h region.h force.h collisions.h initialize.h \
		exactForce.h populateRegions.h planetListFunctions.h resetregion.h \
		parentChild.h systemProperties.h integrators.h timing.h
	g++ -c fmm.c

force.o : force.c vector.h planet.h pln.h region.h intVec.h wellSepCoM.h displist.h \
          timing.h
	g++ -c force.c

getArrayIndex.o : getArrayIndex.c
	g++ -c getArrayIndex.c

initialize.o : initialize.c vector.h planet.h
	g++ -c initialize.c

integrators.o : integrators.c vector.h planet.h pln.h region.h force.h exactForce.h
	g++ -c integrators.c

intVec.o : intVec.c
	g++ -c intVec.c

omelyan.o : omelyan.c vector.h planet.h
	g++ -c omelyan.c

parentChild.o : parentChild.c vector.h pln.h region.h getArrayIndex.h planet.h
	g++ -c parentChild.c

planetListFunctions.o : planetListFunctions.c pln.h vector.h planet.h region.h displist.h
	g++ -c planetListFunctions.c

systemProperties.o : systemProperties.c vector.h planet.h
	g++ -c systemProperties.c

populateRegions.o : populateRegions.c getArrayIndex.h vector.h pln.h region.h planet.h
	g++ -c populateRegions.c

resetregion.o : resetregion.c vector.h pln.h planet.h region.h planetListFunctions.h
	g++ -c resetregion.c

timing.o : timing.c
	g++ -c timing.c

vector.o : vector.c
	g++ -c vector.c

wellSepCoM.o : wellSepCoM.c vector.h planet.h pln.h region.h
	g++ -c wellSepCoM.c

clean :
	rm *.o
