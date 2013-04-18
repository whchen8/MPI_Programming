//	interalltoallw.cpp : 定义控制台应用程序的入口点。
//	组间通信子上的alltoallw

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

void interAlltoallw(int gRank, int color, MPI_Comm *interComm);

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

	interAlltoallw(myRank, color, &myInterComm3);


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

void interAlltoallw(int gRank, int color, MPI_Comm *interComm) {

	int lRank, lSize;
	int *sendCount, *recvCount, *sendDispls, *recvDispls, *sendBuf, *recvBuf;
	int sendSize, recvSize;
	MPI_Datatype *sendType, *recvType;

	int i, j;
	char outStr[600];

	if (color == 2 || color == 0) {

		MPI_Comm_rank(*interComm, &lRank);
		MPI_Comm_size(*interComm, &lSize);

		sendCount = new int [lSize];
		recvCount = new int [lSize];
		sendDispls = new int [lSize];
		recvDispls = new int [lSize];
		sendType = new MPI_Datatype [lSize];
		recvType = new MPI_Datatype [lSize];
		sendSize = (((lRank + 1) * lSize + (lSize + lRank) * lSize) * lSize) / 2;
		recvSize = sendSize;
		sendBuf = new int [sendSize];
		recvBuf = new int [recvSize];

		for (i = 0; i < lSize; i++) {

			sendCount[i] = (i + lRank + 1) * lSize;
			recvCount[i] = (lRank + i + 1) * lSize;
			sendType[i] = MPI_INT;
			recvType[i] = MPI_INT;

			if (i == 0) {
				sendDispls[i] = 0;
				recvDispls[i] = 0;
			} else {
				sendDispls[i] = sendDispls[i - 1] + (sendCount[i - 1] * sizeof(int));
				recvDispls[i] = recvDispls[i - 1] + (recvCount[i - 1] * sizeof(int));
			}

			for (j = 0; j < sendCount[i]; j++) {
				sendBuf[sendDispls[i] / sizeof(int) + j] = (lRank + 1) * i * (color + 1) + j;
			}
			for (j = 0; j < recvCount[i]; j++) {
				recvBuf[recvDispls[i] / sizeof(int) + j] = -1;
			}

		}

		sprintf_s(outStr, "\n -------------------SendBuf-------------------\n");
		sprintf_s(outStr, "gRank = %3d, lRank = %3d, prepare to alltoall\n", gRank, lRank);
		for (i = 0; i < sendSize; i++) {
			sprintf_s(outStr, "%s %3d", outStr, sendBuf[i]);
			for (j = 1; j < lSize; j++) {
				if (i == sendDispls[j] / sizeof(int) - 1) {
					sprintf_s(outStr, "%s\n", outStr);
				}
			}
		}

		MPI_Alltoallw(sendBuf, sendCount, sendDispls, sendType, 
			recvBuf,recvCount, recvDispls, recvType, *interComm);

		sprintf_s(outStr, "%s\n -------------------RecvBuf-------------------\n", outStr);
		for (i = 0; i < recvSize; i++) {
			sprintf_s(outStr, "%s %3d", outStr, recvBuf[i]);
			for (j = 1; j < lSize; j++) {
				if (i == recvDispls[j] / sizeof(int) - 1) {
					sprintf_s(outStr, "%s\n", outStr);
				}
			}
		}


		fprintf(stdout, "%s\n\n", outStr);

	}

}