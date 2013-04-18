//	createdarray.cpp : 定义控制台应用程序的入口点。
//	使用 MPI_Type_created_darray, MPI_Comm_set_errhandler, MPI_Type_size,
//		MPI_Pack, MPI_Unpack

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int test4DDarray(void);
static int pack_and_unpack(char* typebuf, int count , MPI_Datatype datatype, int typebufsz);

int main(int argc, char** argv) {

	int err, errs = 0;

	MPI_Init(&argc, &argv);

	MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);

	err = test4DDarray();
	if (err) {
		fprintf(stderr, "%d errors in 4D darray test.\n", err);
		fflush(stderr);
	}

	MPI_Finalize();
	return errs;
}

int test4DDarray(void) {

	int arr[72];
	int arrSize[4] = {6, 3, 2, 2};
	int arrDistr[4] = {MPI_DISTRIBUTE_BLOCK, MPI_DISTRIBUTE_BLOCK, 
		MPI_DISTRIBUTE_NONE, MPI_DISTRIBUTE_NONE};
	int arrDargs[4] = {MPI_DISTRIBUTE_DFLT_DARG, MPI_DISTRIBUTE_DFLT_DARG, 
		MPI_DISTRIBUTE_DFLT_DARG, MPI_DISTRIBUTE_DFLT_DARG};
	int arrPSize[4] = {6, 3, 1, 1};

	int rank, me;
	MPI_Datatype dTypeDarray;
	int sizeType;

	int i, err, errs = 0;

	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	//for (rank = 0; rank < 18; rank++) {
	rank = 13;
		for (i = 0; i < 72; i++) {
			arr[i] = i;
		}
		err = MPI_Type_create_darray(18, rank, 4, arrSize, arrDistr, arrDargs, arrPSize, 
			MPI_ORDER_C, MPI_INT, &dTypeDarray);
		if (err != MPI_SUCCESS) {
			errs++;
			fprintf(stderr, "in MPI_Type_create_darray call; aborting after &d errors\n", errs);
			fflush(stderr);
			return errs;
		}
		MPI_Type_commit(&dTypeDarray);

		MPI_Type_size(dTypeDarray, &sizeType);
		if (sizeType != 4 * sizeof(int)) {
			errs++;
			fprintf(stderr, "size of type = %d; should be %d\n", 
				sizeType, (int) (4 * sizeof(int)));
			fflush(stderr);
			MPI_Type_free(&dTypeDarray);
			return errs;
		}
		printf("Type size = %d\n", sizeType);

		err = pack_and_unpack((char*) arr, 1, dTypeDarray, 72 * sizeof(int));

		for (i = 0; i < 4 * rank; i++) {
			if (arr[i] != 0) {
				errs++;
				fprintf(stderr, "array[%d] = %d; should be %d\n", i, arr[i], 0);
				fflush(stderr);
			}
		}
		for (i = 4 * rank; i < 4 * rank + 4; i++) {
			if (arr[i] != i) {
				errs++;
				fprintf(stderr, "array[%d] = %d; should be %d\n", i, arr[i], i);
				fflush(stderr);
			}
		}
		for (i = 4 * rank + 4; i < 72; i++) {
			if (arr[i] != 0) {
				errs++;
				fprintf(stderr, "array[%d] = %d; should be %d\n", i, arr[i], 0);
				fflush(stderr);
			}
		}
		//fprintf(stdout, "P: %d, size of darray is: %d\n", me, sizeType);
		if (rank == 13) {
			for (i = 0; i < 72; i++) {
				printf("%d ", arr[i]);
				if (i % 9 == 8) {
					printf("\n");
				}
			}
		}
		
	//}
	MPI_Type_free(&dTypeDarray);

	return errs;

}

static int pack_and_unpack(char* typebuf, int count , MPI_Datatype datatype, int typebufsz) {

	int err, errs = 0;
	int sizeType, sizePack, position;
	char *packbuf;

	err = MPI_Type_size(datatype, &sizeType);
	if (err != MPI_SUCCESS) {
		errs++;
		fprintf(stderr, "error in MPI_Type_size call; aborting after %d errors\n", errs);
		fflush(stderr);
		return errs;
	}
	sizeType *= count;
	err = MPI_Pack_size(count, datatype, MPI_COMM_SELF, &sizePack);
	if (err != MPI_SUCCESS) {
		errs++;
		fprintf(stderr, "error in MPI_Pack_size call; aborting after %d errors\n", errs);
		fflush(stderr);
		return errs;
	}
	printf("Pack size = %d\n", sizePack);

	packbuf = (char *)malloc (sizePack);
	if (packbuf == NULL) {
		errs++;
		fprintf(stderr, "error in malloc call; aborting after %d errors\n", errs);
		fflush(stderr);
		return errs;
	}

	position = 0;
	err = MPI_Pack(typebuf, count, datatype, packbuf, sizeType, &position, MPI_COMM_SELF);
	if (position != sizeType) {
		errs++;
		fprintf(stderr, "position = %d, should be %d (pack)\n", position, sizeType);
		fflush(stderr);
		return errs;
	}
	
	memset(typebuf, 0, typebufsz);
	position = 0;
	err = MPI_Unpack(packbuf, sizeType, &position, typebuf, count, datatype, MPI_COMM_SELF);
	if (err != MPI_SUCCESS) {
		errs++;
		fprintf(stderr, "error in MPI_Unpack call; aborting after %d errors\n",
			position, sizeType);
		fflush(stderr);
		return errs;
	}

	free(packbuf);
	if (position != sizeType) {
		errs++;
		fprintf(stderr, "position = %d, should be %d (unpack)\n", position, sizeType);
		fflush(stderr);
		return errs;
	}

	return errs;

}