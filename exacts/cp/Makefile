CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio129/concert
CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio129/cplex
CPOPTIMIZERDIR = /opt/ibm/ILOG/CPLEX_Studio129/cpoptimizer
CCC = g++
CFLAGS = -DIL_STD -O3 -DNDEBUG -I$(CPOPTIMIZERDIR)/include -I$(CONCERTDIR)/include -fPIC -fstrict-aliasing -pedantic -Wall -Wextra -fexceptions -frounding-math -Wno-long-long -m64 -DILOUSEMT -D_REENTRANT -DILM_REENTRANT
LDFLAGS = -L$(CPOPTIMIZERDIR)/lib/x86-64_linux/static_pic -lcp -L$(CPLEXDIR)/lib/x86-64_linux/static_pic -lcplex -L$(CONCERTDIR)/lib/x86-64_linux/static_pic -lconcert  -lpthread -lm -ldl

make:
	$(CCC) -o main.out $(CFLAGS) main.cpp $(LDFLAGS)