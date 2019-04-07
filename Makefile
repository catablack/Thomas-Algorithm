all:
	g++ -std=c++11 -fopenmp parallel.cpp -o exec
clean:
	rm -rf exec
