//	reduce_scatter.cpp : 定义控制台应用程序的入口点。
//	组内通信子的归约-散发操作

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int i;
	char outstr[600];

	int numprocs, myrank;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	int *recvCount = new int [numprocs];
	int totalCount = 0;
	for (i = 0; i < numprocs; i++) {
		recvCount[i] = i + 5;
		totalCount += recvCount[i];
	}

	int *sendBuf = new int [totalCount];
	for (i = 0; i < totalCount; i++) {
		sendBuf[i] = i;
	}

	int *recvBuf = new int [recvCount[myrank]];
	for (i = 0; i < recvCount[myrank]; i++) {
		recvBuf[i] = -111;
	}
	
	MPI_Reduce_scatter(sendBuf, recvBuf, recvCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	sprintf_s(outstr, "Rank %d \n{", myrank);
	for (i = 0; i < recvCount[myrank]; i++) {
		sprintf_s(outstr, "%s %d", outstr, recvBuf[i]);
	}	

	sprintf_s(outstr, "%s }\n", outstr, myrank);
	fprintf(stdout, "%s\n", outstr);

	MPI_Finalize();
	return 0;

}
