#include <iostream>
#include <stdio.h>
#include <string.h>
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
	char messageOne[100];		// message itself
	char messageTwo[100];
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
    char * ringOne = messageOne;
	char * ringTwo = messageTwo;
	string curInsult;
	string curInsult2;
	string insults[2] = {
		"a cockered tardy-gaited knave",
		"a not so big as a round little worm",
	};
 
	if (my_rank == 0) {
		cout << "Frodo Baggins is " << endl;
		sprintf(ringOne, "Ring One: ");
		curInsult.append(insults[my_rank]);
		sprintf(ringOne + strlen(ringOne), "%d ", my_rank);;
		MPI_Send(ringOne, strlen(ringOne) + 1, MPI_CHAR, my_rank+2, tag, MPI_COMM_WORLD);
		if (p%2==0)
			MPI_Recv(ringOne, 100, MPI_CHAR, p-2, tag, MPI_COMM_WORLD, &status);
		else
			MPI_Recv(ringOne, 100, MPI_CHAR, p-1, tag, MPI_COMM_WORLD, &status);
		curInsult.append(" and an idiot!");
		sprintf(ringOne + strlen(ringOne), "%d ", my_rank);
		cout << ringOne << endl;
		cout << curInsult << endl;
	}
	else if (my_rank%2==0){
		MPI_Recv(ringOne, 100, MPI_CHAR, my_rank - 2, tag, MPI_COMM_WORLD, &status);
		sprintf(ringOne + strlen(ringOne), "%d ", my_rank);
		if (my_rank +2 < p)
			MPI_Send(ringOne, strlen(ringOne) + 1, MPI_CHAR, (my_rank + 2), tag, MPI_COMM_WORLD);
		else 
			MPI_Send(ringOne, strlen(ringOne) + 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
	}
	
	if (my_rank == 1) {
		sprintf(ringTwo, "Ring Two: ");
		curInsult2.append(insults[my_rank]);
		sprintf(ringTwo + strlen(ringTwo), "%d ", my_rank);
		if (p%2==1)
			MPI_Send(ringTwo, strlen(ringTwo) + 1, MPI_CHAR, p-2, tag, MPI_COMM_WORLD);
		else 
			MPI_Send(ringTwo, strlen(ringTwo) + 1, MPI_CHAR, p-1, tag, MPI_COMM_WORLD);
		MPI_Recv(ringTwo, 100, MPI_CHAR, 3, tag, MPI_COMM_WORLD, &status);
		curInsult2.append(" and an idiot!");
		sprintf(ringTwo + strlen(ringTwo), "%d ", my_rank);
		cout << ringTwo << endl;
		cout << curInsult2 << endl;
	}
	else if (my_rank%2==1){
		if (my_rank +2 >= p)
			MPI_Recv(ringTwo, 100, MPI_CHAR, 1, tag, MPI_COMM_WORLD, &status);
		else 
			MPI_Recv(ringTwo, 100, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD, &status);
		sprintf(ringTwo + strlen(ringTwo), "%d ", my_rank);
		MPI_Send(ringTwo, strlen(ringTwo) + 1, MPI_CHAR, my_rank-2, tag, MPI_COMM_WORLD);
	}

	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/*
output:

Frodo Baggins is
Ring One: 0 2 4 6 8 0
a cockered tardy-gaited knave and an idiot!
Ring Two: 1 9 7 5 3 1
a not so big as a round little worm and an idiot!

\*