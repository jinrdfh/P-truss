file = file.o file.h
common = common.h tool/debug.h tool/myList.h graph.h myG.h
delE = delE.o delE.h

total: getGraph

getGraph: getGraph.o $(common) $(file) $(delE)
	g++  -rdynamic -o CPTQuery.out getGraph.o file.o delE.o
	
getGraph.o: getGraph.cpp $(common) file.h delE.h
	g++ -rdynamic -c getGraph.cpp

file.o: file.cpp $(common) file.h delE.h
	g++ -rdynamic -c file.cpp
	
delE.o: delE.cpp delE.h $(common) file.h
	g++ -rdynamic -c delE.cpp
	
	
clean:
	rm *.o