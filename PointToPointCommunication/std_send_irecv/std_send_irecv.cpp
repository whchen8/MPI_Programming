//	std_send_irecv.cpp : 定义控制台应用程序的入口点。
//	

#include "stdafx.h"
#include "mpi.h"
#include <math.h>

#define SIZE 10

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Request req1, req2;
	MPI_Status status1, status2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int sendBuf[SIZE], recvBuf[SIZE];
	int other;

	for (int i = 0; i < SIZE; i++) {
		sendBuf[i] = myRank + i;
	}
	if (myRank == 0) other = 1;
	if (myRank == 1) other = 0;

	if (myRank == 0) {
		fprintf(stdout, "rank %d\t%f\tbefore sending...\n", myRank, MPI_Wtime());
		MPI_Send(sendBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD);
		fprintf(stdout, "rank %d\t%f\tafter sending...\n", myRank, MPI_Wtime());

		fprintf(stdout, "rank %d\t%f\tbefore ireceiving...\n", myRank, MPI_Wtime());
		MPI_Irecv(recvBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD, &req1);
		fprintf(stdout, "rank %d\t%f\tafter ireceiving...\n", myRank, MPI_Wtime());

		fprintf(stdout, "rank %d\t%f\tbefore waiting...\n", myRank, MPI_Wtime());
		MPI_Wait(&req1, &status1);
		fprintf(stdout, "rank %d\t%f\tafter waiting...\n", myRank, MPI_Wtime());
	}

	if (myRank == 1) {
		fprintf(stdout, "\trank %d\t%f\tbefore ireceiving...\n", myRank, MPI_Wtime());
		MPI_Irecv(recvBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD, &req2);
		fprintf(stdout, "\trank %d\t%f\tafter ireceiving...\n", myRank, MPI_Wtime());

		fprintf(stdout, "\trank %d\t%f\tbefore sending...\n", myRank, MPI_Wtime());
		MPI_Send(sendBuf, SIZE, MPI_INT, other, 1, MPI_COMM_WORLD);
		fprintf(stdout, "\trank %d\t%f\tafter sending...\n", myRank, MPI_Wtime());

		fprintf(stdout, "\trank %d\t%f\tbefore waiting...\n", myRank, MPI_Wtime());
		MPI_Wait(&req2, &status2);
		fprintf(stdout, "\trank %d\t%f\tafter waiting...\n", myRank, MPI_Wtime());
	}

	//fprintf(stdout, "Hello World! Rank %d of %d\n", myRank, nProcs);

	MPI_Finalize();
	return 0;

}
