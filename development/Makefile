
OUT=run

CC=g++-4.8
OPTIONS= -std=c++1y -pedantic-errors -Wall -Wconversion
INCLUDES=-I/usr/local/include -I/home/apelles/projects/

LIBS=-lpqxx -lpq -L/usr/local/lib -lconfig++

CODE=$(wildcard *.cpp)
CODE += ../../foam/strlib/strlib.cpp

all:
	$(CC) $(OPTIONS) $(CODE) -o $(OUT) $(INCLUDES) $(LIBS)
