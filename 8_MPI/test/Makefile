include /home/carlos/.local/include/cantera/Cantera.mak

F90=mpiifort
RM=rm -f
F90FLAGS=-g
FORTRANMODDIRPREFIX=-module
F90MODDIR=$(CANTERA_FORTRAN_MODS)
LDFLAGS=
LDLIBS=$(CANTERA_FORTRAN_LIBS)

SRCS=test.f90
OBJS=$(subst .f90,.o,$(SRCS))

all: demo

demo: $(OBJS)
	$(F90) $(LDFLAGS) -o demo $(OBJS) $(LDLIBS)

%.o : %.f90
	$(F90) -c $< $(FORTRANMODDIRPREFIX) $(F90MODDIR) $(F90FLAGS)

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~
