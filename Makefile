all:
	g++ -std=c++11 cache.cpp -o cache
	
clean:
	rm -rf cache
	
run:
	./cache