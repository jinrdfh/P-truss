This project is the query algorithm about "CPT-Basic" and "CPT-Fast", written in C++.
Here is the structure.
/..................
	README.txt
	code/..........
		Makefile
		getGraph.cpp
		.
		.
		.
		tool/........
			.
			.

All codes about the algorithm is in "/code/".
Some codes in "/code/tool/" are about debug and list operation.
The main function is in "/code/getGraph.cpp".

######compile
First put the whole folder into a Linux server (I use CentOS 6.10 (64-bit)).
Then enter the "/code/" folder.
Finally enter the command "make".

Several files will be generated.
The file "CPTQuery.out" generated is the program file.
The version of g++ is "gcc version 4.4.7 20120313 (Red Hat 4.4.7-23) (GCC)".

######run
Here is the command:
./CPTQuery.out ${resultPath} ${dataFile} ${dataLabel} ${queryFile} ${indexPath}

For example, if we want to query "CPT-Basic index", we enter the follow command:
./CPTQuery.out ./result ./datasets/Fruit-Fly.txt CPT-Basic-Fruit-Fly ./query.txt ./index/CPT-Basic

If we want to query "CPT-Fast index", we enter the follow command:
./CPTQuery.out ./result ./datasets/Fruit-Fly.txt CPT-Fast-Fruit-Fly ./query.txt ./index/CPT-Fast

the program will generate a series of files under the folder "./result/".
The first parameter "${resultPath}" is the path where the result will be saved.
The second parameter "${dataFile}" is the location and the file name of the dataset.
The third parameter "${dataLabel}" is the name of dataset, which is used to identify the index under the folder "./index/".
The forth parameter "${queryFile}" is the location and the file name of the query.
Here we put queries in the file for querying in batch.
The fifth parameter "${indexPath}" is the path where the index locates.

######query file format
k,\gamma
.
.
.
where (k,\gamma)-truss is required.
For example, 
.
.
.
4,0.754478
3,0.661324
5,0.098194
.
.
.



######dataset format
u,v,p
.
.
.

where "u" and "v" are nodes id and p is the probability.
For example, 
.
.
.
31,3709,0.191
32,2386,0.226
33,1298,0.196
.
.
.