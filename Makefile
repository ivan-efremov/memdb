CXX      = g++
CXXFLAGS = -ggdb -Wall -pthread -std=c++11
LDFLAGS  = -ggdb

all: test


test: db.o main.o
	$(CXX) $(LDFLAGS) db.o main.o -o test

db.o: db.cpp
	$(CXX) $(CXXFLAGS) -c db.cpp -o db.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

.PHONY: clean
clean:
	-rm -rf *.o test
