// A parallel version, using MPI, of the typical 'Hello World' program.  

#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

int main(void)
{
  int rank, size, ierr;
  MPI_Comm comm;

  // Use the global communicator for all processes
  comm  = MPI_COMM_WORLD;

  MPI_Init(NULL,NULL); 
  MPI_Comm_rank(comm, &rank);     
  MPI_Comm_size(comm, &size);  

  // Only processor 0 prints 
  if(rank == 0)
    {
      printf("Hello World!\n"); 
    }

  // Each process prints out its rank out of the total number of processes
  printf("I am rank %d out of %d\n", rank, size);

  MPI_Finalize();

  return 0;
}
