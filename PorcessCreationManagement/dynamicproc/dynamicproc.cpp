//	dynamicproc.cpp : 定义控制台应用程序的入口点。
//	动态进程管理

/*

#include "mpi.h"
int main( int argc, char **argv ) {

	int rank, size, again;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {

		MPI_Comm client;
		MPI_Status status;
		char port_name[MPI_MAX_PORT_NAME];
		double buf[10];

		MPI_Open_port(MPI_INFO_NULL, port_name);
		printf("server available at %s\n", port_name);
		while (1) {

			MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);
			again = 1;

			while (again) {

				MPI_Recv(buf, 10, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, client, &status);

				switch (status.MPI_TAG) {
				case 0: MPI_Comm_free(&client);
					MPI_Close_port(port_name);
					MPI_Finalize();
					return 0;
				case 1: MPI_Comm_disconnect( &client );
					again = 0;
					break;
				default:
					/ * Unexpected message type * /
					MPI_Abort( MPI_COMM_WORLD, 1 );
				}
			}
		}
	} else if (rank == 1) {

		MPI_Comm server;
		double buf[10];
		char port_name[MPI_MAX_PORT_NAME];
		
		strcpy(port_name, "Test" );/ * assume server's name is cmd-line arg * /
		MPI_Comm_connect( port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD,
			&server );
		while (!done) {
			tag = 2; / * Action to perform * /
			MPI_Send( buf, 10, MPI_DOUBLE, 0, tag, server );
			/ * etc * /
		}
		MPI_Send( buf, 0, MPI_DOUBLE, 0, 1, server );
		MPI_Comm_disconnect( &server );
		MPI_Finalize();
		return 0;
	}
}
*/











#include "stdafx.h"
#include "mpi.h"

int main(int argc, char** argv) {


	int myRank, nProcs;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	int errs = 0, merr;
	char port_name[MPI_MAX_PORT_NAME], port_name_out[MPI_MAX_PORT_NAME];
	char serv_name[256];

	strcpy(serv_name, "MyTest");

	if (myRank == 0) {

		fprintf(stdout, "rank %d, opening port...\n", myRank);
		merr = MPI_Open_port(MPI_INFO_NULL, port_name_out);
		fprintf(stdout, "rank %d, port opened, with <%s>\n", myRank, port_name_out);

		merr = MPI_Publish_name(serv_name, MPI_INFO_NULL, port_name_out);
		if (merr) {
			err++;
			MPI_Error_string(merr, err)
		}

	} else if (myRank == 1) {



	}


	MPI_Finalize();
	return 0;

}