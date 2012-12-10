test_algorithm: Algorithm.o ParallelBSCAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm2.o
	g++ -pthread -fpermissive -o test_algorithm Algorithm.o ParallelBSCAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm2.o
ParallelBSCAlgorithm.o: Algorithm.h ParallelBSCAlgorithm.h
	g++ -c Test_Algorithm2.cpp ParallelBSCAlgorithm.cpp Algorithm.h ParallelBSCAlgorithm.h
Algorithm.o: Algorithm.h
	g++ -c Algorithm.cpp
clean: 
	rm ../Graph/*.o *.o test_algorithm
