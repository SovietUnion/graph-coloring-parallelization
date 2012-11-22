test_algorithm: ContractAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm.o
	g++ -o test_algorithm ContractAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm.o
ContractAlgorithm.o: Algorithm.h ContractAlgorithm.h
	g++ -c Test_Algorithm.cpp ContractAlgorithm.cpp Algorithm.h ContractAlgorithm.h
clean: 
	rm *.o test_algorithm
