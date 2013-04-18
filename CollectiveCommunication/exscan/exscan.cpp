//	exscan.cpp : 定义控制台应用程序的入口点。
//	使用MPI_Exsan

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

void printRecvBuf(int myRank, int *recvBuf, int numCount, int step);

int main(int argc, char **argv) {

	int myRank, numProcs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int i, step = 1;
	int numCount, *sendBuf, *recvBuf;

	for (numCount = 2; numCount < 16; numCount *= 2, step++) {

		sendBuf = (int *)malloc(numCount * sizeof(int));
		recvBuf = (int *)malloc(numCount * sizeof(int));

		for (i = 0; i < numCount; i++) {
			sendBuf[i] = myRank * numProcs + i;
			recvBuf[i] = -1111;
		}

		MPI_Exscan(sendBuf, recvBuf, numCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

		printRecvBuf(myRank, recvBuf, numCount, step);

	}
	MPI_Finalize();
	return 0;
}

void printRecvBuf(int myRank, int *recvBuf, int numCount, int step) {

	int i;
	char outStr[600];

	sprintf_s(outStr, "Rank %d: {", myRank);
	for (i = 0; i < numCount; i++) {
		sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
	}
	fprintf(stdout, "Step: %d\t%s }\n", step, outStr);

}