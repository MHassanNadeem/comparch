CC=g++
CFLAGS=-std=c++11

OBJECTS=cache.o memory.o prefetcher.o constStrideArrayPrefetcher.o varStrideArrayPrefetcher.o linkedListPrefetcher.o fileParser.o main.o
INCLUDES=$(wildcard *.h)
EXECUTABLE=main.bin

%.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)
	
.PHONY : clean
clean:
	rm -rf cache $(EXECUTABLE) $(OBJECTS)
	
run:
	clear && ./$(EXECUTABLE)
