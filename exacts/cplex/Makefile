CCOPT = -m64 -O3 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -DIL_STD -std=c++11
CPLEXDIR  = /Applications/CPLEX_Studio1210/cplex
CONCERTDIR = /Applications/CPLEX_Studio1210/concert
SYSTEM = x86-64_osx
LIBFORMAT = static_pic
CPLEXLIB = cplex$(dynamic:yes=1290)

CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXINCDIR = $(CPLEXDIR)/include
CONCERTINCDIR = $(CONCERTDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
CCLNDIRS = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl

GCC_FLAGS=-std=c++11 -O3

make: pre-processing.o subset.o cplex.o main.cpp
	g++ $(CCFLAGS) $(CCLNDIRS) main.cpp cplex.o subset.o pre-processing.o -o main.out $(CCLNFLAGS)

cplex.o: cplex.cpp cplex.h
	g++ -c $(CCFLAGS) cplex.cpp -o cplex.o

pre-processing.o: ../../util/pre-processing.cpp ../../util/pre-processing.h
	g++ $(GCC_FLAGS) -c ../../util/pre-processing.cpp

subset.o: ../../util/subset.cpp ../../util/subset.h
	g++ $(GCC_FLAGS) -c ../../util/subset.cpp
