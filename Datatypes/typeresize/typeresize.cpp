//	typeresize.cpp : 定义控制台应用程序的入口点。
//	使用 MPI_Type_create_resized, MPI_Type_get_extent, MPI_Type_get_true_extent,
//		MPI_Type_size, MPI_Address

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int main(int argc, char** argv) {

	int rank, numprocs;
	MPI_Datatype newType;
	MPI_Aint oldLB, newLB, oldExtent, newExtent, oldAddr, newAddr;
	int oldSize, newSize, arr[10];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_create_resized(MPI_DOUBLE, (MPI_Aint)0, (MPI_Aint)(2 * sizeof(double)), &newType);
	MPI_Type_commit(&newType);

	MPI_Type_get_extent(newType, &newLB, &newExtent);
	MPI_Type_get_extent(MPI_DOUBLE, &oldLB, &oldExtent);
	fprintf(stdout, "rank %d:\n\t lb old = %ld, ext old = %ld \n\t lb new = %ld, ext new = %ld\n", 
		rank, oldLB, oldExtent, newLB, newExtent);

	MPI_Type_get_true_extent(newType, &newLB, &newExtent);
	MPI_Type_get_true_extent(MPI_DOUBLE, &oldLB, &oldExtent);
	fprintf(stdout, "rank %d:\n\t true lb old = %ld, true ext old = %ld \n\t true lb new = %ld, true ext new = %ld\n", 
		rank, oldLB, oldExtent, newLB, newExtent);

	MPI_Type_size(MPI_DOUBLE, &oldSize);
	MPI_Type_size(newType, &newSize);
	fprintf(stdout, "rank %d:\n\t size old = %d, size new =%d\n", rank, oldSize, newSize);

	MPI_Address(arr, &oldAddr);
	MPI_Address(arr, &newAddr);
	fprintf(stdout, "rank %d:\n\t addr old = %p, addr new =%p\n", 
		rank, (void *)oldAddr, (void *)newAddr);

	MPI_Type_free(&newType);

	MPI_Finalize();
	return 0;

}