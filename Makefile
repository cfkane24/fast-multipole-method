#https://www.cs.swarthmore.edu/~newhall/unixhelp/howto_makefiles.html
CC = g++
CFLAGS = -O2 -funroll-loops
INCLUDES = -I/include
LIBS = -lm
SRCS = collisions.c exactForce.c fmm.c force.c initialize.c \
	integrators.c systemProperties.c popRegions.c resetOctree.c timing.c vector.c
OBJS = $(SRCS:.c=.o)
MAIN = gravity

.PHONY: depend clean

all: $(MAIN)
	@echo COMPILATION SUCCESSFUL

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

#gravity : collisions.o exactForce.o fmm_timing.o test_timing_force.o force.o initialize.o \
	integrators.o systemProperties.o popRegions.o \
#	resetOctree.o timing.o vector.o wellSepCom.o
#	$(CC) $(CFLAGS) -lm -o gravity collisions.o exactForce.o fmm_timing.o test_timing_force.o force.o initialize.o \
		integrators.o systemProperties.o popRegions.o \
		resetOctree.o timing.o vector.o wellSepCom.o

#collisions.o : collisions.c vector.h planet.h
#	$(CC) $(CFLAGS) -c collisions.c

#exactForce.o: exactForce.c vector.h planet.h
#	$(CC) $(CFLAGS) -c exactForce.c

#fmm_timing.o : fmm_timing.c vector.h planet.h pln.h region.h collisions.h initialize.h exactForce.h \
		systemProperties.h integrators.h popRegions.h test_timing_force.h \
#		resetOctree.h timing.h
#	$(CC) $(CFLAGS) -c fmm_timing.c

#force.o : force.c vector.h planet.h pln.h region.h wellSepCom.h timing.h
#	$(CC) $(CFLAGS) -c force.c	

#test_timing_force.o : test_timing_force.c vector.h planet.h pln.h region.h wellSepCom.h timing.h
#	$(CC) $(CFLAGS) -c test_timing_force.c

#initialize.o : initialize.c vector.h planet.h
#	$(CC) $(CFLAGS) -c initialize.c

#integrators.o : integrators.c vector.h planet.h pln.h region.h test_timing_force.h force.h exactForce.h timing.h
#	$(CC) $(CFLAGS) -c integrators.c

#systemProperties.o : systemProperties.c vector.h planet.h
#	$(CC) $(CFLAGS) -c systemProperties.c

#popRegions.o : popRegions.c vector.h pln.h planet.h region.h
#	$(CC) $(CFLAGS) -c popRegions.c

#resetOctree.o : resetOctree.c pln.h vector.h planet.h region.h popRegions.h
#	$(CC) $(CFLAGS) -c resetOctree.c

#timing.o : timing.c 
#	$(CC) $(CFLAGS) -c timing.c

#vector.o : vector.c vector.h
#	$(CC) $(CFLAGS) -c vector.c

#wellSepCom.o : wellSepCom.c vector.h planet.h pln.h region.h timing.h
#	$(CC) $(CFLAGS) -c wellSepCom.c

#clean :
#	rm *.o
