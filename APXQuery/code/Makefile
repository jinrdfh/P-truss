file = file.o file.h
common = common.h tool/debug.h tool/myList.h graph.h
deTruss = deTruss.o deTruss.h
calPek = calPek.o calPek.h
delE = delE.o delE.h
myG = myG.o myG.h

total: getGraph

getGraph: getGraph.o $(common) $(file) $(delE) $(calPek) $(deTruss) $(myG)
	g++  -rdynamic -o APXQuery.out getGraph.o file.o delE.o calPek.o deTruss.o myG.o
	
getGraph.o: getGraph.cpp $(common) file.h delE.h deTruss.h myG.h
	g++ -rdynamic -c getGraph.cpp

file.o: file.cpp $(common) file.h delE.h myG.h
	g++ -rdynamic -c file.cpp
	
deTruss.o: deTruss.cpp $(common) deTruss.h myG.h
	g++ -rdynamic -c deTruss.cpp
	
calPek.o: calPek.cpp calPek.h deTruss.h $(common) myG.h
	g++ -rdynamic -c calPek.cpp
	
delE.o: delE.cpp delE.h $(common) deTruss.h calPek.h file.h myG.h
	g++ -rdynamic -c delE.cpp
	
myG.o: myG.cpp myG.h $(common)
	g++ -rdynamic -c myG.cpp
	
clean:
	rm *.o