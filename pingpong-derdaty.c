// 2-process MPI program exchanging M columns of an N×N matrix repeatedly

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define N  6    // For an NxN matrix
#define M  3    // Number of columns to be sent together
#define colstart  2    // where colstart is the start column
#define itmax 4 // Number of iterations

// Helper function to print matrix
void matprint(int mat[N][N]);

int main(void)
{
  int rank, size, length;
  int start,stop,i,j,iter;    
  double rbuffer,sbuffer;
  MPI_Comm comm;
  MPI_Status status;

  int sendmatrix[N][N], recvmatrix[N][N];
  MPI_Datatype mcoltype;

  comm  = MPI_COMM_WORLD;

  MPI_Init(NULL, NULL);
  MPI_Comm_rank(comm, &rank);            
  MPI_Comm_size(comm, &size); 

  // Abort if not run on 2 processors.
  if(size != 2)
    {
      if(rank == 0)
        {
          printf("The code can only be run on 2 processes\n");
        }
      MPI_Finalize();
      exit(1);
    }

  // Check that M columns starting at colstart fit within the matrix
  if(M > N || colstart > N-1 ||(colstart+M) > N)
    {
      printf("The number of rows MUST NOT be bigger than the number of coulumns: M=%d, N=%d, colstart=%d \n",M,N,colstart);
      MPI_Finalize();
      exit(1);
    }

  if(rank == 0)
    {
      printf("Sending %d column(s) of an %d x %d matrix, starting with column %d and sending it %d times.\n",M,N,N,colstart,itmax);
    }

  // Create new datatype mcoltype to send M columns and commit it.
  MPI_Type_vector(N,M,N,MPI_INT,&mcoltype);
  MPI_Type_commit(&mcoltype);

  // Initialize matrix
  for(i=0;i<N;i++)
    {
      for(j=0;j<N;j++)
        {
          sendmatrix[i][j] = 1+j+N*i;
          recvmatrix[i][j] = 0;
        }
    }

  if (rank == 0)
    {
      printf("\nSend matrix on rank 0\n\n");
      matprint(sendmatrix);
    }

  // Repeated exchange of M columns between the two ranks
  for(iter=0;iter<itmax;iter++)
    {
      if(rank == 0)
        {
          MPI_Ssend(&(sendmatrix[0][colstart-1]),1,mcoltype,1,50,comm);
          MPI_Recv(&(recvmatrix[0][colstart-1]),1,mcoltype,1,60,comm,&status);
        }
      else
        {
          MPI_Recv(&(recvmatrix[0][colstart-1]),1,mcoltype,0,50,comm,&status);
          MPI_Ssend(&(sendmatrix[0][colstart-1]),1,mcoltype,0,60,comm);
        }

      for(i=0;i<N;i++)
        {
          for(j=colstart-1;j<(colstart+M-1);j++)
            {
              sendmatrix[i][j] = recvmatrix[i][j];
            }
        }
    }

  if (rank == 0)
    {
      printf("\nRecv matrix on rank 0\n\n");
      matprint(recvmatrix);
    }

  MPI_Finalize();
  return 0;
}

// Helper function to print matrix
void matprint(int mat[N][N])
{
  int i, j;
  
  for (i=0; i < N; i++)
    {
      for (j=0; j < N; j++)
        {
          printf(" %3d", mat[i][j]);
        }
      printf("\n");
    }
}
