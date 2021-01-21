This project is the algorithm about "(𝜖, Δ𝑟 )-APX", written in C++.
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
The file "APX.out" generated is the program file.
The version of g++ is "gcc version 4.4.7 20120313 (Red Hat 4.4.7-23) (GCC)".

######run
You can copy "APX.out" to "/" folder and run it.
Here is the command:
./APX.out ${dataFile} ${resultPath} ${miniP} ${accu} ${kSpace} ${dataLabel}

For example, if we enter the follow command:
./APX.out ./datasets/Fruit-Fly.txt ./index/APX/ 0.1 0.001 1 APX-Fruit-Fly

the program will generate a series of files under the folder "./index/".
Here the first parameter "${dataFile}" is the location and the file name of the dataset.
The second parameter "${resultPath}" is the path where the result will be saved.
The third parameter "${miniP}" is the pruning probability \epsilon.
The forth parameter "${accu}" is the approximation ratio \Delta_r.
The fifth parameter "${kSpace}" is the interval of k, which is the function that we are studying and is not mentioned in the paper. This can be ignored by just set it as "1".
The sixth parameter "${dataLabel}" is the name of dataset, which is used to identify the index generated.

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