//	vectype.cpp : 定义控制台应用程序的入口点。
//	创建向量数据类型进行通信

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

#define SIZE 4

void is_type_hvector(int rank, MPI_Datatype datatype);

int main (int argc, char** argv) {

	int rank, numprocs;
	int arrA[SIZE][SIZE] = {
		{11, 12, 13, 14},
		{21, 22, 23, 24},
		{31, 32, 33, 34},
		{41, 42, 43, 44},
	};
	int arrB[SIZE];
	char outstr[600];
	int source = 0, tag = 1, i, j;

	MPI_Status status;
	MPI_Datatype colType;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_vector(SIZE, 1, SIZE, MPI_INT, &colType);
	MPI_Type_commit(&colType);
	//is_type_hvector(rank, colType);

	if (numprocs == SIZE) {
		if (rank == 0) {
			sprintf_s(outstr, "rank: %d has \n", rank);
			for(i = 0; i < SIZE; i++) {
				sprintf_s(outstr, "%s\t{", outstr);
				for(j = 0; j < SIZE; j++) {
					if(j < SIZE - 1) {
						sprintf_s(outstr, "%s%d, ", outstr, arrA[i][j]);
					} else {
						sprintf_s(outstr, "%s%d},\n",outstr, arrA[i][j]);
					}
				}
			}
			fprintf(stdout, "%s\n", outstr);
			for(i = 1; i < numprocs; i++) {
				sprintf_s(outstr, "rank: %d send to: %d {", rank, i);
				for(j = 0; j < SIZE; j++) {
					if(j < SIZE - 1) {
						sprintf_s(outstr, "%s%d, ", outstr, arrA[j][i]);
					} else {
						sprintf_s(outstr, "%s%d}\n",outstr, arrA[j][i]);
					}
				} 
				fprintf(stdout, "%s", outstr); 
				MPI_Send(&arrA[0][i], 1, colType, i, tag, MPI_COMM_WORLD);
			}
			printf("\n"); 
		} else {
			MPI_Recv(arrB, SIZE, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			//MPI_Recv(arrB, 1, colType, source, tag, MPI_COMM_WORLD, &status);
			sprintf_s(outstr, "rank: %d received: {", rank);
			for (j = 0; j < SIZE; j++) {
				if (j < SIZE - 1) {
					sprintf_s(outstr, "%s%d, ", outstr, arrB[j]);
				} else {
					sprintf_s(outstr, "%s%d}", outstr, arrB[j]);
				}
			}
			fprintf(stdout, "%s\n", outstr);
		}
	} else {
		printf("Must specify %d processors. Terminating.\n", SIZE);
	}

	MPI_Type_free(&colType);

	MPI_Finalize();
	return 0;
}

void is_type_hvector(int rank, MPI_Datatype datatype) {

	int nints, nadds, ntypes, combinder;
	int *ints;
	MPI_Aint *adds;
	MPI_Datatype *types;

	MPI_Type_get_envelope(datatype, &nints, &nadds, &ntypes, &combinder);
	printf("Envelop:\trank %d: nints = %d, nadds = %d, ntypes = %d\n", rank, nints, nadds, ntypes);

	if (combinder != MPI_COMBINER_VECTOR) {
		printf("rank %d not type vector\n", rank);
	} else {
		printf("rank %d is type vector\n", rank);

		ints = (int *) malloc(nints * sizeof(int));
		adds = (MPI_Aint *) malloc(nadds * sizeof(MPI_Aint));
		types = (MPI_Datatype *) malloc(ntypes * sizeof(MPI_Datatype));

		MPI_Type_get_contents(datatype, nints, nadds, ntypes, ints, adds, types);
		printf("Contents:\trank %d: nints = %d, nadds = %d, ntypes = %d\n",
			rank, nints, nadds, ntypes);
		printf("rank %d, count = %d, blocklength = %d, stride = %ld\n", 
			rank, ints[0], ints[1], adds[0]);

		free(ints);
		free(adds);
		free(types);
	}

}