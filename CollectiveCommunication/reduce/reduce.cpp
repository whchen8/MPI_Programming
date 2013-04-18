//	reduce.cpp : 定义控制台应用程序的入口点。
//	使用MPI_reduce进行面积法求π

#include "stdafx.h"
#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double x);

int main(int argc, char** argv) {

	int i, interval;
	double height, sum = 0.0, mypi, pi;
	double PI25DT = 3.141592653589793238462643;
	double startWTime = 0.0, endWTime;

	int rank, numprocs;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (rank == 0) {
		printf("please enter the interval: ");
		scanf_s("%d", &interval);
		startWTime = MPI_Wtime();
	}
	MPI_Bcast(&interval, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	for (i = rank + 1; i <= interval; i += numprocs) {
		height = ((double)i - 0.5) / double(interval);
		sum += f(height);
	}
	mypi = sum / (double)interval;

	MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
		endWTime = MPI_Wtime();
		printf("wall clock time = %f\n", endWTime - startWTime);
		fflush(stdout);
	}

	MPI_Finalize();
	return 0;

}

double f(double x) {
	return (4.0 / (1.0 + x * x));
}