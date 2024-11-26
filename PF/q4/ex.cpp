#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <ctime>

#define tamanho 1000

void cria_matriz(std::vector<std::vector<double>>& matriz) {
    #pragma omp parallel for
    for (int i = 0; i < tamanho; ++i) {
        unsigned int seed = omp_get_thread_num() + time(NULL);
        for (int j = 0; j < tamanho; ++j) {
            matriz[i][j] = static_cast<double>(rand_r(&seed)) / RAND_MAX;
        }
    }
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (tamanho % size != 0) {
        if (rank == 0) {
            std::cerr << "Erro" << std::endl;
        }
        MPI_Finalize();
        return -1;
    }

    int rows_per_process = tamanho / size;

    std::vector<std::vector<double>> matriz;
    
    std::vector<double> flat_matriz;

    if (rank == 0) {
        matriz.resize(tamanho, std::vector<double>(tamanho));
        cria_matriz(matriz);

        flat_matriz.resize(tamanho * tamanho);
        for (int i = 0; i < tamanho; ++i) {
            std::copy(matriz[i].begin(), matriz[i].end(), flat_matriz.begin() + i * tamanho);
        }
    }

    std::vector<double> matriz_flat_local(rows_per_process * tamanho);
    MPI_Scatter(
        rank == 0 ? flat_matriz.data() : nullptr,
        rows_per_process * tamanho,
        MPI_DOUBLE,
        matriz_flat_local.data(),
        rows_per_process * tamanho,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD
    );

    std::vector<std::vector<double>> matriz_local(rows_per_process, std::vector<double>(tamanho));

    for (int i = 0; i < rows_per_process; ++i) {
        std::copy(
            matriz_flat_local.begin() + i * tamanho,
            matriz_flat_local.begin() + (i + 1) * tamanho,
            matriz_local[i].begin()
        );
    }

    std::vector<double> local_means(rows_per_process);
    #pragma omp parallel for
    for (int i = 0; i < rows_per_process; ++i) {
        double sum = std::accumulate(matriz_local[i].begin(), matriz_local[i].end(), 0.0);
        local_means[i] = sum / tamanho;
    }

    std::vector<double> global_means;
    if (rank == 0) {
        global_means.resize(tamanho);
    }
    MPI_Gather(
        local_means.data(),
        rows_per_process,
        MPI_DOUBLE,
        rank == 0 ? global_means.data() : nullptr,
        rows_per_process,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0) {
        double media = std::accumulate(global_means.begin(), global_means.end(), 0.0) / tamanho;
        std::cout << "Média: " << media << std::endl;
    }

    MPI_Finalize();
    return 0;
}