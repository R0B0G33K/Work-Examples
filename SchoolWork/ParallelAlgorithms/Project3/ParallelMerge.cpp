//Parker Winters

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int globalCounterB=0;
	int globalCounterA=0;
	int pmergeCall =0;
	
	void smerge(int * a, int first1, int last1, int first2, int last2)
	{
		int i =first1;
		int j =first2;
		int k =first1;
		int * c = new int [(last2+1)];
	
		while ( i<=last1 && j<=last2)
			if (a[i] < a[j])
				c[k++]=a[i++];
			else 
				c[k++] = a[j++];
		for (i; i<=last1;i++)
			c[k++] = a[i];
		for (j; j<=last2;j++)
			c[k++] = a[j];
		for (int h=first1; h<k; h++)
			a[h] = c[h];
		delete [] c;
	}
	
	void painSort(int * a, int first, int last)
	{
		if (first < last) {
			int middle =(first+last)/2;
			painSort(a, first, middle);
			painSort(a, (middle+1), last);	
			smerge(a,first,middle, (middle+1), last);
		}
	}

	int rank(int * a, int first, int last, int valToFind)
	{

		// cout << "first: " << first << " last: " << last << "valToFind: " << valToFind << endl;
		// for ( int i=first; i<=last; i++)
			// cout << a[i] << " ";
		// cout << endl << "work damn it" << endl;
		int middle = (first + last)/2;
		if (last >= first) {
			if (a[middle]>valToFind) 
				return rank(a,first, middle-1,valToFind);
			else 
				return rank(a,middle+1, last,valToFind);
		}
		else 
			if (middle==0) {
				if (valToFind>a[middle])
					return (middle+1);
				return 0;}
			if (valToFind>a[middle])
				return (middle+1);
			return middle-1;
			
	}
	
	void pmerge(int * a, int first1, int last1, int first2, int last2)
	{
		//pmergeCall++;
		int n = last1-first1+1;
		int m = last2-first2;
		int LOGN = log2 (n);
		int LOGM = log2 (m);
		
		//cout << "first2: " << first2 << endl;
		
		int sa[(int)(n/LOGM)+1] = {0};
		int sb[m/LOGN+1] = {0};
		int sc[(int)(n/LOGM)+1] = {0};
		int sd[m/LOGN+1] = {0};
		int valToFind=0;
		int*sranka = sa; int *srankb = sb; int *srankc = sc; int *srankd = sd;
		// if(my_rank == 0){
			// cout << "Ranks" << endl;
			// for(int i = 0; i < (int)(n/LOGM)+1; i++)
			// cout << "sa: " << sranka[i] << endl;
		//}
		for(int i = my_rank; i < (int)(n/LOGM)+1; i+=p){
			// globalCounterA++;
			sranka[i] = rank(a, first2, last2-1, a[(i*LOGN)])-first2;
			// cout << "sranka[" << i << "]: " << sranka[i] << endl;
		}
		for(int i = my_rank; i < (int)(m/LOGN)+1; i+=p){
			// globalCounterB++;
			srankb[i] = rank(a, first1, last1, a[(i*LOGM)+first2]-first1);
			// cout << "srankb[" << i << "]: " << srankb[i] << endl;
		}
		
		// cout << "my_rank: " << my_rank << " globalCounterA " << globalCounterA <<endl;
		// cout << "my_rank: " << my_rank << " globalCounterB " << globalCounterB <<endl;
		MPI_Allreduce(sranka, srankc, (int)(n/LOGM)+1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
		MPI_Allreduce(srankb, srankd, (int)(m/LOGN)+1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
		
		if(my_rank == 0){
			cout << "Ranks" << endl;
			for(int i = 0; i < (int)(n/LOGM)+1; i++)
			cout << "sranka: " << srankc[i] << endl;}
		if(my_rank == 0){
			cout << "Ranks" << endl;
			for(int i = 0; i < (int)(m/LOGN)+1; i++)
			cout << "srankb: " << srankd[i] << endl;}
		
	}
	
	void mergesort(int * a, int first, int last)
	{
		if ((last-first)<=32)
			return;
		if (first < last) {
			int middle =(first+last)/2;
			mergesort(a, first, middle);
			mergesort(a, (middle+1), last);	
			pmerge(a,first,(middle-1), middle, last);
		}
	}

int main (int argc, char * argv[]) {

	

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
	
	int enteredAmount = 64;
	int ranktofind=0;
	// cout << "Please enter how many numbers to sort: ";
	// cin >> enteredAmount;
	// cout << "Please enter an number to rank: ";
	// cin >> ranktofind;
	
	int * a = new int [enteredAmount];
	
	srand(5);
	int temp=0;
	bool error = false;
	int i=0;
	
	if(my_rank == 0){
		while ( i<sizeOfArray && error==false)
		{	
			
			temp = rand() %100;
			for (int j=0; j<i; j++)
			{
				if (a[j] == temp)
					error =true;
			}
			if (error==false){
					a[i] = temp;
					i++;
			}
			error = false;
		}
		
		for (int i=0; i<enteredAmount; i++)
			cout << a[i] << " ";
		cout << endl;
		
		smergesort(a,0,(enteredAmount/2)-1);
		smergesort(a,(enteredAmount/2),enteredAmount-1);
		
		for (int i=0; i<enteredAmount; i++)
			cout << a[i] << " ";
		cout << endl;
	}
	
	MPI_Bcast(a, enteredAmount, MPI_INT, 0, MPI_COMM_WORLD);
	mergesort(a, 0, enteredAmount);
	//cout << "pmergeCall: " << pmergeCall << endl;
	
	
	
	
// srand(time(NULL));
	// srand(5);
	// for (int i=0; i<enteredAmount; i++) {
		// a[i] = rand() % 100;
		// cout << a[i] << " ";
	// }
	// cout << endl;
	
	// mergesort(a,0,enteredAmount-1);
	
	// for (int i=0; i<enteredAmount; i++)
		// cout << a[i] << " ";
	// cout << endl;
	
	// int * b = new int [enteredAmount];
	// srand(5);
	// cout << "a" << endl;
	// int temp=0;
	// bool containd =false;
	// int k=0;
	// while ( k<enteredAmount) {
		// containd=false;
		// temp = rand() % 100;
		// for (int j =0; j<=k; j++)
			// if (temp==a[j])
				// containd=true;
		// if (containd ==false){
			// a[k] =temp;
			// k++;
		// }
		// cout << a[k] << " ";
	// }
	// cout << endl;
	
	// mergesort(b,0,enteredAmount-1);
	
	// cout << "b" << endl;
	// for (int i=0; i<enteredAmount; i++)
		// cout << b[i] << " ";
	// cout << endl;
	
//RANK CODE
	// int * b = new int [enteredAmount];
	// srand(5);
	// cout << "a" << endl;
	// int temp=0;
	// bool containd =false;
	// int k=0;
	// while ( k<enteredAmount) {
		// containd=false;
		// temp = rand() % 100;
		// for (int j =0; j<=k; j++)
			// if (temp==a[j])
				// containd=true;
		// if (containd ==false){
			// a[k] =temp;
			// k++;
		// }
		// cout << a[k] << " ";
	// }
	// cout << endl;
	
	// mergesort(a, 0, enteredAmount-1);
	
	// srand(1);
	// cout << "b" << endl;
	// for (int i=0; i<enteredAmount; i++) {
		// b[i] = rand() % 100;
		// cout << b[i] << " ";
	// }
	// cout << endl;
	
	// mergesort(b, 0, enteredAmount-1);
	
	// cout << "a" << endl;
	// for (int i=0; i<enteredAmount; i++)
		// cout << a[i] << " ";
	// cout << endl;

	// cout << (rank(a, 0, enteredAmount-1, ranktofind)) << " ";
	// cout << endl;
	
	
	// Shut down MPI
	MPI_Finalize();
	delete [] a;
	return 0;
}
