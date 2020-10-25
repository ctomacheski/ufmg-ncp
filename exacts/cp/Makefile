CPLEXDIR  = /Applications/CPLEX_Studio1210/cplex
CONCERTDIR = /Applications/CPLEX_Studio1210/concert
CPOPTIMIZERDIR = /Applications/CPLEX_Studio1210/cpoptimizer
CCC = g++
SYSTEM = x86-64_osx
CFLAGS = -DIL_STD -O3 -DNDEBUG -I$(CPOPTIMIZERDIR)/include -I$(CONCERTDIR)/include -fPIC -fstrict-aliasing -pedantic -Wall -Wextra -fexceptions -frounding-math -Wno-long-long -m64 -DILOUSEMT -D_REENTRANT -DILM_REENTRANT
LDFLAGS = -L$(CPOPTIMIZERDIR)/lib/$(SYSTEM)/static_pic -lcp -L$(CPLEXDIR)/lib/$(SYSTEM)/static_pic -lcplex -L$(CONCERTDIR)/lib/$(SYSTEM)/static_pic -lconcert  -lpthread -lm -ldl

make:
	$(CCC) -o main.out $(CFLAGS) main.cpp $(LDFLAGS)
