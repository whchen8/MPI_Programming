//	std_send_recv.cpp : 定义控制台应用程序的入口点。
//	

#include "stdafx.h"
#include "mpi.h"

#define SIZE 10

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int sendBuf[SIZE], recvBuf[SIZE];
	int other;
	MPI_Status status;

	for (int i = 0; i < SIZE; i++) {
		sendBuf[i] = myRank + i;
	}
	if (myRank == 0) other = 1;
	if (myRank == 1) other = 0;

	if (myRank == 0) {
		fprintf(stdout, "rank %d of %d trying sending...\n", myRank, nProcs);
		MPI_Send(sendBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD);
		fprintf(stdout, "rank %d of %d trying receiving...\n", myRank, nProcs);
		MPI_Recv(recvBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD, &status);
	}

	if (myRank == 1) {
		fprintf(stdout, "rank %d of %d trying receiving...\n", myRank, nProcs);
		MPI_Recv(recvBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD, &status);
		fprintf(stdout, "rank %d of %d trying sending...\n", myRank, nProcs);
		MPI_Send(sendBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD);
	}

	fprintf(stdout, "Hello World! Rank %d of %d\n", myRank, nProcs);

	MPI_Finalize();
	return 0;

}
