include /home/carlos/.local/include/cantera/Cantera.mak

CC=icc
CXX=icpc -g -O0 
RM=rm -f
CCFLAGS=-g
CPPFLAGS=$(CANTERA_INCLUDES)
LDFLAGS=
LDLIBS=$(CANTERA_LIBS)

SRCS=WSR_KIVA.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: WSR_KIVA

combustor: $(OBJS)
	$(CXX) $(LDFLAGS) -g -o WSR_KIVA $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ 
