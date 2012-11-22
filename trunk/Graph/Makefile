test_graph: MatrixGraph.o Test_Graph.o
	g++ -o test_graph MatrixGraph.o Test_Graph.o
MatrixGraph.o: Graph.h MatrixGraph.h 
	g++ -c Test_Graph.cpp MatrixGraph.cpp Graph.h MatrixGraph.h
clean: 
	rm *.o test_graph
