#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstdlib>

int main(int argc, char** argv) {
    int rank, size;
    int N = 1000; // Tamanho da matriz (pode ser alterado para testes)
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divisão da matriz entre os processos
    int rows_per_proc = N / size;
    int remainder = N % size;
    int start_row = rank * rows_per_proc + (rank < remainder ? rank : remainder);
    int end_row = start_row + rows_per_proc + (rank < remainder ? 1 : 0);

    int local_rows = end_row - start_row;

    // Alocar matrizes locais
    std::vector<std::vector<double>> local_matrix(local_rows, std::vector<double>(N));

    // Inicializar matriz (por simplicidade, usamos valores aleatórios)
    srand(time(NULL) + rank);
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < N; ++j) {
            local_matrix[i][j] = rand() % 10;
        }
    }

    double start_time = MPI_Wtime();

    // Cálculo do quadrado de cada elemento sem OpenMP
    for (int i = 0; i < local_rows; ++i) {
        for (int j = 0; j < N; ++j) {
            local_matrix[i][j] *= local_matrix[i][j];
        }
    }

    double end_time = MPI_Wtime();

    if (rank == 0) {
        std::cout << "Tempo de execução sem OpenMP: " << end_time - start_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
