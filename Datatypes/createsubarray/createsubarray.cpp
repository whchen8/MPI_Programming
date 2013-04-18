//	createsubarray.cpp : 定义控制台应用程序的入口点。
//	使用 MPI_Type_create_subarray

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int rank, numprocs;
	MPI_Status status;
	MPI_Datatype dtypeASubArray;

	int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int arrSize[] = {9}, arrSubSize[] = {3}, arrStart[] = {1}; 

	int i, source = 0, tag = 1;
	char outstr1[600], outstr2[600];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_create_subarray(1, arrSize, arrSubSize, arrStart,
		MPI_ORDER_C, MPI_INT, &dtypeASubArray);
	MPI_Type_commit(&dtypeASubArray);

	if (rank == 0) {
		for (i = 1; i < numprocs; i++) {
			MPI_Send(arr, 1, dtypeASubArray, i, tag, MPI_COMM_WORLD);
		}
	} else {
		for (i = 0; i < 9; i++) {
			arr[i] = 0;
		}
		sprintf_s(outstr1, "rank %d before recv: {", rank);
		outstr2[0] = 0;
		for (i = 0; i < 9; i++) {
			sprintf_s(outstr2, "%s%d ", outstr2, arr[i]);
		}
		fprintf(stdout, "%s%s}\n", outstr1, outstr2);
		fflush(stdout);

		MPI_Recv(arr, 1, dtypeASubArray, source, tag, MPI_COMM_WORLD, &status);
		//MPI_Recv(arr, 3, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		sprintf_s(outstr1, "rank %d after recv: {", rank);
		outstr2[0] = 0;
		for (i = 0; i < 9; i++) {
			sprintf_s(outstr2, "%s%d ", outstr2, arr[i]);
		}
		fprintf(stdout, "%s%s}\n", outstr1, outstr2);
		fflush(stdout);
	}

	MPI_Type_free(&dtypeASubArray);

	MPI_Finalize();
	return 0;

}
