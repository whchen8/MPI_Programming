//	alltoall.cpp : 定义控制台应用程序的入口点。
//	MPI_Alltoall

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int myrank, numprocs;
	int sendcount = 2;
	int i, j;
	char outstr[600];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	int *sendBuf = (int *)malloc(numprocs * sendcount * sizeof(int));
	int *recvBuf = (int *)malloc(numprocs * sendcount * sizeof(int));
	if (!sendBuf || !recvBuf) {
		fprintf(stdout, "cannot allocate buffer\n");
		free(sendBuf);
		free(recvBuf);
		MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
	}

	for (i = 0; i < numprocs; i++) {
		for (j = 0; j < sendcount; j++) {
			sendBuf[i * sendcount + j] = myrank + i * sendcount + j;
			recvBuf[i * sendcount + j] = -111;
		}
	}

	MPI_Alltoall(sendBuf, sendcount, MPI_INT, recvBuf, sendcount, MPI_INT, MPI_COMM_WORLD);

	sprintf_s(outstr, "Rank %d:\n", myrank);
	for (i = 0; i < numprocs; i++) {
		sprintf_s(outstr, "%s\treceive from %d: ", outstr, i);
		for (j = 0; j < sendcount; j++) {
			sprintf_s(outstr, "%s %d", outstr, recvBuf[i * sendcount + j]);
		}
		sprintf_s(outstr, "%s\n", outstr);
	}
	fprintf(stdout, "%s\n", outstr);

	free(sendBuf);
	free(recvBuf);

	MPI_Finalize();
	return 0;

}
