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
		
		//done
		if(a[middle] == valToFind){
			return middle;
		}	
		else if(a[middle] > valToFind){//its in first half
			return rank(a, first, middle, valToFind);
		}	
		else if(a[middle] < valToFind){//its in second half
			global =  middle+1;
			return rank(a, middle+1, last, valToFind);
		}
	
	}	
	return global;
}


void smerge(int *a, int * b, int first, int last, int middle){
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

void smergesort(int * a, int first, int last){
	int middle;
	if(first<last){
		middle = (first+last)/2;
		smergesort(a,first,middle);
		smergesort(a,middle+1,last);
		smerge(a, a, first, last, middle);
	}
	return;
}

void pmerge(int * a, int first1, int last1, int first2, int last2, int my_rank, int p){
	

		int totalSize = 2*(last2 - first2+1);
		
		int w[totalSize] = {0};
		int * WIN = w;	
		
		int * arrayA = &a[first1];
		int * arrayB = &a[first2];

		int m = totalSize/2;
		int n = totalSize/2;
		int stripeSpace = log2(m+1);
		int LOGM = log2(m);
		int LOGN = log2(m);
		int numOfShapes = 2*(ceil((double)(n+m)/(double)(LOGM+LOGN)));
		int size = ceil((double)n/LOGM);
		
		
		int sa[size] = {0};
		int sb[size] = {0};
		int sc[size] = {0};
		int sd[size] = {0};
		int*lsranka = sa; int *lsrankb = sb; int *sranka = sc; int *srankb = sd;

		for(int i = my_rank; i < size; i+=p){
			lsranka[i] = (rank(a, first2, last2, arrayA[i*stripeSpace])) -first2;
		}

		for(int i = my_rank; i < size; i+=p){
			lsrankb[i] = rank(a, first1, last1, arrayB[i*stripeSpace]) -first1;
		}
		
		MPI_Allreduce(lsranka, sranka, size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
		MPI_Allreduce(lsrankb, srankb, size, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
		

		int lw[totalSize] = {0};
		int * localWin = lw;

		int* endpointA = new int[2*size];
		int* endpointB = new int[2*size];
		
		for(int i = 0; i <size; i++){
			endpointA[i] = i*stripeSpace;
			endpointA[i+size] = srankb[i];
			endpointB[i] = i*stripeSpace;
			endpointB[i+size] = sranka[i];
		}
		
		 smergesort(endpointA,0, 2*size-1);
		 smergesort(endpointB, 0, 2*size-1);
		
		
		for(int i = my_rank; i < numOfShapes; i+=p){
			int elementsInA;
			int elementsInB;
			int totalElements;
			int startLocation;
			if(i < numOfShapes-1){
				elementsInA = endpointA[i+1] - endpointA[i];
				elementsInB = endpointB[i+1] - endpointB[i];
				totalElements = elementsInA + elementsInB;
				startLocation = endpointA[i] + endpointB[i];
		
				for(int j = 0; j< elementsInA; j++){
					localWin[startLocation+j] = arrayA[endpointA[i]+j];
				}
				for(int j = 0; j< elementsInB; j++){
					localWin[startLocation+elementsInA+j] = arrayB[endpointB[i]+j];
				}
			}			
			else{
				
				elementsInA = (totalSize/2) - endpointA[i];
				elementsInB = (totalSize/2) - endpointB[i];
				totalElements = elementsInA + elementsInB;
				startLocation = endpointA[i] + endpointB[i];
		
				for(int j = 0; j< elementsInA; j++){
					localWin[startLocation+j] = arrayA[endpointA[i]+j];
				}
				for(int j = 0; j< elementsInB; j++){
					localWin[startLocation+elementsInA+j] = arrayB[endpointB[i]+j];
				}
			}
			smergesort(localWin, startLocation, startLocation+totalElements-1);					
		}
		
		
	MPI_Allreduce(localWin, WIN, totalSize, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

	
	for (int i = 0; i < totalSize; i++)
    {
        a[i+first1] = WIN[i];
    }	
	
}


void mergesort(int * a, int first, int last, int my_rank, int p){
	
	int n = last - first + 1;
	
	if(n <=4){
		smergesort(a, first, last);
		return;
	}
	int middle;
	if(first<last){
		middle = floor((double)(first+last)/2);
		mergesort(a,first,middle, my_rank, p);
		mergesort(a,middle+1,last, my_rank, p);
		pmerge(a, first, middle, middle+1, last, my_rank, p);
	}
	return;
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
	srand(666);
    int sizeOfArray = 128;

	int * a = new int [sizeOfArray];
	
	int temp=0;
	bool error = false;
	int i=0;
	
	if(my_rank == 0){
			
		while ( i<sizeOfArray && error==false)
		{	
			
			temp = rand() %200;
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
		
		cout<<"Original:"<<endl;
		for(int i=0; i<sizeOfArray; i++)
			cout<<a[i]<<", ";
		cout<<endl;	
	}

	MPI_Bcast(a,sizeOfArray, MPI_INT, 0, MPI_COMM_WORLD);
	mergesort(a, 0, sizeOfArray-1, my_rank, p);
	
	if(my_rank == 0){
	cout<<"Complete: "<< endl;
	for(int i = 0; i< sizeOfArray; i++){
		cout<< a[i] << ", ";
	}
	cout<< endl;
	}

	delete [] a;
	// Shut down MPI
	MPI_Finalize();

	return 0;
}




