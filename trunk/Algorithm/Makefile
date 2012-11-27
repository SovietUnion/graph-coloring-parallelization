test_algorithm: ParallelContractAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm.o
	g++ -pthread -fpermissive -o test_algorithm ParallelContractAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm.o
ContractAlgorithm.o: Algorithm.h ParallelContractAlgorithm.h
	g++ -c Test_Algorithm.cpp ParallelContractAlgorithm.cpp Algorithm.h ParallelContractAlgorithm.h
clean: 
	rm *.o test_algorithm
