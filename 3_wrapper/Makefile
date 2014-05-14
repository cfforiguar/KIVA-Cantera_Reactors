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

SRCS=cppwrapper.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

#all: test

test: $(OBJS)
	$(FC) $(LDFLAGS) -o test $(OBJS) fwrapper.f -lc -lstdc++
	

%.o: %.cpp
	$(CXX) $(LDFLAGS) -c $<  

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ 
