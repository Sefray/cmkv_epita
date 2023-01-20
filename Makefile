CXX=g++
CXXFLAGS=-DNDEBUG -O5 -std=c++17 -lpthread -Wl,--no-as-needed

SRC= *.cc

all: main test bench

main:
	$(CXX) $(CXXFLAGS) $(SRC)

test:
	$(CXX) $(CXXFLAGS) -DTEST $(SRC) -o test -lcriterion

check: test
	./test --verbose --jobs=1

bench:
	$(CXX) $(CXXFLAGS) -DBENCH $(SRC) -o bench -lbenchmark

clean:
	$(RM) a.out *.o test bench

.PHONY: all test bench clean
