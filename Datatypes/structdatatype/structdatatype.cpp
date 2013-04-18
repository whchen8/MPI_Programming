//	structdatatype.cpp : 定义控制台应用程序的入口点。
//	使用 MPI_Type_struct

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define  SIZE 25

int main(int argc, char** argv) {

	typedef struct {
		float x, y, z, velocity;
		int n , type;
	} Particle;
	Particle pSend[SIZE], pRecv[SIZE];

	MPI_Status status;
	MPI_Datatype particleType;
	MPI_Datatype oldTypes[2];

	int arrBlockLength[2];
	MPI_Aint extent, arrDisp[2];

	int rank, numprocs;
	int source = 0, tag = 1, i;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_extent(MPI_FLOAT, &extent);
	arrDisp[0] = 0, arrDisp[1] = 4 * extent;
	oldTypes[0] = MPI_FLOAT, oldTypes[1] = MPI_INT;
	arrBlockLength[0] = 4, arrBlockLength[1] = 2;

	MPI_Type_create_struct(2, arrBlockLength, arrDisp, oldTypes, &particleType);
	MPI_Type_commit(&particleType);

	if (rank == 0) {
		for (i = 0; i < SIZE; i++) {
			pSend[i].x = i * 1.0;
			pSend[i].y = i * (-2.0);
			pSend[i].z = i * 3.0;
			pSend[i].velocity = 0.25;
			pSend[i].n = i;
			pSend[i].type = i % 2;
		}
		for (i = 0; i < numprocs; i++) {
			MPI_Send(pSend, SIZE, particleType, i, tag, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(pRecv, SIZE, particleType, source, tag, MPI_COMM_WORLD, &status);
		printf("rank = %d, pRecv[3]: %3.2f %3.2f %3.2f %3.2f %d %d\n",
			rank, pRecv[3].x, pRecv[3].y, pRecv[3].z, pRecv[3].velocity,
			pRecv[3].n, pRecv[3].type);
		printf("rank = %d: pRecv[22]: %3.2f %3.2f %3.2f %3.2f %d %d\n",
			rank, pRecv[22].x, pRecv[22].y, pRecv[22].z, pRecv[22].velocity,
			pRecv[22].n, pRecv[22].type);
	}

	MPI_Type_free(&particleType);

	MPI_Finalize();
	return 0;

}
