import random 
import sys

n = int(sys.argv[1])   

f = open("date.in", "w+");

f.write(str(n)+"\n")

for i in range(n-1):
    f.write(str(random.randint(1,10000)))
    f.write(" ")

f.write("\n")

for i in range(n):
    f.write(str(random.randint(1,10000)))
    f.write(" ")

f.write("\n")

for i in range(n-1):
    f.write(str(random.randint(1,10000)))
    f.write(" ")

f.write("\n")

for i in range(n):
    f.write(str(random.randint(1,10000)))
    f.write(" ")

