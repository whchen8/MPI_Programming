//	intercommcreate.cpp : 定义控制台应用程序的入口点。
//	使用MPI_Comm_create和MPI_Comm_split创建组间通信子

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int myRank, nProcs, ranksp;
	MPI_Comm commNew, commInter;
	MPI_Group grpWorld, grpLeft, grpRight, grpSub;

	int color, key, tag = 1;
	int arrExcl[2] = {0, 2}, arrIncl[2] = {1, 3};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	color = myRank % 2;
	key = myRank;

	MPI_Comm_group(MPI_COMM_WORLD, &grpWorld);
	MPI_Comm_split(MPI_COMM_WORLD, color, key, &commNew);

	if (color == 0) {

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 1, tag, &commInter);
		MPI_Comm_group(commInter, &grpLeft);
		MPI_Group_excl(grpLeft, 2, arrExcl, &grpSub);
		MPI_Comm_rank(commInter, &ranksp);

		fprintf(stdout, "rank = %d, ranksp = %d\n", myRank, ranksp);

	} else {

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 0, tag, &commInter);
		MPI_Comm_group(commInter, &grpRight);
		MPI_Group_incl(grpRight, 2, arrIncl, &grpSub);
		MPI_Comm_rank(commInter, &ranksp);

		fprintf(stdout, "rank = %d, ranksp = %d\n", myRank, ranksp);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Comm_free(&commNew);
	MPI_Comm_free(&commInter);
	if (color == 0) {
		MPI_Group_free(&grpLeft);
	} else {
		MPI_Group_free(&grpRight);
	}
	MPI_Group_free(&grpSub);
	MPI_Group_free(&grpWorld);

	MPI_Finalize();
	return 0;
}
