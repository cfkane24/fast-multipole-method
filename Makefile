gravity : collisions.o exactForce.o fmm.o force.o initialize.o \
	integrators.o systemProperties.o popRegions.o \
	resetOctree.o timing.o vector.o wellSepCom.o
	g++ -o gravity collisions.o exactForce.o fmm.o force.o initialize.o \
		integrators.o systemProperties.o popRegions.o \
		resetOctree.o timing.o vector.o wellSepCom.o

collisions.o : collisions.c vector.h planet.h
	g++ -c collisions.c

exactForce.o: exactForce.c vector.h planet.h
	g++ -c exactForce.c

fmm.o : fmm.c vector.h planet.h pln.h region.h collisions.h initialize.h exactForce.h \
		systemProperties.h integrators.h popRegions.h force.h \
		resetOctree.h timing.h
	g++ -c fmm.c

force.o : force.c vector.h planet.h pln.h region.h wellSepCom.h timing.h
	g++ -c force.c

initialize.o : initialize.c vector.h planet.h
	g++ -c initialize.c

integrators.o : integrators.c vector.h planet.h pln.h region.h force.h exactForce.h
	g++ -c integrators.c

systemProperties.o : systemProperties.c vector.h planet.h
	g++ -c systemProperties.c

popRegions.o : popRegions.c vector.h pln.h planet.h region.h
	g++ -c popRegions.c

resetOctree.o : resetOctree.c pln.h vector.h planet.h region.h popRegions.h
	g++ -c resetOctree.c

timing.o : timing.c 
	g++ -c timing.c

vector.o : vector.c vector.h
	g++ -c vector.c

wellSepCom.o : wellSepCom.c vector.h planet.h pln.h region.h
	g++ -c wellSepCom.c

clean :
	rm *.o
