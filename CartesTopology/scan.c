// Code to compute the prefix (partial) sum of data stored on each process
// using the collective operation MPI_Scan. Each rank's result is the sum
// of its own value and all values from lower-ranked processes.
// Use virtual topologies, here cartesian 1D.


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>


int main(void)
{
  int rank, size;
  int passon, sum;
  MPI_Comm comm;

  comm  = MPI_COMM_WORLD;

  MPI_Init(NULL,NULL);

  MPI_Comm_size(comm, &size); 
  MPI_Comm_rank(comm,&rank);

  sum = 0;

  // Initialise local values to:
  passon = rank;


  // Use MPI_scan
  MPI_Scan(&passon,&sum,1,MPI_INT,MPI_SUM,comm);

  printf( "The sum is: %d on rank %d \n",sum, rank);
  MPI_Finalize();

  return 0;
}
