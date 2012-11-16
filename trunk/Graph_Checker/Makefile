graphchecker: GraphChecker.o ../Graph/MatrixGraph.o
	g++ -o graphchecker GraphChecker.o ../Graph/MatrixGraph.o
GraphChecker.o: GraphChecker.h
	g++ -c GraphChecker.cpp
clean: 
	rm *.o graphchecker
