# lib directory
ALGO = ./Algorithm
GRAPH = ./Graph

# complier flags
CC=g++
CFLAGS=-c
PTFLAGS=-pthread -fpermissive
TIMEFLAGS=-lrt

colour_graph: $(ALGO)/Algorithm.o $(ALGO)/ContractAlgorithm.o $(ALGO)/ParallelContractAlgorithm.o $(GRAPH)/MatrixGraph.o ColourGraph.o $(ALGO)/BSCAlgorithm.o
		$(CC) $(PTFLAGS) -o colour_graph $(ALGO)/Algorithm.o $(ALGO)/BSCAlgorithm.o $(ALGO)/ContractAlgorithm.o $(ALGO)/ParallelContractAlgorithm.o $(GRAPH)/MatrixGraph.o ColourGraph.o $(TIMEFLAGS)

ColourGraph.o: ColourGraph.cpp
	$(CC) $(CFLAGS) ColourGraph.cpp

ParallelAlgorithm.o: $(ALGO)/ParallelContractAlgorithm.cpp $(ALGO)/Algorithm.h $(ALGO)/ParallelContractAlgorithm.h
	$(CC) $(CFLAGS) $(ALGO)/ParallelContractAlgorithm.cpp

ContractAlgorithm.o:  $(ALGO)/ContractAlgorithm.cpp $(ALGO)/Algorithm.h $(ALGO)/ContractAlgorithm.h 
	$(CC) $(CFLAGS) $(ALGO)/ContractAlgorithm.cpp

BSCAlgorithm.o:  $(ALGO)/BSCAlgorithm.cpp $(ALGO)/BSCAlgorithm.h $(ALGO)/BSCAlgorithm.h 
	$(CC) $(CFLAGS) $(ALGO)/BSCAlgorithm.cpp

Algorithm.o:  $(ALGO)/Algorithm.cpp $(ALGO)/Algorithm.h  
	$(CC) $(CFLAGS) $(ALGO)/Algorithm.cpp

MatrixGraph.o: $(GRAPH)/MatrixGraph.cpp $(GRAPH)/Graph.h $(GRAPH)/MatrixGraph.h
	$(CC) $(CFLAGS) $(GRAPH)/MatrixGraph.cpp 


clean: 
	rm $(ALGO)/*.o $(GRAPH)/*.o *.o colour_graph


