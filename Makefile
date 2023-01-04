CXX=g++
CXXFLAGS=-DNDEBUG -O5 -std=c++17 -lpthread -Wl,--no-as-needed

SRC= *.cc

all: main

main:
	$(CXX) $(CXXFLAGS) $(SRC)

test:
	$(CXX) $(CXXFLAGS) -DTEST $(SRC) -o test -lcriterion

check: test
	./test --verbose

bench:
	$(CXX) $(CXXFLAGS) -DBENCH $(SRC) -o bench -lbenchmark

clean:
	$(RM) a.out *.o test bench

.PHONY: all test bench clean