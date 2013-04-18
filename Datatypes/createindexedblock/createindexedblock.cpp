//	createindexedblock.cpp : 定义控制台应用程序的入口点。
//	使用 MPI_Type_create_indexed_block

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {
	
	int rank, numprocs;
	MPI_Datatype contType, idBlockType;
	MPI_Status status;

	int blocklength = 2, arrDisp[3] = {0, 3, 8};
	int buffer[30];
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (numprocs != 2) {
		printf("Please run with 2 processes.\n");
	} else {
		MPI_Type_contiguous(3, MPI_INT, &contType);
		MPI_Type_commit(&contType);
		MPI_Type_create_indexed_block(3, blocklength, arrDisp, contType, &idBlockType);
		MPI_Type_commit(&idBlockType);
	}

	if (rank == 0) {
		for (i = 0; i < 30; i++) {
			buffer[i] = i;
		}
		MPI_Send(buffer, 1, idBlockType, 1, 123, MPI_COMM_WORLD);
	} else {
		for (i = 0; i < 30; i++) {
			buffer[i] = -1;
		}
		MPI_Recv(buffer, 1, idBlockType, 0, 123, MPI_COMM_WORLD, &status);
		for (i = 0; i < 30; i++) {
			printf("%d\t", buffer[i]);
			if (i % 10 == 9) {
				printf("\n");
			}
		}
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;

}
