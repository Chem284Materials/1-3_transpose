cmake_minimum_required(VERSION 3.22)

# Project name and version
project(TransposeProblem VERSION 1.0
        DESCRIPTION "Evaluation of square root precision"
        LANGUAGES CXX)

# Add the executable
add_executable(transpose src/main.cpp)

# Specify the C++ standard
target_compile_features(transpose PUBLIC cxx_std_11)

# Find MPI
find_package(MPI REQUIRED)
target_link_libraries(transpose PUBLIC MPI::MPI_CXX)
