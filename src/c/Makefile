#
# Hierarchical Affinity Propagation
#

EXE = affinity_propagation

OBJS = affinity_propagation.o \
       csv.o \
       ap.o

CXXFLAGS = -O3

CXX = g++

# Additional libraries
ADDLIBS =

# Additional flags for compilation (e.g., include flags)
ADDINCFLAGS =

# additional C++ Compiler options for linking
CXXLINKFLAGS =  -Wl,--rpath -Wl

# Necessary Include dirs (we use the CYGPATH_W variables to allow
# compilation with Windows compilers)
INCL =  -I`$(CYGPATH_W) $(IPOPTINCDIR)` $(ADDINCFLAGS)

CYGPATH_W = echo

all: $(EXE)

.SUFFIXES: .cpp .c .o .obj

$(EXE): $(OBJS)
	bla=;\
	for file in $(OBJS); do bla="$$bla `$(CYGPATH_W) $$file`"; done; \
	$(CXX) $(CXXFLAGS) -o $@ $$bla $(ADDLIBS) $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCL) -c -o $@ $<


clean:
	rm -rf $(OBJS) $(EXE)
