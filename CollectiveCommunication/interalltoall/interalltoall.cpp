//	interalltoall.cpp : 定义控制台应用程序的入口点。
//	组间通信子上的alltoall

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define SIZE 3

void interAlltoall(int bufSize, int gRank, int lRoot, int color, MPI_Comm *interComm);

int main(int argc, char** argv) {

	int myRank, nProcs;
	int color, key;
	int tag12 = 12, tag23 = 23, tag13 = 13;
	MPI_Comm myComm, myInterComm1, myInterComm2, myInterComm3;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);


	color = myRank % 3;
	key = myRank;
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &myComm);

	if (color == 0) {

		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 1, tag12, &myInterComm1);
		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 2, tag13, &myInterComm3);

	} else if (color == 1) {

		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 0, tag12, &myInterComm1);
		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 2, tag23, &myInterComm2);


	} else if (color == 2) {

		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 0, tag13, &myInterComm3);
		MPI_Intercomm_create(myComm, 0, MPI_COMM_WORLD, 1, tag23, &myInterComm2);

	}

	interAlltoall(SIZE, myRank, 2, color, &myInterComm3);


	switch (color) {
	case 0:
		MPI_Comm_free(&myInterComm1);
		MPI_Comm_free(&myInterComm3);
		break;
	case 1:
		MPI_Comm_free(&myInterComm1);
		MPI_Comm_free(&myInterComm2);
		break;
	case 2:
		MPI_Comm_free(&myInterComm3);
		MPI_Comm_free(&myInterComm2);
		break;
	}

	MPI_Finalize();
	return 0;
}

void interAlltoall(int bufSize, int gRank, int lRoot, int color, MPI_Comm *interComm) {

	int i, base = 77;
	int *recvBuf = new int [bufSize];
	int *sendBuf = new int [bufSize];
	char outStr[600];
	int lRank;

	if (color == 2) {

		MPI_Comm_rank(*interComm, &lRank);

		for (i = 0; i < bufSize; i++) {
			sendBuf[i] = gRank * color + i;
			recvBuf[i] = 9999;
		}

		fprintf(stdout, "COLOR1: gRank = %d, lRank = %d, alltoall\n", 
			gRank, lRank);

		MPI_Alltoall(sendBuf, 1, MPI_INT, recvBuf, 1, MPI_INT, *interComm);

		sprintf_s(outStr, "%s", "{");
		for (i = 0; i < bufSize; i++) {
			sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
		}
		fprintf(stdout, "COLOR1: gRank = %d, lRank = %d, got: %s }\n", 
			gRank, lRank, outStr);

	} else if (color == 0) {

		MPI_Comm_rank(*interComm, &lRank);

		for (i = 0; i < bufSize; i++) {
			sendBuf[i] = gRank * color + i;
			recvBuf[i] = 9999;
		}

		fprintf(stdout, "\tCOLOR2: gRank = %d, lRank = %d, alltoall\n", 
			gRank, lRank);

		MPI_Alltoall(sendBuf, 1, MPI_INT, recvBuf, 1, MPI_INT, *interComm);

		sprintf_s(outStr, "%s", "{");
		for (i = 0; i < bufSize; i++) {
			sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
		}
		fprintf(stdout, "\tCOLOR2: gRank = %d, lRank = %d, got: %s }\n", 
			gRank, lRank, outStr);

	}

}