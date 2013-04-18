//	comm_grp2.cpp : 定义控制台应用程序的入口点。
//	组和通信子的使用

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {

	int rankLeft[4] = {0, 1, 2, 3}, rankRight[4] = {4, 5, 6, 7};
	int i, result;
	char outStr[600];

	int nProcs, myRank;
	MPI_Group grpWorld, grpNew;
	MPI_Comm commNew;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	MPI_Comm_group(MPI_COMM_WORLD, &grpWorld);
	if (myRank < nProcs	/ 2) {
		MPI_Group_incl(grpWorld, nProcs / 2, rankLeft, &grpNew);
	} else {
		MPI_Group_incl(grpWorld, nProcs / 2, rankRight, &grpNew);
	}
	MPI_Comm_create(MPI_COMM_WORLD, grpNew, &commNew);

	int myRankCommNew, nProcsCommNew;
	int myRankGrpNew, nProcsGrpNew;

	MPI_Comm_rank(commNew, &myRankCommNew);
	MPI_Comm_size(commNew, &nProcsCommNew);
	MPI_Group_rank(grpNew, &myRankGrpNew);
	MPI_Group_size(grpNew, &nProcsGrpNew);

	fprintf(stdout, "WorldRank: %d in %d, NewCommRank: %d in %d, NewGrpRank: %d in %d\n",
		myRank, nProcs, myRankCommNew, nProcsCommNew, myRankGrpNew, nProcsGrpNew);

	MPI_Barrier(MPI_COMM_WORLD);

	int sendBuf = myRank, recvBuf;

	MPI_Allreduce(&sendBuf, &recvBuf, 1, MPI_INT, MPI_SUM, commNew);

	fprintf(stdout, "WorldRank = %d, sendBuf = %d, recvBuf = %d\n", myRank, sendBuf, recvBuf);
	fflush(stdout);

	MPI_Barrier(MPI_COMM_WORLD);

	int ranks1[8] = {0, 1, 2, 3, 4, 5, 6, 7}, ranks2[8];

	MPI_Group_compare(grpWorld, grpNew, &result);
	MPI_Group_translate_ranks(grpWorld, nProcs, ranks1, grpNew, ranks2);
	
	if (myRank == 0) {
		fprintf(stdout, "result = %d\n", result);
	}
	sprintf_s(outStr, "rank %d: ", myRank);
	for (i = 0; i < nProcs; i++) {
		sprintf_s(outStr, "%s%d = %d ", outStr, ranks1[i], ranks2[i]);
	}
	fprintf(stdout, "%s\n", outStr);


	MPI_Comm_free(&commNew);
	MPI_Group_free(&grpNew);
	MPI_Group_free(&grpWorld);

	MPI_Finalize();
	return 0;
}