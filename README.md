# PC_Project
Parallelisation Conway's Game of Life

Compiling main.cpp use: g++ main.cpp -o main
Run with: ./main

Compiling pmain.cpp use: mpic++ pmain.cpp -o pmain
Run with: mpirun -np insert_num_procs ./pmain insert_num_rows insert_num_cols insert_num_generations

