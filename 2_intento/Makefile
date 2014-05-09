include /home/carlos/.local/include/cantera/Cantera.mak

CC=icc
CXX=icpc
RM=rm -f
CCFLAGS=-g
CPPFLAGS=$(CANTERA_INCLUDES)
LDFLAGS=
LDLIBS=$(CANTERA_LIBS)

SRCS=combustor.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: combustor

combustor: $(OBJS)
	$(CXX) $(LDFLAGS) -o combustor $(OBJS) $(LDLIBS)

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ 
