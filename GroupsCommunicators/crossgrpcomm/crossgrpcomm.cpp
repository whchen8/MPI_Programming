//	crossgrpcomm.cpp : 定义控制台应用程序的入口点。
//	

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

void interP2P(int bufSize, int rank, int color, MPI_Comm *commInter);

int main(int argc, char **argv) {

	int myRank, nProcs;
	int color, key, tag1= 11, tag2 = 22;
	MPI_Comm commNew, commInter1, commInter2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	color = myRank % 3;
	key = myRank;

	MPI_Comm_split(MPI_COMM_WORLD, color, key, &commNew);

	if (color == 0) {
		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 1, tag1, &commInter1);
//		fprintf(stdout, "rank %d, first call end\n", myRank);
	} else if (color == 1) {
		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 0, tag1, &commInter1);
//		fprintf(stdout, "rank %d, second 1 call end\n", myRank);
		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 2, tag2, &commInter2);
//		fprintf(stdout, "rank %d, second 2 call end\n", myRank);
	} else if (color == 2) {
//		fprintf(stdout, "rank %d, third call before\n", myRank);
		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 1, tag2, &commInter1);
//		fprintf(stdout, "rank %d, third call end\n", myRank);
	}

	interP2P(5, myRank, color, &commInter1);

	switch(color) {
	case 0:
		MPI_Comm_free(&commInter1);
		break;
	case 1:
		MPI_Comm_free(&commInter1);
		MPI_Comm_free(&commInter2);
		break;
	case 2:
		MPI_Comm_free(&commInter1);
		break;
	}

	MPI_Finalize();
	return 0;
}

void interP2P(int bufSize, int rank, int color, MPI_Comm *commInter) {

	int i, tag1 = 123, tag2 = 321;
	int *sendBuf = new int[bufSize], *recvBuf = new int[bufSize];
	int *sendBuf2 = new int[bufSize], *recvBuf2 = new int[bufSize];
	char outStr[600];

	int myRank, nProcs;
	MPI_Request sendReq1, recvReq1, sendReq2, recvReq2;
	MPI_Status sendStats1, recvStats1, sendStats2, recvStats2;

	MPI_Comm_rank(*commInter, &myRank);
	MPI_Comm_size(*commInter, &nProcs);

	if (color == 0) {

		for (i = 0; i < bufSize; i++) {
			sendBuf[i] = myRank * nProcs + i;
		}

		MPI_Issend(sendBuf, bufSize, MPI_INT, myRank, tag1, *commInter, &sendReq1);
		MPI_Wait(&sendReq1, &sendStats1);
		
	} else if (color == 1) {
		
		MPI_Irecv(recvBuf, bufSize, MPI_INT, myRank, tag1, *commInter, &recvReq1);
		MPI_Wait(&recvReq1, &recvStats1);

		sprintf_s(outStr, "proc G: %d, L: %d, received:\n", rank, myRank);
		for (i = 0; i < bufSize; i++) {
			sprintf_s(outStr, "%srb[%d] = %d, ", outStr, i, recvBuf[i]);
		}
		fprintf(stdout, "%s\n", outStr);

	} else if (color == 2) {

	}

}