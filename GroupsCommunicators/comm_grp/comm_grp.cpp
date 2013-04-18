//	comm_grp.cpp : 定义控制台应用程序的入口点。
//	组和通信子的使用

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {

	int myRank, numProcs;
	MPI_Group grpWorld, grpExclRoot;
	MPI_Comm commExclRoot, commExclRootDup, commSplit, commSplit2;
	int myRankExclRoot, myRankExclRootDup, myRankSplit;
	int numProcsExclRoot, numProcsExclRootDup, numProcsSplit;

	static int ranks[] = {0};
	int i, color, key, result;
	int sendBuf[5] = {1, 2, 3, 4, 5}, recvBuf[5] = {-1, -1, -1, -1, -1};
	int sendBuf2[6] = {11, 12, 13, 14, 15, 16}, recvBuf2[6] = {-11, -11, -11, -11, -11, -11};
	int count = 5, count2 = 6;
	char outStr[600];
	

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	MPI_Comm_group(MPI_COMM_WORLD, &grpWorld);
	MPI_Group_excl(grpWorld, 1, ranks, &grpExclRoot);
	MPI_Comm_create(MPI_COMM_WORLD, grpExclRoot, &commExclRoot);

	if (myRank != 0) {

		MPI_Comm_dup(commExclRoot, &commExclRootDup);
		MPI_Comm_compare(commExclRoot, commExclRootDup, &result);

		MPI_Comm_rank(commExclRoot, &myRankExclRoot);
		MPI_Comm_rank(commExclRootDup, &myRankExclRootDup);
		MPI_Comm_size(commExclRoot, &numProcsExclRoot);
		MPI_Comm_size(commExclRootDup, &numProcsExclRootDup);

		fprintf(stdout, "WorldRank %d in %d, ExclRootRank %d in %d, " \
			"ExclRootDupRank %d in %d, result = %d\n",
			myRank, numProcs, myRankExclRoot, numProcsExclRoot, 
			myRankExclRootDup, numProcsExclRootDup, result);
	}

	color = myRank % 2;
	key = myRank;
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &commSplit);

	MPI_Comm_rank(commSplit, &myRankSplit);
	MPI_Comm_size(commSplit, &numProcsSplit);
	fprintf(stdout, "\tWorldRank %d: SplitRank %d in %d, color = %d, key = %d\n",
		myRank, myRankSplit, numProcsSplit, color, key);

	MPI_Barrier(MPI_COMM_WORLD);

	color = myRank / 3;
	key = numProcs - myRank;
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &commSplit2);

	MPI_Comm_rank(commSplit2, &myRankSplit);
	MPI_Comm_size(commSplit2, &numProcsSplit);
	fprintf(stdout, "WorldRank %d: Split2Rank %d in %d, color = %d, key = %d\n",
		myRank, myRankSplit, numProcsSplit, color, key);

	MPI_Barrier(MPI_COMM_WORLD);

	if (myRank != 0) {
		MPI_Reduce(sendBuf, recvBuf, count, MPI_INT, MPI_SUM, 1, commExclRoot);
	}

	sprintf_s(outStr, "\tWorldRank %d: {", myRank);
	for (i = 0; i < count; i++) {
		sprintf_s(outStr, "%s %d", outStr, recvBuf[i]);
	}
	fprintf(stdout, "%s }\n", outStr);

	MPI_Barrier(MPI_COMM_WORLD);

	outStr[0] = 0;
	MPI_Reduce(sendBuf2, recvBuf2, count2, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	sprintf_s(outStr, "WorldRank %d: {", myRank);
	for (i = 0; i < count2; i++) {
		sprintf_s(outStr, "%s %d", outStr, recvBuf2[i]);
	}
	fprintf(stdout, "%s }\n", outStr);

	MPI_Barrier(MPI_COMM_WORLD);

	if (myRank != 0) {
		MPI_Comm_free(&commExclRoot);
		MPI_Comm_free(&commExclRootDup);
	}
	MPI_Group_free(&grpWorld);
	MPI_Group_free(&grpExclRoot);

	MPI_Finalize();
	return 0;

}

