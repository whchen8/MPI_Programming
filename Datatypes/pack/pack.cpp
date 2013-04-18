//	pack.cpp : 定义控制台应用程序的入口点。
//	利用MPI_Pack、MPI_Unpack进行通信

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int rank, numprocs;
	int intA;
	double doubleB;
	unsigned long ulongC;

	int packsize, position;
	char packbuf[100];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	do {
		if(rank == 0) {
			fprintf(stdout, "Please input an int, a double, and a ull:\n");
			scanf_s("%d%lf%lu", &intA, &doubleB, &ulongC);
			packsize = 0;

			MPI_Pack(&intA, 1, MPI_INT, packbuf, 100, &packsize, MPI_COMM_WORLD);
			printf("packsize = %d\t", packsize);
			printf("size(int) = %d\n", sizeof(int));
			MPI_Pack(&doubleB, 1, MPI_DOUBLE, packbuf, 100, &packsize, MPI_COMM_WORLD);
			printf("packsize = %d\t", packsize);
			printf("size(double) = %d\n", sizeof(double));
			MPI_Pack(&ulongC, 1, MPI_UNSIGNED_LONG, packbuf, 100, &packsize, MPI_COMM_WORLD);
			printf("packsize = %d\t", packsize); 
			printf("size(unsigned long) = %d\n", sizeof(unsigned long));
		}

		MPI_Bcast(&packsize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(packbuf, packsize, MPI_PACKED, 0, MPI_COMM_WORLD);

		if(rank != 0) {
			position = 0;
			MPI_Unpack(packbuf, packsize, &position, &intA, 1, MPI_INT, MPI_COMM_WORLD);
			printf("postion= %d\t", position);
			MPI_Unpack(packbuf, packsize, &position, &doubleB, 1, MPI_DOUBLE, MPI_COMM_WORLD);
			printf("postion= %d\t", position);
			MPI_Unpack(packbuf, packsize, &position, &ulongC, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);
			printf("postion= %d\n", position);
			printf("rank %d got int %d, double %lf, and ull %lu\n", rank, intA, doubleB, ulongC);
		}

		MPI_Barrier(MPI_COMM_WORLD);

	} while (intA >= 0);

	printf("myid = %d, %d, %lf\n", rank, intA, doubleB);

	MPI_Finalize();
	return 0;
}
