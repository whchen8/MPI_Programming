//	iddatatype.cpp : 定义控制台应用程序的入口点。
//	使用MPI_Type_indexed

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define SIZE 16

int main(int argc, char** argv) {

	int rank, numprocs;
	int arrBlength[2] = {4, 2}, arrDisp[2] = {5, 12};
	int arrA[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	int arrB[SIZE] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
	int i, source = 0, tag = 1;
	char outstr[600];

	MPI_Status status;
	MPI_Datatype newIndexType;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_indexed(2, arrBlength, arrDisp, MPI_INT, &newIndexType);
	MPI_Type_commit(&newIndexType);

	if (rank == 0) {
		for (i = 1; i < numprocs; i++) {
			MPI_Send(arrA, 1, newIndexType, i, tag, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(arrB, 1, newIndexType, source, tag, MPI_COMM_WORLD, &status);
		//MPI_Recv(arrB, 6, newIndexType, source, tag, MPI_COMM_WORLD, &status);
		sprintf_s(outstr, "rank %d {", rank);
		for (i = 0; i < SIZE; i++) {
			if (i < SIZE - 1) {
				sprintf_s(outstr, "%s%d, ", outstr, arrB[i]);
			} else {
				sprintf_s(outstr, "%s%d}", outstr, arrB[i]);
			}
		}
		printf("%s\n", outstr);
	}

	MPI_Type_free(&newIndexType);

	MPI_Finalize();
	return 0;
}

