//	alltoallw.cpp : 定义控制台应用程序的入口点。
//	使用MPI_Alltoallw，每个进程i向各个进程发送不同消息

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

void printRecvBuf(int myRank, int *recvBuf, int bufSize);

int main(int argc, char** argv) {

	int myrank, numprocs;
	int i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	int *sendBuf, *sendCounts, *sendDispls, *recvBuf, *recvCounts, *recvDispls;
	MPI_Datatype *sendTypes, *recvTypes;

	sendBuf = (int *)malloc(numprocs * numprocs * sizeof(int));
	recvBuf = (int *)malloc(numprocs * numprocs * sizeof(int));
	if (!sendBuf || !recvBuf) {
		fprintf(stderr, "Could not allocate buffers~\n");
		fflush(stderr);
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	// Init buffers
	for (i = 0; i < numprocs * numprocs; i++) {
		sendBuf[i] = i + 100 * myrank;
		recvBuf[i] = -1;
	}
	
	sendCounts = (int *)malloc(numprocs * sizeof(int));
	recvCounts = (int *)malloc(numprocs * sizeof(int));
	sendDispls = (int *)malloc(numprocs * sizeof(int));
	recvDispls = (int *)malloc(numprocs * sizeof(int));
	sendTypes = (MPI_Datatype *)malloc(numprocs * sizeof(MPI_Datatype));
	recvTypes = (MPI_Datatype *)malloc(numprocs * sizeof(MPI_Datatype));
	if (!sendCounts || !sendDispls || !sendTypes || !recvCounts || !recvDispls || !recvTypes) {
		fprintf(stderr, "could not allocate arg items!\n");
		fflush(stderr);
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}
	// Init args
	// 进程0不发送消息（sendcount[0] = 0）
	for (i = 0; i < numprocs; i++) {
		sendCounts[i] = i;
		recvCounts[i] = myrank;
		sendDispls[i] = (((i + 1) * i) / 2) * sizeof(int);
		recvDispls[i] = i * myrank * sizeof(int);
		sendTypes[i] = recvTypes[i] = MPI_INT;
	}

	MPI_Alltoallw(sendBuf, sendCounts, sendDispls, sendTypes, 
		recvBuf, recvCounts, recvDispls, recvTypes, MPI_COMM_WORLD);

	printRecvBuf(myrank, recvBuf, numprocs * numprocs);

	free(sendBuf), free(sendCounts), free(sendDispls), free(sendTypes);
	free(recvBuf), free(recvCounts), free(recvDispls), free(recvTypes);

	MPI_Finalize();
	return 0;

}

void printRecvBuf(int myRank, int *recvBuf, int bufSize) {

	int i;

	char outStr[600];

	sprintf_s(outStr, "Rank %d:\n", myRank);
	for (i = 0; i < bufSize; i++) {
		sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
	}
	fprintf(stdout, "%s\n", outStr);

}