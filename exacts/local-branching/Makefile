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


make: cplex.o main.cpp
	g++ $(CCFLAGS) $(CCLNDIRS) main.cpp cplex.o -o main.out $(CCLNFLAGS)

make2: main2.cpp
	g++ $(CCFLAGS) $(CCLNDIRS) main2.cpp -o main2.out $(CCLNFLAGS)

cplex.o: ../cplex/cplex.cpp ../cplex/cplex.h
	g++ -c $(CCFLAGS) ../cplex/cplex.cpp -o cplex.o