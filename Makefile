CXX = g++
CXXFLAGS = -std=c++17 -O2

all: mbhs mbhc

mbhs: MBHS.o Graph.o
	$(CXX) $(CXXFLAGS) -o mbhs MBHS.o Graph.o

mbhc: MBHC.o Graph.o
	$(CXX) $(CXXFLAGS) -o mbhc MBHC.o Graph.o

Graph.o: Graph.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c Graph.cpp

MBHS.o: MBHS.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c MBH_S.cpp

MBHC.o: MBHC.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c MBH_C.cpp

clean:
	rm -f *.o mbhs mbhc
