include /home/carlos/.local/include/cantera/Cantera.mak

#Parte de F90
FC= ifort -g -O0

#Parte de c++
CC=icc
CXX=icpc
RM=rm -f
CCFLAGS=-g
CPPFLAGS=$(CANTERA_INCLUDES)
LDFLAGS=  
LDLIBS=$(CANTERA_LIBS)

SRCScpp=cppwrapper.cpp
SRCSf90=fwrapper.f
OBJS=$(subst .f90,.o,$(SRCSf90)) $(subst .cpp,.o,$(SRCScpp))

all: test

test: $(OBJS)
	$(FC) $(LDFLAGS) -o test $(OBJS)  -lc -lstdc++ $(LDLIBS) 
	
%.o : %.f90
	$(F90) -c $<

%.o: %.cpp
	$(CXX) $(LDFLAGS) -c $< $(LDLIBS) $(CANTERA_INCLUDES)

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ 
