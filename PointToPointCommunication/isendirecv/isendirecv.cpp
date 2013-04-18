// isendirecv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Request req1, req2;
	MPI_Status status1, status2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int right = (myRank + 1) % nProcs;
	int left = myRank - 1;
	if (left < 0) left = nProcs - 1;
	int buf1[10], buf2[10];

	for (int i = 0; i < 10; i++) {
		buf2[i] = myRank * nProcs + 1;
	}

	MPI_Irecv(buf1, 10, MPI_INT, left, 123, MPI_COMM_WORLD, &req1);
	MPI_Isend(buf2, 10, MPI_INT, right, 123, MPI_COMM_WORLD, &req2);

	MPI_Wait(&req1, &status1);
	MPI_Wait(&req2, &status2);
	fprintf(stdout, "rank %d, send & recv finished\n", myRank);

	MPI_Finalize();
	return 0;

}