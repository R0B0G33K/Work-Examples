#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <time.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah
void merge(int *a, int * b, int first, int last, int middle){
	int i =first;
	int j =middle+1;
	int index = first;
	int * c = new int[(last+1)];
	
	while(i<=middle && j<=last){
		if(a[i] < b[j])
			c[index++]=a[i++];
		else
			c[index++] = b[j++];
	}
	if(i>middle)
		while(j<=last)
			c[index++] = b[j++];
	else
		while(i<=middle)
			c[index++] = a[i++];
	for(int k = first; k<index;k++)
		a[k] = c[k];
	delete[] c;
}

void mergesort(int * a, int first, int last){
	int middle;
	if(first<last){
		middle = (first+last)/2;
		mergesort(a,first,middle);
		mergesort(a,middle+1,last);
		merge(a, a, first, last, middle);
	}
	return;
}

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
    int sizeOfArray = 0;
	cout << "Enter size of array you want sorted: ";
	cin >> sizeOfArray;
	int * a = new int [sizeOfArray];
	
	srand(time(NULL));
	for(int i=0; i<sizeOfArray; i++)
		a[i] = rand() % 1000000;
	cout<<"Original:"<<endl;
	for(int i=0; i<sizeOfArray; i++)
		cout<<a[i]<<endl;
	cout<<endl;
	
	mergesort(a,0,sizeOfArray-1);
	
	cout<<"Sorted:"<<endl;
	for(int i=0; i<sizeOfArray; i++)
		cout<<a[i]<<endl;

	delete [] a;
	// Shut down MPI
	MPI_Finalize();

	return 0;
}

/*
Enter size of array you want sorted: 69
Original:
748497
867749
4663
709216
601977
329661
526597
463927
70852
306912
552770
5096
689485
41812
959493
570663
858171
899087
116800
826322
600005
905390
643408
676631
230131
875542
704462
462148
64290
689708
386507
329140
73810
391170
38356
192139
237183
81305
172418
308036
388218
725188
829484
594055
767001
788977
681070
141524
204417
797871
967847
804422
219613
127607
997406
449744
3149
701868
428245
583792
907929
814752
912932
981739
722274
467640
690230
959458
548945

Sorted:
3149
4663
5096
38356
41812
64290
70852
73810
81305
116800
127607
141524
172418
192139
204417
219613
230131
237183
306912
308036
329140
329661
386507
388218
391170
428245
449744
462148
463927
467640
526597
548945
552770
570663
583792
594055
600005
601977
643408
676631
681070
689485
689708
690230
701868
704462
709216
722274
725188
748497
767001
788977
797871
804422
814752
826322
829484
858171
867749
875542
899087
905390
907929
912932
959458
959493
967847
981739
997406
*/