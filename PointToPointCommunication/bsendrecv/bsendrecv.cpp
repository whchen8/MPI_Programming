// bsendrecv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"

int main(int argc, char* argv[]) {

	int myRank, nProcs;
	MPI_Status status;
	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int size1, size2, size3, bufSize;
	char *buf, msg1[7], msg3[17], rmsg1[64], rmsg3[64];
	double msg2[2], rmsg2[64];
	int src = 0, dest = 1, tag = 1, errs = 0;

	MPI_Pack_size(7, MPI_CHAR, comm, &size1);
	MPI_Pack_size(2, MPI_DOUBLE, comm, &size2);
	MPI_Pack_size(17, MPI_CHAR, comm, &size3);

	bufSize = 3 * MPI_BSEND_OVERHEAD + size1 + size2 + size3;
	buf = (char *)malloc(bufSize);
	MPI_Buffer_attach(buf, bufSize);

	strncpy(msg1, "012345", 7);
	strncpy(msg3, "0123401234012341", 17);
	msg2[0] = 1.23, msg2[1] = 3.21;

	if (myRank == src) {
		MPI_Bsend(msg1, 7, MPI_CHAR, dest, tag, comm);
		MPI_Bsend(msg2, 2, MPI_DOUBLE, dest, tag, comm);
		MPI_Bsend(msg3, 17, MPI_CHAR, dest, tag, comm);
	}

	if (myRank == dest) {
		MPI_Recv(rmsg1, 7, MPI_CHAR, src, tag, comm, &status);
		MPI_Recv(rmsg2, 10, MPI_DOUBLE, src, tag, comm, &status);
		MPI_Recv(rmsg3, 17, MPI_CHAR, src, tag, comm, &status);
		
		if (strcmp(rmsg1, msg1) != 0) {
			errs++;
			fprintf(stderr, "message 1 %s should be %s\n", rmsg1, msg1);
			fflush(stderr);
		}
		if (rmsg2[0] != msg2[0] || rmsg2[1] != msg2[1]) {
			errs++;
			fprintf(stderr, "message 2 incorrect, values are (%f, %f) but should be (%f, %f)\n",
				rmsg2[0], rmsg2[1], msg2[0], msg2[1]);
			fflush(stderr);
		}
		if (strcmp(rmsg3, msg3) != 0) {
			errs++;
			fprintf(stderr, "message 3 %s should be %s\n", rmsg3, msg3);
		}

	}

	MPI_Buffer_detach(buf, &bufSize);

	free(buf);

	MPI_Finalize();
	return 0;

}