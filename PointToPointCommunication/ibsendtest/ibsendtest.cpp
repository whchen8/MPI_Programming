// ibsendtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

#define SIZE 65535

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Request sendReq, recvReq;
	MPI_Status sendStats, recvStats;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int packSize, bufSize;
	char *buf;

	MPI_Pack_size(SIZE, MPI_INT, MPI_COMM_WORLD, &packSize);
	bufSize = MPI_BSEND_OVERHEAD + packSize;
	buf = (char *)malloc(bufSize);
	MPI_Buffer_attach(buf, bufSize);

	int other, sendFlag = 0, recvFlag = 0;
	int sendBuf[SIZE], recvBuf[SIZE];

	myRank == 0 ? other = 1 : other = 0;
	for (int i = 0; i < SIZE; i++) {
		sendBuf[i] = (myRank + SIZE) + i;
	}

	if (myRank == 0) {

		fprintf(stdout, "rank %d %f, before Ibsend...\n", myRank, MPI_Wtime());
		MPI_Ibsend(sendBuf, SIZE, MPI_INT, other, 111, MPI_COMM_WORLD, &sendReq);
		fprintf(stdout, "rank %d %f, after Ibsend...\n", myRank, MPI_Wtime());

		while (!sendFlag) {
			fprintf(stdout, "rank %d %f, before Ibsend Test, flag = %d\n", myRank, MPI_Wtime(), sendFlag);
			MPI_Test(&sendReq, &sendFlag, &sendStats);
			fprintf(stdout, "rank %d %f, after Ibsend Test, flag = %d\n", myRank, MPI_Wtime(), sendFlag);
		}
	} else if (myRank == 1) {

		fprintf(stdout, "rank %d %f ...\n", myRank, MPI_Wtime());
		for (int i = 0; i < 9999999; i++) {
			MPI_Wtime();
		}

		fprintf(stdout, "rank %d %f, before Irecv...\n", myRank, MPI_Wtime());
		MPI_Irecv(recvBuf, SIZE, MPI_INT, other, 111, MPI_COMM_WORLD, &recvReq);
		fprintf(stdout, "rank %d %f, after Irecv...\n", myRank, MPI_Wtime());

		while (!recvFlag) {
			fprintf(stdout, "rank %d %f, before Irecv Test, flag = %d\n", myRank, MPI_Wtime(), recvFlag);
			MPI_Test(&recvReq, &recvFlag, &recvStats);
			fprintf(stdout, "rank %d %f, after Irecv Test, flag = %d\n", myRank, MPI_Wtime(), recvFlag);
		}

		fprintf(stdout,	 "rank %d, received...\n", myRank);
		for (int i = 0; i < 2; i++) {
			fprintf(stdout, "b[%d] = %d\n", i, recvBuf[i]);
		}

	}

	fprintf(stdout, "rank %d %f, before bufDetach...\n", myRank, MPI_Wtime());
	MPI_Buffer_detach(&buf, &bufSize);
	fprintf(stdout, "rank %d %f, after bufDetach...\n", myRank, MPI_Wtime());
	free(buf);

	MPI_Finalize();
	return 0;

}
