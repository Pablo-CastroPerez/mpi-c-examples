// Simple program to implement a scatter using point-to-point communications. 
// It replicates the behaviour of MPI_Scatter().
 
#include <stdio.h>
#include <mpi.h>

// Helper function to print an array
void printarray(int rank, int *array, int n);

int main(void)
{
  int i, src, dest, rank, size, root, tag, count;
  MPI_Comm comm;
  MPI_Status status;
  
  const int N = 12;

  int x[N];

  comm = MPI_COMM_WORLD;
  root = 0;   // The process that contains the data to be broadcast

  MPI_Init(NULL, NULL);

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  if (rank == 0)
    {
      printf("Running scatter program on %d processes\n", size);
    }
	
  // Initialise data
  for (i=0; i<N; i++)
    {
      if (rank == root)
	{
	  x[i] = i;
	}
      else
	{
	  x[i] = -1;
	}
    }

  printarray(rank, x, N);

  count = N/size; // Number of elements per process. Assumes N is divisible by the number of processes, otherwise data would be left over. 

  tag = 0;

  if (rank == root)
    {
      for (dest=0; dest < size; dest++)
	{
	  if (dest != root)
	    {
	      MPI_Ssend(&x[dest*count], count, MPI_INT, dest, tag, comm);
	    }
	}
    }
  else
    {
      MPI_Recv(x, count, MPI_INT, root, tag, comm, &status);
    }
    
  printarray(rank, x, N);

  MPI_Finalize();

  return 0;
}  

// Helper function to print an array
void printarray(int rank, int *array, int n)
{
  int i;
  printf("On rank %d, array[] = [", rank);
  for (i=0; i < n; i++)
    {
      if (i != 0) printf(",");
      printf(" %d", array[i]);
    }
  printf(" ]\n");
}
