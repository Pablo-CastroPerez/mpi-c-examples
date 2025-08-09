// A simple parallel (MPI) code that approximates pi using:
// pi ≈ 4 * Σ [1 / (1 + ((i - 0.5)/N)^2)] * (1/N)

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#define N 840 //Total number of integration points. Must be divisible by the number of processes.

int main(void)
{
  double pi, exactpi;

  int i;

  // MPI variables 
  MPI_Comm comm;
  MPI_Status status;

  int rank, size, source, tag;

  // Local computation variables 
  int istart, istop;
  double partialpi, recvpi;

  printf("Computing approximation to pi using N = %d\n", N);

  // Initialise MPI and get number of processes and local rank 
  comm = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  printf("Hello from rank %d\n", rank);

  if (rank == 0) printf("Running on %d process(es)\n", size);

  partialpi = 0.0;

  /*
   * Compute an approximation to pi using a simple series expansion for pi/4 using:
   * pi ≈ 4 * Σ [1 / (1 + ((i - 0.5)/N)^2)] * (1/N)
   * Each process computes a separate section of the summation
   * The program assumes that N is exactly divisible by the number of processes
   */

  istart = N/size * rank + 1;
  istop  = istart + N/size - 1;

  printf("On rank %d istart = %d, istop = %d\n", rank, istart, istop);

  // Compute partial values
	
  for (i=istart; i<=istop; i++)
    {
      partialpi = partialpi +
	          1.0/( 1.0 + pow( (((double) i)-0.5)/((double) N), 2.0) );
    }

  printf("On rank %d partialpi = %f\n", rank, partialpi*4.0/((double) N));

  // Compute global value of pi by sending partial values to rank 0  
  if (rank == 0)
    {
      // Initialise pi to locally computed partial sum 
      pi = partialpi;

      // Add in contribution from other processes 
      for (source = 1; source < size; source++)
	{
	  // Receive partial result from any source (tag = 0)
		
	  tag = 0;

	  MPI_Recv(&recvpi, 1, MPI_DOUBLE, MPI_ANY_SOURCE, tag, comm, &status);

	  printf("rank 0 receiving from rank %d\n", status.MPI_SOURCE);

	  // Add to running total 
	  pi = pi + recvpi;
	}
    }
  else
    {
      // All non-zero ranks send their partial result to rank 0

      tag = 0;

      printf("rank %d sending to rank 0\n", rank);

      MPI_Ssend(&partialpi, 1, MPI_DOUBLE, 0, tag, comm);
    }

  pi = pi * 4.0/((double) N);

  exactpi = 4.0*atan(1.0);
  
  if (rank == 0)
    {
      printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));
    }

  MPI_Finalize();

  return 0;
}
