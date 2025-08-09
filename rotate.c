// A ring-based MPI program to compute the global sum of data stored on each process.
// Each process sends its current value to the right and receives from the left.
// Over 'size' iterations, all values are circulated around the ring and summed.

#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

int main(void)
{
  int rank, size, tag;
  int start,stop,i;
  int left, right, addon, passon, sum;
  MPI_Comm comm;
  MPI_Status status;
  MPI_Request request;

  comm  = MPI_COMM_WORLD;

  tag = 1;

  MPI_Init(NULL,NULL);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  //Send current number to the right and receive from the left
  left = rank - 1;
  right = rank + 1;

  if (rank == size-1)
    {
      right = 0;
    }
  if(rank == 0)
    {
      left = size - 1;
    }

  //Intitialise running total
  sum = 0;

  // Initialise local values to:
  passon = rank;

  // Use non-blocking point-to-point communication
  for(i=0;i<size;i++)
    {
      MPI_Issend(&passon,1,MPI_INT,right,tag,comm,&request);
      MPI_Recv(&addon,1,MPI_INT,left,tag,comm,&status);
      MPI_Wait(&request,&status);

      sum = sum + addon;
      passon = addon;
  }
  printf( "The sum is %d on rank %d\n", sum, rank);
  MPI_Finalize();

  return 0;
}
