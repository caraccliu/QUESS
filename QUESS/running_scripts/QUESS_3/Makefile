
MAKE=make
CXX =g++ -O3 -Wall  -fomit-frame-pointer -fopenmp
#CXX=clang -O3 -Wall -fomit-frame-pointer -Xclang -ast-dump -Xclang  -fopenmp=libiomp5
#CXX=g++ -O3 -Wall -fomit-frame-pointer

QUESS: QUESS.o hashTable.o read.o seeds.o
	$(CXX) QUESS.o hashTable.o read.o seeds.o -o $@

QUESS.o : QUESS.cpp QUESS.h
	$(CXX) -c QUESS.cpp -o $@

hashTable.o: hashTable.cpp QUESS.h
	$(CXX) -c hashTable.cpp -o $@

read.o: read.cpp QUESS.h
	$(CXX) -c read.cpp -o $@

seeds.o: seeds.cpp QUESS.h
	$(CXX) -c seeds.cpp -o $@

clean:
	rm -f *.o
	rm -f QUESS
