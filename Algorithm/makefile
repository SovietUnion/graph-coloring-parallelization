algorithm: ContractAlgorithm.o ../Graph/MatrixGraph.o
	g++ -o algorithm ContractAlgorithm.o ../Graph/MatrixGraph.o
ContractAlgorithm.o: Algorithm.h ContractAlgorithm.h
	g++ -c ContractAlgorithm.cpp Algorithm.h ContractAlgorithm.h
clean: 
	rm *.o algorithm
