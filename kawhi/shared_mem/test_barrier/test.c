
#include <mpi.h>
#include <unistd.h>

int main (int argc , char *argv[])
{
  int rank;

  MPI_Init (&argc, &argv);      /* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id */

  sleep(5*rank); // make sure each process waits for different amount of time
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();
  return 0;
}