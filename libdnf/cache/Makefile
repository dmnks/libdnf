CC = g++
CFLAGS = -Wall -g -std=c++11
LDFLAGS = $(shell pkg-config --cflags --libs glib-2.0)

all: hst

hashstore.o: hashstore.hpp hashstore.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -c $?

hst: hashstore.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

clean:
	rm -f hst hashstore.o hashstore.hpp.gch
