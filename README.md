# Transpose a Matrix Across MPI Ranks

In this problem, you will implement similar functionality to `MPI_Alltoall` using point-to-point communication calls (i.e. `MPI_Send`, `MPI_Irecv`, etc.).

You will find starting code in `src/main.cpp`.
This code creates a matrix in which each MPI rank stores a subset of rows in a matrix.
As discussed in the lectures, an all-to-all operation effectively performs a transpose of a matrix.
Write your code in the location indicated in `src/main.cpp`.
Each rank has a vector named `my_partial_matrix`, which stores the elements of the matrix owned by that rank.
The elements of the transposed matrix which are owned by each rank should be stored in the vector named `my_partial_transpose`.
The code prints both the `my_partial_matrix` and `my_partial_transpose` vectors, so you should be able to confirm the validity of your implementation.

You can compile and run the code with:

```bash
cmake -S . -B build
cmake --build build
```

You can run the code with, for example:

```bash
mpiexec -n 4 ./build/transpose
```

Note that there is an overhead cost associated with every point-to-point communication call you make.
In order to achieve reasonable efficiency, ensure that your implementation does not make an excessive number of point-to-point communication calls; in particular, the complete set of all ranks should make no more than a total of `2 * world_size * world_size` point-to-point communication calls.

After performing the point-to-point transpose, perform the same operation using a call to `MPI_Alltoall`.
Time your point-to-point transpose as well as the MPI_Alltoall for several different numbers of ranks.
Report your result below.

## Answer
