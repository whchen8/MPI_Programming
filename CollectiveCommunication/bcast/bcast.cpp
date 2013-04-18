//	bcast.cpp : 定义控制台应用程序的入口点。
//	MPI_Bcast

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int rank, numprocs;

	int value;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	do {
		if (rank == 0) {
			fprintf(stdout, "Please enter a value, -1 to finish\n");
			scanf_s("%d", &value);
			//MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
		MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
		fprintf(stdout, "Process %d got %d\n", rank, value);

	} while (value >= 0);

	MPI_Finalize();
	return 0;
}
