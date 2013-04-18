// commsetgetname.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

int main(int argc, char **argv) {

	int myRank, nProcs;

	int nameLen;
	char commName[MPI_MAX_OBJECT_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	MPI_Comm_get_name(MPI_COMM_WORLD, commName, &nameLen);
	printf("Proc %d: commName is %s, len = %d\n", myRank, commName, nameLen);
	fflush(stdout);

	sprintf_s(commName, "MY_COMM_%d", myRank);
	MPI_Comm_set_name(MPI_COMM_WORLD, commName);
	MPI_Comm_get_name(MPI_COMM_WORLD, commName, &nameLen);
	printf("Proc %d: commName is %s, len = %d\n", myRank, commName, nameLen);
	fflush(stdout);

	MPI_Finalize();
	return 0;
}
