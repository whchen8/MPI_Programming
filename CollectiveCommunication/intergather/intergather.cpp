//	intergather.cpp : 定义控制台应用程序的入口点。
//	组间通信子上的散发操作

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define SIZE 4

void interGather(int bufSize, int gRank, int lRoot, int color, MPI_Comm *interComm);

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

	interGather(SIZE, myRank, 2, color, &myInterComm3);


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

void interGather(int bufSize, int gRank, int lRoot, int color, MPI_Comm *interComm) {

	int i, base = 77, totalBufSize = 4 * bufSize;
	int *recvBuf = new int [totalBufSize], *sendBuf = new int [totalBufSize];
	char outStr[600];
	int lRank;

	for (i = 0; i < totalBufSize; i++) {
		recvBuf[i] = -1;
	}

	if (color == 2) {

		MPI_Comm_rank(*interComm, &lRank);

		if(lRank == lRoot) {

			MPI_Gather(NULL, 0, MPI_INT, recvBuf, 4, MPI_INT, MPI_ROOT, *interComm);

			fprintf(stdout, "gRank = %d, lRank = %d, gather data finished\n", gRank, lRank);

			sprintf_s(outStr, "%s", "X");
			for (i = 0; i < totalBufSize; i++) {
				sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
			}
			fprintf(stdout, "gRank = %d, lRank = %d, received: %s X\n", 
				gRank, lRank, outStr);

		} else {

			MPI_Gather(NULL, 0, MPI_INT, recvBuf, 4, MPI_INT, MPI_PROC_NULL, *interComm);

			sprintf_s(outStr, "%s", "{");
			for (i = 0; i < totalBufSize; i++) {
				sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
			}
			fprintf(stdout, "gRank = %d, lRank = %d, received: %s }\n", 
				gRank, lRank, outStr);
		}
	} else if (color == 0) {

		MPI_Comm_rank(*interComm, &lRank);

		for (i = 0; i < totalBufSize; i++) {
			sendBuf[i] = base + i * 11;
		}
		fprintf(stdout, "gRank = %d, lRank = %d, prepare to scatter data\n", gRank, lRank);
		sprintf_s(outStr, "%s", "{");
		for (i = 0; i < totalBufSize; i++) {
			sprintf_s(outStr, "%s %d", outStr, sendBuf[i]);
		}
		fprintf(stdout, "gRank = %d, lRank = %d, scatter data finished: %s }\n", 
			gRank, lRank, outStr);
		MPI_Gather(sendBuf, 4, MPI_INT, recvBuf, 4, MPI_INT, lRoot, *interComm);

		sprintf_s(outStr, "%s", "{");
		for (i = 0; i < totalBufSize; i++) {
			sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
		}
		fprintf(stdout, "gRank = %d, lRank = %d, receive data finished: %s }\n", 
			gRank, lRank, outStr);
	}

}