This project is the algorithm about "CPT-Fast", written in C++.
Here is the structure.
/..................
	README.txt
	code/..........
		Makefile
		decpToKR.cpp
		.
		.
		.
		tool/........
			.
			.

All codes about the algorithm is in "/code/".
Some codes in "/code/tool/" are about debug and list operation.
The main function is in "/code/decpToKR.cpp".

######compile
First put the whole folder into a Linux server (I use CentOS 6.10 (64-bit)).
Then enter the "/code/" folder.
Finally enter the command "make".

Several files will be generated.
The file "CPT-Fast.out" generated is the program file.
The version of g++ is "gcc version 4.4.7 20120313 (Red Hat 4.4.7-23) (GCC)".

######run
You can copy "CPT-Fast.out" to "/" folder and run it.
Here is the command:
./CPT-Fast.out ${dataFile} ${resultPath} ${dataLabel}

For example, if we enter the follow command:
./CPT-Fast.out ./datasets/Fruit-Fly.txt ./index/CPT-Fast/ CPT-Fast-Fruit-Fly

the program will generate a series of files under the folder "./index/".
Here the first parameter "${dataFile}" is the location and the file name of the dataset.
The second parameter "${resultPath}" is the path where the result will be saved.
The third parameter "${dataLabel}" is the name of dataset, which is used to identify the index generated.

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