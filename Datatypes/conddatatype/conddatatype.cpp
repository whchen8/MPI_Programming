//	conddatatype.cpp : 定义控制台应用程序的入口点。
//	自定义连续数据类型MPI_Type_contiguous

#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define SIZE 4

int main(int argc, char* argv[]){
	
	int rank, numprocs;
	int source = 0, tag = 1;
	char outstr[600];
	int i, j;
	int arrA[SIZE][SIZE] = {
			{10, 11, 12, 13},
			{20, 21, 22, 23},
			{30, 31, 32, 33},
			{40, 41, 42, 43},
	};
	int arrB[SIZE];
	MPI_Status status;
	MPI_Datatype newRowType;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	
	MPI_Type_contiguous(SIZE, MPI_INT, &newRowType);
	MPI_Type_commit(&newRowType);
	
	if(numprocs == SIZE) {
		if(rank == 0) {
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
						sprintf_s(outstr, "%s%d, ", outstr, arrA[i][j]);
					} else {
						sprintf_s(outstr, "%s%d},\n",outstr, arrA[i][j]);
					}
				}
				fprintf(stdout, "%s\n", outstr);
				MPI_Send(&arrA[i][0], 1, newRowType, i, tag, MPI_COMM_WORLD);
			}
		} else {
			MPI_Recv(arrB, SIZE, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			sprintf_s(outstr, "rank: %d received: {", rank);
			for(j = 0; j < SIZE; j++) {
				if(j < SIZE - 1) {
					sprintf_s(outstr, "%s%d, ", outstr, arrB[j]);
				} else {
					sprintf_s(outstr, "%s%d},\n",outstr, arrB[j]);
				}
			} 
			fprintf(stdout, "%s\n", outstr);
		}
	} else {
		printf("Must specify %d processors. Terminating.\n", SIZE);
	}
	
	MPI_Type_free(&newRowType);
	MPI_Finalize();
	return 0;
}
