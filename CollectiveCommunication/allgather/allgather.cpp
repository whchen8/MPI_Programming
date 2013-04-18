//	allgather.cpp : 定义控制台应用程序的入口点。
//	组内通信子的全收集

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

static const int SIZE = 5;

int main(int argc, char** argv) {

	int bufSend[SIZE];
	int i, root = 0;
	char outstr[600];

	int myrank, numprocs;
	

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	for (i = 0; i < SIZE; i++) {
		bufSend[i] = numprocs * myrank + i;
	}

	int *bufRecv = new int [numprocs * SIZE];

	MPI_Allgather(bufSend, SIZE, MPI_INT, bufRecv, SIZE, MPI_INT, MPI_COMM_WORLD);

	sprintf_s(outstr, "Rank %d gathered {", myrank);
	for (i = 0; i < numprocs * SIZE; i++) {
		sprintf_s(outstr, "%s %d", outstr, bufRecv[i]);
	}
	fprintf(stdout, "%s}\n", outstr);
	fflush(stdout);

	MPI_Finalize();
	return 0;
}

