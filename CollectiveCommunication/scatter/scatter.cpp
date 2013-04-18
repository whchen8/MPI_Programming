//	scatter.cpp : 定义控制台应用程序的入口点。
//	MPI_Scatter

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define SIZE 4

int main(int argc, char** argv) {

	int numprocs, rank;

	int arrSend[SIZE][SIZE] = {
		{10, 11, 12, 13},
		{20, 21, 22, 23},
		{30, 31, 32, 33},
		{40, 41, 42, 43}
	};
	int arrRecv[SIZE];
	int sendCount = SIZE, recvCount = SIZE;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (numprocs == SIZE) {
		MPI_Scatter(arrSend, sendCount, MPI_INT, arrRecv, recvCount, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Rnak %d: arrRecv = {%d, %d, %d, %d}\n",
			rank, arrRecv[0], arrRecv[1], arrRecv[2], arrRecv[3]);
	} else {
		printf("Must specify %d processors. Terminating.\n", SIZE);
	}

	MPI_Finalize();
	return 0;

}