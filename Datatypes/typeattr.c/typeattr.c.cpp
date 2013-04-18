//	typeattr.c.cpp : 定义控制台应用程序的入口点。
//	设置和访问数据类型的属性
//	复制数据类型及其属性

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int checkExists(MPI_Datatype type, int n, int key[], int attrval[]);
int checkNotExists(MPI_Datatype type, int n, int key[]);

int main(int argc, char** argv) {

	int rank, numprocs;
	int key[3], attrval[3];
	MPI_Datatype type1, type2;
	int i, errs = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//	创建关键字
	for(i = 0; i < 3; i++) {
		MPI_Type_create_keyval(MPI_TYPE_DUP_FN, MPI_TYPE_NULL_DELETE_FN, &key[i], (void*)0);
		attrval[i] = 1024 * i;
	}

	type1 = MPI_INT;
	for (i = 0; i < 3; i++) {
		MPI_Type_set_attr(type1, key[i], &attrval[i]);
	}
	fprintf(stdout, "Set for Type1. Check Type1.\n");
	errs += checkExists(type1, 3, key, attrval);

	MPI_Type_dup(type1, &type2);
	fprintf(stdout, "Dup for Type2. Check Type2.\n");
	errs += checkExists(type2, 3, key, attrval);

	for (i = 0; i < 3; i++) {
		MPI_Type_delete_attr(type1, key[i]);
	}
	fprintf(stdout, "Delete for Type1. Check Not in Type1.\n");
	errs += checkNotExists(type1, 3, key);

	for (i = 0; i < 3; i++) {
		MPI_Type_free_keyval(&key[i]);
	}

	MPI_Finalize();
	return 0;

}

int checkExists(MPI_Datatype type, int n, int key[], int attrval[]) {
	
	int i, errs = 0;
	int *val_p, flag;

	for (i = 0; i < n; i++) {
		MPI_Type_get_attr(type, key[i], &val_p, &flag);
		if (!flag) {
			errs++;
			fprintf(stderr, "\tAttribute for key %d not set\n", i);
			fflush(stderr);
		} else if (val_p != &attrval[i]) {
			errs++;
			fprintf(stderr, "\tAttribute value for key %d not correct\n", i);
			fflush(stderr);
		} else {
			fprintf(stdout, "\tvalues for key %d is: %d\n", i, *val_p);
			fflush(stdout);
		}
	}

	return errs;

}

int checkNotExists(MPI_Datatype type, int n, int key[]) {

	int i, errs = 0;
	int *val_p, flag;

	for (i = 0; i < n; i++) {
		MPI_Type_get_attr(type, key[i], &val_p, &flag);
		if (flag) {
			errs++;
			fprintf(stderr, "\tAttribute for key %d set but should be deleted\n", i);
			fflush(stderr);
		} else {
			fprintf(stdout, "\tAttribute for key %d not set\n", i);
			fflush(stdout);
		}
	}

	return errs;

}