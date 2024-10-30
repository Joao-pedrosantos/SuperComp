// ex4.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <cmath> // Biblioteca para funções matemáticas

int main(int argc, char* argv[]) {
    int rank, size;
    const int N = 1000; // Tamanho total do array
    std::vector<float> data;
    int n_per_proc;

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    n_per_proc = N / size; // Número de elementos por processo

    std::vector<float> sub_data(n_per_proc); // Aloca memória para o sub-array

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        data.resize(N);
        for (int i = 0; i < N; i++) {
            data[i] = static_cast<float>(rand() % 1000); // Valores entre 0 e 999
        }
    }

    // Distribui partes do array para todos os processos
    MPI_Scatter(data.data(), n_per_proc, MPI_FLOAT, sub_data.data(), n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a soma e a soma dos quadrados de seus elementos
    double local_sum = 0.0;
    double local_sum_sq = 0.0;
    for (int i = 0; i < n_per_proc; i++) {
        local_sum += sub_data[i];
        local_sum_sq += sub_data[i] * sub_data[i];
    }

    // Reduz as somas locais para obter as somas globais no processo raiz
    double global_sum = 0.0;
    double global_sum_sq = 0.0;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_sum_sq, &global_sum_sq, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Processo raiz calcula o desvio padrão global
    if (rank == 0) {
        double global_mean = global_sum / N;
        double variance = (global_sum_sq / N) - (global_mean * global_mean);
        double std_deviation = std::sqrt(variance);

        std::cout << "Média global: " << global_mean << std::endl;
        std::cout << "Desvio padrão global: " << std_deviation << std::endl;
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
