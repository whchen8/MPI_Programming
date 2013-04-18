// ssendrecv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int buffer[10];

	if (myRank == 0) {

		for(int i = 0; i < 10; i++) {
			buffer[i] = i;
		}
		fprintf(stdout, "rank %d sending...\n", myRank);

		MPI_Ssend(buffer, 10, MPI_INT, 1, 123, MPI_COMM_WORLD);

	} else if (myRank == 1) {

		for(int i = 0; i < 10; i++) {
			buffer[i] = -1;
		}
		fprintf(stdout, "rank %d receiving...\n", myRank);

		MPI_Recv(buffer, 10, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);

		for(int i = 0; i < 10; i++) {
			if (buffer[i] != i) {
				fprintf(stderr, "Error: buf[%d] = %d, but is expected to be %d\n", i, buffer[i], i);
			} else {
				fprintf(stdout, "rank %d: buf[%d] = %d, is expected to be %d\n", myRank, i, buffer[i], i);
			}
		}
		fflush(stdout);

	}

	MPI_Finalize();
	return 0;

}