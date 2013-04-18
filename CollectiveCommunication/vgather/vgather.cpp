//	vgather.cpp : 定义控制台应用程序的入口点。
//	MPI_Gatherv

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int numprocs, rank;

	int i, root = 0;
	char outstr[600];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	int sizeSend;
	sizeSend = 3 * rank + 2;
	int *bufSend = new int [sizeSend];
	for (i = 0; i < sizeSend; i++) {
		bufSend[i] = i + sizeSend * rank;
	}

	int sizeTotal;
	sizeTotal = (3 * numprocs * numprocs + numprocs) / 2;
	int *bufRecv = new int [sizeTotal];
	int *countRecv = new int [numprocs], *arrdispl = new int [numprocs];
	int currentDisp = 0;


	for (i = 0; i < numprocs; i++) {
		arrdispl[i] = currentDisp;
		countRecv[i] = 3 * i + 2;
		currentDisp += countRecv[i];
	}

	MPI_Gatherv(bufSend, sizeSend, MPI_INT, bufRecv, 
		countRecv, arrdispl, MPI_INT, root, MPI_COMM_WORLD);
	
	if (rank == root) {
		sprintf_s(outstr, "rank %d receive", rank);
		for (i = 0; i < sizeTotal; i++) {
			sprintf_s(outstr, "%s, buf[%d] = %d", outstr, i, bufRecv[i]);
		}
		fprintf(stdout, "%s\n", outstr);
	}

	MPI_Finalize();
	return 0;

}
