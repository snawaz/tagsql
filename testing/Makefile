
OUT=run

CC=g++-4.8
OPTIONS= -std=c++1y -pedantic-errors -Wall -Wconversion -Wno-unused-variable
INCLUDES=-I/usr/local/include -I../.. -I..

LIBS=-lpqxx -lpq -L/usr/local/lib -lconfig++

CODE=$(wildcard *.cpp)
CODE += ../../foam/strlib/strlib.cpp

all:
	$(CC) $(OPTIONS) $(CODE) -o $(OUT) $(INCLUDES) $(LIBS)
