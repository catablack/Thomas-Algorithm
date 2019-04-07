all:
        g++ -std=c++11 -fopenmp *.cpp -o exec
clean:
        rm -rf exec