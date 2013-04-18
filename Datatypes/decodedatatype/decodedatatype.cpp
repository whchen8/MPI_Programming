//	decodedatatype.cpp : 定义控制台应用程序的入口点。
//	构造数据类型析构的方法和结构

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>

int printDataType(MPI_Datatype datatype);

int main(int argc, char** argv) {

	int rank, numprocs;
	MPI_Datatype dTypeCont, dTypeIDBlock, dTypeCont2;

	int arrDisp [3] = {0, 3, 8};

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Type_contiguous(3, MPI_INT, &dTypeCont);
	MPI_Type_commit(&dTypeCont);

	MPI_Type_create_indexed_block(3, 2, arrDisp, dTypeCont, &dTypeIDBlock);
	MPI_Type_commit(&dTypeIDBlock);

	MPI_Type_contiguous(10, dTypeIDBlock, &dTypeCont2);
	MPI_Type_commit(&dTypeCont2);

	printDataType(dTypeCont2);

	MPI_Type_free(&dTypeCont);
	MPI_Type_free(&dTypeIDBlock);
	MPI_Type_free(&dTypeCont2);

	MPI_Finalize();
	return 0;
}

int printDataType(MPI_Datatype datatype) {

	int numInts, numAddrs, numDatatypes, combiner;
	int *arrInts;
	MPI_Aint *arrAddrs = NULL;
	MPI_Datatype *arrDTypes = NULL;
	int i;

	MPI_Type_get_envelope(datatype, &numInts, &numAddrs, &numDatatypes, &combiner);
	printf("numInts = %d, numAddrs = %d, numDatatypes = %d\n", numInts, numAddrs, numDatatypes);

	switch (combiner) {
	case MPI_COMBINER_NAMED:
		printf("Datetype is named: ");
		if (datatype == MPI_INT) printf("MPI_INT\n");
		//else if () ;
		return 0;
		break;
	case MPI_COMBINER_CONTIGUOUS:
		printf("Datatype is contiguous, containing...\n");
		arrInts = (int *) malloc(numInts * sizeof(int));
		arrAddrs = NULL;
		arrDTypes = (MPI_Datatype *) malloc(numDatatypes * sizeof(MPI_Datatype));
		MPI_Type_get_contents(datatype, numInts, numAddrs, numDatatypes,
			arrInts, arrAddrs, arrDTypes);
		printf("Count is %d\n\n", arrInts[0]);
		if (printDataType(arrDTypes[0])) {
			MPI_Type_free(&arrDTypes[0]);
		}
		break;
	case MPI_COMBINER_INDEXED_BLOCK:
		printf("Datatype is indexed_block, containing...\n");
		arrInts = (int *) malloc(numInts * sizeof(int));
		arrAddrs = NULL;
		arrDTypes = (MPI_Datatype *) malloc(numDatatypes * sizeof(MPI_Datatype));
		MPI_Type_get_contents(datatype, numInts, numAddrs, numDatatypes, 
			arrInts, arrAddrs, arrDTypes);
		printf("count = %d, blocklength = %d\n", arrInts[0], arrInts[1]);
		printf("arrDisp = {");
		for (i = 0; i < arrInts[0] - 1; i++) {
			printf("%d, ", arrInts[i+2]);
		}
		printf("%d}\n\n", arrInts[i + 2]);
		if (printDataType(arrDTypes[0])) {
			MPI_Type_free(&arrDTypes[0]);
		}
		break;
	default:
		printf("Other/Unrecognized combiner type\n");
	}

	return 1;

}