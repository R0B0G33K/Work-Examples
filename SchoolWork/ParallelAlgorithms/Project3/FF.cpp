///CS452 | Pmerge | NIMSIE && TROY
///g++ -o mergethis Pmerge.cpp
///mergethis #poweroftwoyou want #youwantfrom0tothis
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iomanip>
#include <bits/stdc++.h>

#include "mpi.h" // message passing interface

using namespace std;

void swap(int *a, int x, int y);
void mergesort(int *input, int *WIN, int first, int mid, int last, int my_rank, int p);
void pmerge(int *input, int *WIN, int first, int mid, int last, int my_rank, int p);
void smerge(int *array, int first, int last);
void smergesort(int *array, int first, int last);
int rank(int *array, int first, int last, int valToFind);
void SMERGE(int *input, int *HOLDER, int firsta, int lasta, int firstb, int lastb, int init);

void showArray(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << setw(10) << i << ": " << setw(10) << array[i];
        if ((i + 1) % 4 == 0)
            cout << endl;
    }
    cout << endl;
}

void swap(int *a, int x, int y)
{

    int temp = a[x];
    a[x] = a[y];
    a[y] = temp;
}

//Main makes the randoms and makes it cool
int main(int argc, char *argv[])
{

    int my_rank;       // my CPU number for this process
    int p;             // number of CPUs that we have
    int source;        // rank of the sender
    int dest;          // rank of destination
    int tag = 0;       // message number
    char message[100]; // message itself
    MPI_Status status; // return status for receive

    // Start MPI
    MPI_Init(&argc, &argv);

    // Find out my rank!
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Find out the number of processes!
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    //DECLARE THE GOODIES
    srand(1251);
    //Fixing to 64 for a hawt minute
    int n = pow(2, atoi(argv[1]));
    int range = 2 * pow(2, atoi(argv[1]));

    int ugh[32] = {4, 6, 8, 9, 16, 17, 18, 19, 20, 21, 23, 25, 27, 29, 31, 32, 1, 2, 3, 5, 7, 10, 11, 12, 13, 14, 15, 22, 24, 26, 28, 30};
    int hgu[32] = {30, 28, 26, 24, 22, 15, 14, 13, 12, 11, 10, 7, 5, 3, 2, 1, 32, 31, 29, 27, 25, 23, 21, 20, 19, 18, 17, 16, 9, 8, 6, 4};
    //int *input = hgu;
    int *WIN = new int[n];
    int *input = new int[n];
    if (my_rank == 0)
        for (int i = 0; i < n; i++)
            input[i] = rand() % range;

    MPI_Bcast(input, n, MPI_INT, 0, MPI_COMM_WORLD);

    mergesort(input, WIN, 0, n / 2 - 1, n - 1, my_rank, p);

    if (my_rank == 0)
    {
        cout << "WIN" << endl;
        showArray(WIN, n);
    }

    MPI_Finalize();

    return 0;
}

void mergesort(int *input, int *WIN, int first, int mid, int last, int my_rank, int p)
{

    int n = last - first + 1;

    if (n <= 4)
    {
        smergesort(input, first, last);
        return;
    }

    mergesort(input, WIN, first, (first + mid) / 2, mid, my_rank, p);
    mergesort(input, WIN, mid + 1, (last + mid + 1) / 2, last, my_rank, p);

    pmerge(input, WIN, first, mid, last, my_rank, p);
}

void pmerge(int *input, int *WIN, int first, int mid, int last, int my_rank, int p)
{
    int n = last - first + 1;

    int *a = &input[first];
    int *b = &input[first + n / 2];

    int StripeSpace = log2(n / 2);
    int numberofShapes = n / (2 * log2(n / 2));
    int lengthofPOS = 2 * numberofShapes + 1;

    int *localPOSA = new int[lengthofPOS];
    int *localPOSB = new int[lengthofPOS];

    int *POSA = new int[lengthofPOS];
    int *POSB = new int[lengthofPOS];

    for (int i = 0; i < lengthofPOS; i++)
    {
        if (i % p == my_rank)
        {
            if (i < numberofShapes)
            {
                localPOSA[i] = rank(a, 0, n / 2 - 1, b[i * StripeSpace]);
                localPOSB[i] = rank(b, 0, n / 2 - 1, a[i * StripeSpace]);
            }
            else if (i == lengthofPOS - 1)
            {
                localPOSA[i] = n / 2;
                localPOSB[i] = n / 2;
            }
            else
            {
                localPOSA[i] = (i - numberofShapes) * StripeSpace;
                localPOSB[i] = (i - numberofShapes) * StripeSpace;
            }
        }
        else
        {
            localPOSA[i] = 0;
            localPOSB[i] = 0;
        }
    }

    //DO THE APROPIATE MPI ALLREDUCE mpi_max

    MPI_Allreduce(localPOSA, POSA, lengthofPOS, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
    MPI_Allreduce(localPOSB, POSB, lengthofPOS, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    //SORT POSA and POSB

    smergesort(POSA, 0, lengthofPOS - 1);
    smergesort(POSB, 0, lengthofPOS - 1);
	
	for (int i = 0; i< lengthofPOS; i++){
		cout << "POSA " << POSA[i] << " POSB " << POSB[i] << endl;
	}
    int *WIN2 = new int[n];

    for (int i = 0; i < n; i++)
    {
        WIN2[i] = 0;
    }
    //SMERGE SHAPES
    for (int x = my_rank; x < 2 * numberofShapes; x += p)
    {
        int init = 0;

        for (int i = 0; i < x; i++)
        {
            if (*(POSA + i + 1) - 1 - *(POSA + i) < 0)
            {
                init += *(POSB + i + 1) - 1 - *(POSB + i) + 1;
            }
            else if (*(POSB + i + 1) - 1 - *(POSB + i) < 0)
            {
                init += *(POSA + i + 1) - 1 - *(POSA + i) + 1;
            }
            else
            {
                init += *(POSB + i + 1) - 1 - *(POSB + i) + 1;
                init += *(POSA + i + 1) - 1 - *(POSA + i) + 1;
            }
        }

        SMERGE(input, WIN2, first + *(POSA + x), first + *(POSA + x + 1) - 1, first + *(POSB + x) + n / 2, first + *(POSB + x + 1) + n / 2 - 1, init);
    }

    MPI_Allreduce(WIN2, WIN, n, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
    {
        input[i + first] = WIN[i];
    }
}

void smergesort(int *array, int first, int last)
{

    if (last <= first)
        return;

    if (last - first == 1 && array[last] > array[first])
        swap(array, first, last);

    int mid = (first + last) / 2;

    smergesort(array, first, mid);
    smergesort(array, mid + 1, last);

    smerge(array, first, last);
}

void smerge(int *array, int first, int last)
{
    int size = last - first + 1;
    int middle = (first + last) / 2;
    int *b = new int[size];

    int leftpoint = first;
    int rightpoint = middle + 1;

    for (int i = 0; i < size; i++)
        if (leftpoint <= middle && (rightpoint > last || array[leftpoint] <= array[rightpoint]))
            b[i] = array[leftpoint++];
        else
            b[i] = array[rightpoint++];

    for (int i = 0; i < size; i++)
        array[first + i] = b[i];

    delete[] b;
}

void SMERGE(int *input, int *HOLDER, int firsta, int lasta, int firstb, int lastb, int init)
{
    int sizea = lasta - firsta + 1;
    int sizeb = lastb - firstb + 1;
    int SIZE = sizea + sizeb;

    if (lasta < firsta)
    {
        for (int i = 0; i < sizeb; i++)
            HOLDER[init++] = input[i + firstb];
        return;
    }

    if (lastb < firstb)
    {
        for (int i = 0; i < sizea; i++)
            HOLDER[init++] = input[i + firsta];
        return;
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (firsta <= lasta && (firstb > lastb || input[firsta] <= input[firstb]))
            HOLDER[init++] = input[firsta++];
        else
            HOLDER[init++] = input[firstb++];
    }
}

int rank(int *array, int first, int last, int valToFind)
{
    if (last - first == 1)
    {
        if (valToFind < array[first])
            return rank(array, first, first, valToFind);
        else
            return rank(array, last, last, valToFind);
    }

    //Actually Super Base Case
    if (last - first == 0)
    {
        if (array[first] > valToFind)
            return first;
        else
            return first + 1;
    }
    //FUN FACT: last - \frac{last-first}{2} = \frac{last+first}{2}
    //If what we have is less or equal break under
    if (array[(int)((last + first) / 2)] >= valToFind)
    {
        return rank(array, first, (int)((last + first) / 2), valToFind);
    }
    //ANOTHER FUN FACT: first + \frac{last-first}{2} = \frac{last+first}{2}
    //If what we have is greater than break over
    if (array[(int)((last + first) / 2)] < valToFind)
    {
        return rank(array, (int)((last + first) / 2), last, valToFind);
    }
}
