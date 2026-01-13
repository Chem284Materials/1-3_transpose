#include <chrono>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <thread>
#include <vector>

int main(int argc, char *argv[]) {
  int N = 11; // Number of rows before transpose
  int M = 5;  // Number of columns before transpose

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

  std::vector<std::vector<int>> my_partial_matrix(end_rows[my_rank] - start_rows[my_rank], std::vector<int>(M));
  for (int irow = start_rows[my_rank]; irow < end_rows[my_rank]; ++irow) {
    for (int icolumn = 0; icolumn < M; ++icolumn) {
      my_partial_matrix[irow - start_rows[my_rank]][icolumn] = icolumn + ( M * irow );
    }
  }

  // Print the matrix
  if ( my_rank == 0 ) {
    std::cout << "Matrix before transpose: " << '\n';
  }
  for (int irank = 0; irank < world_size; ++irank) {
    if ( my_rank == irank ) {
      for (int irow = start_rows[my_rank]; irow < end_rows[my_rank]; ++irow) {
        for (int icolumn = 0; icolumn < M; ++icolumn) {
          std::cout << std::setw(3) << my_partial_matrix[irow - start_rows[my_rank]][icolumn] << " ";
        }
        std::cout << '\n';
      }
    }
    // Note: MPI_Barrier does not ensure that I/O buffers have been written, so the result of this is technically undefined
    // The call to sleep_for should largely ensure that the printing is well behaved
    MPI_Barrier(MPI_COMM_WORLD);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  if ( my_rank == 0 ) {
    std::cout << '\n';
  }

  // In the transposed matrix, each rank will only store the set of rows from start_rows_transpose[my_rank] (inclusive) to end_rows_transpose[my_rank] (exclusive)
  int start_rows_transpose[world_size];
  int end_rows_transpose[world_size];
  for (int irank = 0; irank < world_size; ++irank) {
    if ( irank == 0 ) { start_rows_transpose[irank] = 0; }
    else { start_rows_transpose[irank] = end_rows_transpose[irank - 1]; }
    end_rows_transpose[irank] = start_rows_transpose[irank] + ( M / world_size );
    if ( irank < M % world_size ) ++end_rows_transpose[irank];
  }

  // This vector should hold the transposed matrix
  std::vector<std::vector<int>> my_partial_transpose(end_rows_transpose[my_rank] - start_rows_transpose[my_rank], std::vector<int>(N));



  // YOUR CODE GOES HERE



  // Print the transposed matrix
  if ( my_rank == 0 ) {
    std::cout << "Matrix after transpose: " << '\n';
  }
  for (int irank = 0; irank < world_size; ++irank) {
    if ( my_rank == irank ) {
      for (int irow = start_rows_transpose[my_rank]; irow < end_rows_transpose[my_rank]; ++irow) {
        for (int icolumn = 0; icolumn < N; ++icolumn) {
          std::cout << std::setw(3) << my_partial_transpose[irow - start_rows_transpose[my_rank]][icolumn] << " ";
        }
        std::cout << '\n';
      }
    }
    // Note: MPI_Barrier does not ensure that I/O buffers have been written, so the result of this is technically undefined
    // The call to sleep_for should largely ensure that the printing is well behaved
    MPI_Barrier(MPI_COMM_WORLD);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  if ( my_rank == 0 ) {
    std::cout << '\n';
  }

  MPI_Finalize();
  return 0;
}
