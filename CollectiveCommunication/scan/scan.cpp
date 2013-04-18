//	scan.cpp : 定义控制台应用程序的入口点。
//	使用MPI_Scan

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

static const int BUFSIZE = 5;

int main(int argc, char **argv) {

	int i;
	char outStr[600];

	int myRank, numProcs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	int *sendBuf = (int *)malloc(BUFSIZE * sizeof(int));
	int *recvBuf = (int *)malloc(BUFSIZE * sizeof(int));

	sprintf_s(outStr, "Rank %d send : {", myRank);
	for (i = 0; i < BUFSIZE; i++) {
		sendBuf[i] = myRank * numProcs + i;
		sprintf_s(outStr, "%s %d", outStr, sendBuf[i]);
	}
	fprintf(stdout, "%s}\n", outStr);
	
	MPI_Scan(sendBuf, recvBuf, BUFSIZE, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	sprintf_s(outStr, "Rank %d receive: {", myRank);
	for (i = 0; i < BUFSIZE; i++) {
		sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
	}
	fprintf(stdout, "%s}\n", outStr);

	MPI_Finalize();
	return 0;

}