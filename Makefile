CC=g++
CFLAGS=-std=c++11

%.o: %.cpp
	@$(CC) $(CFLAGS) -c -o $@ $<

all: cache.o
	$(CC) $(CFLAGS) main.cpp cache.o -o main
	
.PHONY : clean
clean:
	rm -rf cache main
	
run:
	./main