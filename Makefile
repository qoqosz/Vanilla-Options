CXX=clang++
CXXFLAGS=-std=c++11 -Wall -pedantic -O3
BIN=vanilla

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

QL_CXXFLAGS=-I/Users/qoqosz/local/include -I/usr/local/include
QL_LDFLAGS=-L/Users/qoqosz/local/lib -L/usr/local/lib 
QL_LDLIBS=-lQuantLib


all: $(OBJ) $(QL_OBJ)
	$(CXX) $(QL_LDFLAGS) $(QL_LDLIBS)  -o $(BIN) $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(QL_CXXFLAGS) -o $@ -c $<

quantlib_mc.o: quantlib_mc.cpp quantlib_mc.hpp
	$(CXX) $(CXXFLAGS) $(QL_CXXFLAGS) -o $@ -c $<

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)