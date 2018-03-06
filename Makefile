CC=g++
CFLAGS=-std=c++11

%.o: %.cpp %.h
	@$(CC) $(CFLAGS) -c -o $@ $<

all: cache.o memory.o prefetcher.o constStrideArrayPrefetcher.o
	$(CC) $(CFLAGS) main.cpp cache.o prefetcher.o memory.o constStrideArrayPrefetcher.o -o main.bin
	
.PHONY : clean
clean:
	rm -rf cache main.bin cache.o memory.o prefetcher.o
	
run:
	clear && ./main.bin