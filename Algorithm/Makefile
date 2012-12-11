test_algorithm: Algorithm.o BSCAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm_BSC.o
	g++ -pthread -fpermissive -o test_algorithm Algorithm.o BSCAlgorithm.o ../Graph/MatrixGraph.o Test_Algorithm_BSC.o
BSCAlgorithm.o: Algorithm.h BSCAlgorithm.h
	g++ -c Test_Algorithm_BSC.cpp BSCAlgorithm.cpp Algorithm.h BSCAlgorithm.h
Algorithm.o: Algorithm.h
	g++ -c Algorithm.cpp
clean: 
	rm ../Graph/*.o *.o test_algorithm
