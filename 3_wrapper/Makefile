include  ~/.local/include/cantera/Cantera.mak #Cantera
ifneq ($(BASEMK_INCLUDED),YES)
include $(PFUNIT)/include/base.mk             #Tests unitarios
endif


#Parte de F90
FC= ifort -g -O0

#Parte de c++
CC=icc
CXX=icpc
RM=rm -f
CCFLAGS=-g
CPPFLAGS=$(CANTERA_INCLUDES)
LDFLAGS= # -fopenmp
LDLIBS=$(CANTERA_LIBS)

SRCScpp=cppwrapper.cpp
SRCSf90=fwrapper.f


all: test

#Generar los F90 de los tests unitarios
%.F90: %.pf
	$(PFUNIT)/bin/pFUnitParser.py $<  $@

#Genera el ejecutable de Cantera
#test: $(OBJS)
#	$(FC) $(LDFLAGS) -o test $(OBJS)  -lc -lstdc++ $(LDLIBS) 

#Compila los .o de Cantera	
%.o : %.f90
	$(F90) -c $<

%.o: %.cpp
	$(CXX) $(LDFLAGS) -c $< $(LDLIBS) $(CANTERA_INCLUDES)

#Compila los .o de pfunit
%.o : %.F90
	$(FC) -c $< -I$(PFUNIT)/mod
SRCS = $(wildcard *.F90)

#Añade el driver de pfunit
DRIVER = $(PFUNIT)/include/driver.F90 


#Junta los objetos ya compilados
OBJS= $(SRCS:.F90=.o) $(TESTS:.pf=.o) $(subst .f90,.o,$(SRCSf90)) $(subst .cpp,.o,$(SRCScpp))


#Linkear el ejecutable del test unitario
tests.x: $(DRIVER) $(OBJS) testSuites.inc
	$(FC) -o $@  -I$(PFUNIT)/mod $(DRIVER) $(OBJS) -L$(PFUNIT)/lib -lpfunit -I.

tests: tests.x
	./tests.x

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ 
