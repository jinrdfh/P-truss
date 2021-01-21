file = file.o file.h
common = common.h tool/debug.h tool/myList.h graph.h
deTruss = deTruss.o deTruss.h
calPek = calPek.o calPek.h
delE = delE.o delE.h
myG = myG.o myG.h

total: decpToKR
	
decpToKR: decpToKR.o $(file) $(common) $(deTruss) $(calPek) $(delE) $(myG)
	g++ -rdynamic -o CPT-Fast.out decpToKR.o file.o deTruss.o calPek.o delE.o myG.o

getKMax: getKMax.o $(file) $(common) $(deTruss) $(calPek) $(delE) $(myG)
	g++ -rdynamic -o getKMax getKMax.o file.o deTruss.o calPek.o delE.o myG.o
	
decpToKR.o: decpToKR.cpp file.h $(common) deTruss.h calPek.h delE.h
	g++ -rdynamic -c decpToKR.cpp

getKMax.o: getKMax.cpp file.h $(common) deTruss.h calPek.h delE.h
	g++ -rdynamic -c getKMax.cpp
	
file.o: file.cpp $(common) file.h delE.h myG.h deTruss.h
	g++ -rdynamic -c file.cpp
	
show.o: show.cpp $(common) deTruss.h myG.h
	g++ -rdynamic -c show.cpp
	
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