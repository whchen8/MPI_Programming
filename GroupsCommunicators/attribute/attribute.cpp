//	attribute.cpp : 定义控制台应用程序的入口点。
//	属性操作，以及复制通信子对象导致的属性复制和删除


#include "stdafx.h"
#include "mpi.h"

MPI_Copy_function funcCopy;
MPI_Delete_function funcDelete;

int main(int argc, char** argv) {

	int myRank, nProcs;
	MPI_Comm commDup;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int comm_keyval = 111, extra_state;
	int attribute_val[1], flag = -1;

	MPI_Comm_create_keyval(&funcCopy, &funcDelete, &comm_keyval, &extra_state);
	MPI_Comm_get_attr(MPI_COMM_WORLD, comm_keyval, &attribute_val, &flag);

	if (flag) {
		fprintf(stdout, "Before Dup: rank %d, true, get attr = %d\n", myRank, *attribute_val);
	} else {
		fprintf(stdout, "Before Dup: rank %d, false, no attr\n", myRank);
	}

	attribute_val[0] = 120;

	MPI_Comm_set_attr(MPI_COMM_WORLD, comm_keyval, attribute_val);

	MPI_Comm_dup(MPI_COMM_WORLD, &commDup);
	MPI_Comm_get_attr(commDup, comm_keyval, &attribute_val, &flag);
	if (flag) {
		fprintf(stdout, "After Dup: rank %d, true, get attr = %d\n", myRank, *attribute_val);
	} else {
		fprintf(stdout, "After Dup: rank %d, false, no attr\n", myRank);
	}

	MPI_Comm_free(&commDup);

	MPI_Finalize();
	return 0;

}

int funcCopy(MPI_Comm oldcomm, int comm_keyval, void *extra_state, void *attribute_val_in, void *attribute_val_out, int *flag) {

	int *a = (int *)attribute_val_in;

	if (attribute_val_out == 0) {
		fprintf(stdout, "in copy function: attr_out is null\n");
	} else {
		*a = *a + 1;
		* (int *)attribute_val_out = *a;
		fprintf(stdout, "in copy function: attr_out is %d\n", *(int *)attribute_val_out);
	}

	*flag = 1;

	return MPI_SUCCESS;

}

int funcDelete(MPI_Comm comm, int comm_keyval, void *attribute_val, void *extra_state) {

	int a = (int)attribute_val;

	if (attribute_val == 0) {
		fprintf(stdout, "in delete function: why passed value is null?\n");
		return 0;
	}
	fprintf(stdout, "in delete function: attr = %d\n", a);

	return MPI_SUCCESS;

}


