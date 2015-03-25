#### make test ####
SHELL=/bin/sh

CXXFLAGS=-Wall -std=c++11 -O3 -I. -Ismasher
LDFLAGS=-lm

OBJECTS=smasher/qsort_smasher.o test.o

test: $(OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -fv $(OBJECTS) test core
