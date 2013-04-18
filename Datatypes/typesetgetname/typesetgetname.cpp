//	typesetgetname.cpp : 定义控制台应用程序的入口点。
//	为数据类型设置名字

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

typedef struct mpi_names_t {
	MPI_Datatype dtype;
	const char* name;
} mpi_names_t;

static mpi_names_t mpi_names[] = {
	{MPI_CHAR, "MPI_CHAR"},
	{MPI_INT, "MPI_INT"},
	{MPI_DOUBLE, "MPI_DOUBLE"},
};

int main (int argc, char** argv) {

	int rank, numprocs;
	int namelen;
	char name[MPI_MAX_OBJECT_NAME], newname[MPI_MAX_OBJECT_NAME];
	int i, errs;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//	获取所有类型名字
	for (i = 0; i < 3; i++) {
		MPI_Type_get_name(mpi_names[i].dtype, name, &namelen);
		if (strncmp(name, mpi_names[i].name, MPI_MAX_OBJECT_NAME)) {
			errs++;
			fprintf(stderr, "Expected %s but got: %s \n", mpi_names[i].name, name);
			fflush(stderr);
		} else {
			fprintf(stdout, "Proc: %d, %s type's name: %s, namelen = %d\n", 
				rank, mpi_names[i].name, name, namelen);
			fflush(stdout);
		}
	}

	//	重设类型名字
	sprintf_s(newname, "int_%d", rank);
	name[0]=0;
	MPI_Type_set_name(MPI_INT, newname);
	MPI_Type_get_name(MPI_INT, name, &namelen);
	if (strncmp(name, newname, MPI_MAX_OBJECT_NAME)) {
		errs++;
		fprintf(stderr, "\nExpected MPI_INT but got: %s \n", name);
		fflush(stderr);
	} else {
		fprintf(stdout, "\nProc: %d, MPI_INT type's name: %s, namelen = %d\n", 
			rank, name, namelen);
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;

}

