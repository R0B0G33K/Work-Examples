#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h" // message passing interface
using namespace std;



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
	srand(71911);
	int n = 40000;
	int * a = new int[n];
	long int maxValue = 0;	
	long int minValue = 0;
	long int avgValue = 0;
	
	// Fill in the array with random numbers of soldiers killed (max of 150000 as that was the army size)
	if (my_rank == 0)
		for (int x = 0; x < n; x++) {
			a[x] = rand() % 150000;
		}
	
	// DIVIDE THE PROBLEM UP INTO PIECES
	int localn = n/p;
	int * localA = new int[localn];

	MPI_Scatter(&a[0], localn, MPI_INT, localA, localn, MPI_INT, 0, MPI_COMM_WORLD);

	// DO THE LOCAL WORK
	long int localMax = 0;
	long int localMin = 0;
	long int localSum = 0;
	for (int x = 0; x < localn; x++){
		if (localA[x] > localMax)
			localMax = localA[x];
		if (localA[x] < localMax)
			localMin = localA[x];
		localSum += localA[x];
	}
	// COLLECT THE RESULTS BACK TOGETHER
	MPI_Reduce(&localMax, &maxValue, 1, MPI_LONG, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(&localMin, &minValue, 1, MPI_LONG, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(&localSum, &avgValue, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
	
	//Print the results 
	if (my_rank == 0){
		cout << "Max Number: " << maxValue << endl;
		cout << "Min Number: " << minValue << endl;
		cout << "Avg Number: " << avgValue/n << endl;
	}
	
	delete [] a;
	delete [] localA;
 
 	// Shut down MPI
	MPI_Finalize();

	return 0;
}



/*
output:

with MOD

Max Number: 149999
Min Number: 37381
Avg Number: 74864


without MOD:

Max Number: 2147321315
Min Number: 55411947
Avg Number: 1072932261

\*