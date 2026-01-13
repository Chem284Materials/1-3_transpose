#include <iostream>
#include <mpi.h>
#include <vector>

int main(int argc, char *argv[]) {
  int N = 10; // Number of rows
  int M = 10; // Number of columns

  MPI_Init(&argc, &argv);

  int world_size, my_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  // We will construct a matrix that is split across ranks
  // Each rank only stores the set of rows from start_rows[my_rank] (inclusive) to end_rows[my_rank] (exclusive)
  int start_rows[world_size];
  int end_rows[world_size];
  for (int irank = 0; irank < world_size; ++irank) {
    if ( irank == 0 ) { start_rows[irank] = 0; }
    else { start_rows[irank] = end_rows[irank - 1]; }
    end_rows[irank] = start_rows[irank] + ( N / world_size );
    if ( irank < N % world_size ) ++end_rows[irank];
  }

  std::vector<std::vector<int>> my_partial_matrix(world_size, std::vector<int>(M));
  for (int irow = start_rows[my_rank]; irow < end_rows[my_rank]; ++irow) {
    for (int icolumn = 0; icolumn < M; ++icolumn) {
      my_partial_matrix[irow - start_rows[my_rank]][icolumn] = icolumn + ( M * my_rank );
    }
  }

  // YOUR CODE GOES HERE
  // Use point-to-point communication calls to transpose the matrix across ranks

  MPI_Finalize();
  return 0;
}
