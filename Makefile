CXX=g++
CXXFLAGS=-DNDEBUG -O5 -std=c++17 -lpthread -Wl,--no-as-needed

SRC= *.cc

all: main

main:
	$(CXX) $(CXXFLAGS) $(SRC)

clean:
	$(RM) a.out *.o