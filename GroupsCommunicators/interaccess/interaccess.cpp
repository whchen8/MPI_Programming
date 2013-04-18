//	interaccess.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char **argv) {

	int myRank, nPorcs, myColor, myKey;
	MPI_Comm commNew, commInter, commInter2;

	int arrColor[8] = {0, 0, 0, 1, 1, 1, 1, 2};
	int tag12 = 12, tag23 = 23, tag31 = 31;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nPorcs);

	myColor = arrColor[myRank];
	myKey = myRank;

	MPI_Comm_split(MPI_COMM_WORLD, myColor, myKey, &commNew);

	int flagInter, localSize, remoteSize;

	if (myColor == 0) {
		
		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 3, tag12, &commInter);

		MPI_Comm_size(commInter, &localSize);
		MPI_Comm_remote_size(commInter, &remoteSize);
		MPI_Comm_test_inter(commInter, &flagInter);

		if( myRank == 0) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor + 1, localSize, remoteSize, flagInter);
		}

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 7, tag31, &commInter2);

		MPI_Comm_size(commInter2, &localSize);
		MPI_Comm_remote_size(commInter2, &remoteSize);
		MPI_Comm_test_inter(commInter2, &flagInter);

		if( myRank == 0) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor + 2, localSize, remoteSize, flagInter);
		}
	} else if (myColor == 1) {

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 0, tag12, &commInter);

		MPI_Comm_size(commInter, &localSize);
		MPI_Comm_remote_size(commInter, &remoteSize);
		MPI_Comm_test_inter(commInter, &flagInter);

		if( myRank == 3) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor - 1, localSize, remoteSize, flagInter);
		}

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 7, tag23, &commInter2);

		MPI_Comm_size(commInter2, &localSize);
		MPI_Comm_remote_size(commInter2, &remoteSize);
		MPI_Comm_test_inter(commInter2, &flagInter);

		if( myRank == 3) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor + 1, localSize, remoteSize, flagInter);
		}

	} else if (myColor == 2) {

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 3, tag23, &commInter);

		MPI_Comm_size(commInter, &localSize);
		MPI_Comm_remote_size(commInter, &remoteSize);
		MPI_Comm_test_inter(commInter, &flagInter);

		if( myRank == 7) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor - 1, localSize, remoteSize, flagInter);
		}

		MPI_Intercomm_create(commNew, 0, MPI_COMM_WORLD, 0, tag31, &commInter2);

		MPI_Comm_size(commInter2, &localSize);
		MPI_Comm_remote_size(commInter2, &remoteSize);
		MPI_Comm_test_inter(commInter2, &flagInter);

		if( myRank == 7) {
			fprintf(stdout, "color: %d => %d, localSize = %d, remoteSize = %d, flagInter = %d\n",
				myColor, myColor - 2, localSize, remoteSize, flagInter);
		}
	}

	MPI_Comm_free(&commInter);
	MPI_Comm_free(&commInter2);

	MPI_Finalize();
	return 0;

}