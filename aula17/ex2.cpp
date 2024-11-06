#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <algorithm>

int main(int argc, char** argv) {
    int rank, size;
    int N = 1000; // Tamanho da matriz (pode ser alterado para testes)
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cálculo das linhas para cada processo
    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    int base_rows = N / size;
    int extra = N % size;

    int offset = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = base_rows + (i < extra ? 1 : 0);
        sendcounts[i] *= N; // Número total de elementos para cada processo
        displs[i] = offset;
        offset += sendcounts[i];
    }

    int local_elements = sendcounts[rank];
    int local_rows = local_elements / N;

    // Alocar matriz local
    std::vector<double> local_matrix(local_elements);

    // Inicializar matriz no rank 0
    std::vector<double> matrix;
    if (rank == 0) {
        matrix.resize(N * N);
        srand(time(NULL));
        for (int i = 0; i < N * N; ++i) {
            matrix[i] = rand() % 10;
        }
    }

    // Distribuir dados usando MPI_Scatterv
    MPI_Scatterv(matrix.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                 local_matrix.data(), local_elements, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();

    // Cálculo do quadrado de cada elemento
    for (int i = 0; i < local_elements; ++i) {
        local_matrix[i] *= local_matrix[i];
    }

    double end_time = MPI_Wtime();

    // Coletar resultados (opcional)
    MPI_Gatherv(local_matrix.data(), local_elements, MPI_DOUBLE,
                matrix.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Tempo de execução com proteção contra divisões desiguais: " << end_time - start_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
