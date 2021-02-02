//QUESTION 2 "WOULD YOU RANK THIS PROBLEM A TEN? NEIN".
//by Matthew O'Hern

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <time.h>
#include <algorithm>
#include <cmath>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 4 blah


int global = 0;
int rank(int * a, int first, int last, int valToFind){
	int middle;
	
	if (last - first == 0)
    {
        if (a[first] > valToFind)
            return first;
        else
            return first + 1;
    }
	
	
	if(first < last){
		middle = (first+last)/2; 

		if(a[middle] > valToFind){
			return rank(a, first, middle, valToFind);
		}	
		else{
			global =  middle+1;
			return rank(a, middle+1, last, valToFind);
		}
	
	}	
	return global;
}



int main (int argc, char * argv[]) {
	
	int my_rank;			// my CPU number for this process
	int p;	
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

	int sortedArray[] = {0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,4};
	int valToFind = 2;
	int size = sizeof(sortedArray)/sizeof(sortedArray[0]);
	int * a = sortedArray;
	
	int answer;
	answer = rank(a, 0, size-1, valToFind);
	
	cout << answer << endl;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}