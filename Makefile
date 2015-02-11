.PHONY: clean all

CC = g++
CFLAGS = -Wall -g3 -pedantic -std=c++0x -std=c99


all:driver

driver: driver.o implementation.o
	$(CC) $^ $(LFLAGS) -o $@  

driver.o: driver.cpp
	$(CC) $(CFLAGS) $^ -c

implementation.o: implementation.cpp
	$(CC) $(CFLAGS) $^ -c

distclean:
	$(RM) client server library

clean:
	$(RM) *.o
