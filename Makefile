CXX = g++
CXXFLAGS = -std=c++11 -O2

all: mbhs mbhc snh

mbhs: MBHS.o Graph.o
	$(CXX) $(CXXFLAGS) -o mbhs MBHS.o Graph.o

mbhc: MBHC.o Graph.o
	$(CXX) $(CXXFLAGS) -o mbhc MBHC.o Graph.o

snh: SNH.o Graph.o
	$(CXX) $(CXXFLAGS) -o snh SNH.o Graph.o

Graph.o: Graph.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c Graph.cpp

MBHS.o: MBHS.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c MBHS.cpp

MBHC.o: MBHC.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c MBHC.cpp

SNH.o: SNH.cpp Graph.h
	$(CXX) $(CXXFLAGS) -c SNH.cpp

clean:
	rm -f *.o mbhs mbhc snh
