test_graph: MatrixGraph.o test_Graph.o
	g++ -o test_Graph MatrixGraph.o test_Graph.o
MatrixGraph.o: Graph.h MatrixGraph.h 
	g++ -c test_Graph.cpp MatrixGraph.cpp Graph.h MatrixGraph.h
clean: 
	rm *.o test_graph
