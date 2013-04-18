// rsendrecv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int buf[10];

	if (myRank == 0) {

		for (int i = 0; i < 10; i++) {
			buf[i] = -1;
		}

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Recv(buf, 10, MPI_INT, 1, 123, MPI_COMM_WORLD, &status);

		for (int i = 0; i < 10; i++) {
			if (buf[i] != i) {
				fprintf(stderr, "Error: buf[%d] = %d, but is expected to be %d\n", i, buf[i], i);
			}
		}
		for (int i = 0; i < 10; i++) {
			fprintf(stdout, "rank %d: buf[%d] = %d, expected to be %d\n", myRank, i, buf[i], i);
		}
		fflush(stdout);

	} else if (myRank == 1) {

		for (int i = 0; i < 10; i++) {
			buf[i] = i;
		}
		MPI_Rsend(buf, 10, MPI_INT, 0, 123, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;

}
