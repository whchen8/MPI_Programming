//	defreduce.cpp : 定义控制台应用程序的入口点。
//	使用自定义归约操作和自定义数据类型

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

typedef struct {
	int real, imag;
} Complex;

void myProd(Complex *in, Complex *inout, int *len, MPI_Datatype *dType);

int main(int argc, char** argv) {

	Complex arrInit[2], arrResult[2];
	int i;

	int numprocs, myrank;
	MPI_Op myOp;
	MPI_Datatype myType;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_contiguous(2, MPI_INT, &myType);
	MPI_Type_commit(&myType);

	MPI_Op_create((MPI_User_function *) myProd, 1, &myOp);

	switch (myrank) {
	case 0:
		arrInit[0].real = 1, arrInit[0].imag = 2;
		arrInit[1].real = 3, arrInit[1].imag = 4;
		break;
	case 1:
		arrInit[0].real = 5, arrInit[0].imag = 6;
		arrInit[1].real = 7, arrInit[1].imag = 8;
		break;
	case 2:
		arrInit[0].real = 9, arrInit[0].imag = 10;
		arrInit[1].real = 11, arrInit[1].imag = 12;
		break;
	case 3:
		arrInit[0].real = 13, arrInit[0].imag = 14;
		arrInit[1].real = 15, arrInit[1].imag = 16;
		break;
	}

	MPI_Reduce(arrInit, arrResult, 2, myType, myOp, 0, MPI_COMM_WORLD);

	if (myrank == 0) {
		for (i = 0; i < 2; i++) {
			fprintf(stdout, "arrResult[%d].real = %d, arrResult[%d].imag = %d\n",
				i, arrResult[i].real, i, arrResult[i].imag);
		}
	}

	MPI_Op_free(&myOp);

	MPI_Finalize();
	return 0;

}

void myProd(Complex *in, Complex *inout, int *len, MPI_Datatype *dType) {

	int i;
	Complex c;

	for (i = 0; i < *len; i++) {
		printf("in: <%d, %d>, inout: <%d, %d>\n", in->real, in->imag, inout->real, inout->imag);
		c.real = inout->real * in->real - inout->imag * in->imag;
		c.imag = inout->real * in->imag + inout->imag * in->real;
		*inout = c;
		printf("c: <%d, %d>\n", c.real, c.imag);
		printf("i = %d, len = %d\n", i, *len);
		in++;
		inout++;
	}

}