#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <iomanip>
#include <fstream>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	
	int * totalPerLetter = new int[26];
	ifstream fin("Shakespeare.txt");
	fin.seekg (0, fin.end);
	int textFileSize = fin.tellg();
	
	
	
	
	fin.clear();
	fin.seekg (0, fin.beg);
		
	char * a = new char[textFileSize];

	fin.get(a,textFileSize);
	fin.close(); //close anyway to be safe
	
	
	// DIVIDE THE PROBLEM UP
	int localTextFileSize;

	localTextFileSize = (textFileSize/p)+2;
	
	char * locala = new char[localTextFileSize];
	int * localTotalPerLetter = new int[26];
		
	for (int i =0; i<26; i++){
		localTotalPerLetter[i] =0;
		totalPerLetter[i] =0;
	}
	
	MPI_Scatter(&a[0], localTextFileSize, MPI_CHAR, locala, localTextFileSize, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	// DO THE LOCAL WORK
	for(int n = 0; n < localTextFileSize; n++)
	{
		char oneLetter = char( locala[n] ); // get a letter
		if (oneLetter >= 'a' && oneLetter <='z') {
			localTotalPerLetter[int(oneLetter)- 97]++;
		}
	}

	
	// COLLECT ALL ANSWERS BACK TOGETHER IN A HAPPY ENDING
	MPI_Reduce(localTotalPerLetter, totalPerLetter, 26, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (my_rank == 0) {
		for (int x =0; x<26; x++){
			cout << char(97+x) << " " <<totalPerLetter[x]<<endl;
		}
	}
	
	delete [] a;
	delete [] totalPerLetter;
	delete [] locala;
	delete [] localTotalPerLetter;
	
 	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/*

output:

a 2784
b 744
c 690
d 1088
e 4395
f 619
g 885
h 2037
i 2565
j 115
k 547
l 1628
m 913
n 2412
o 3353
p 517
q 26
r 1958
s 2184
t 3447
u 1341
v 377
w 1017
x 37
y 1253
z 68

\*