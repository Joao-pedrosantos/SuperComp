// ex1.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    const int N = 100; // Tamanho total do array
    std::vector<int> data;
    int n_per_proc;

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    n_per_proc = N / size; // Número de elementos por processo

    std::vector<int> sub_data(n_per_proc); // Aloca memória para o sub-array

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        data.resize(N);
        for (int i = 0; i < N; i++) {
            data[i] = rand() % 100; // Valores entre 0 e 99
        }
    }

    // Distribui partes do array para todos os processos
    MPI_Scatter(data.data(), n_per_proc, MPI_INT, sub_data.data(), n_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo calcula a média local
    int local_sum = 0;
    for (int i = 0; i < n_per_proc; i++) {
        local_sum += sub_data[i];
    }
    double local_mean = static_cast<double>(local_sum) / n_per_proc;

    // Coleta as médias locais no processo raiz
    std::vector<double> local_means;
    if (rank == 0) {
        local_means.resize(size);
    }

    MPI_Gather(&local_mean, 1, MPI_DOUBLE, local_means.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média global
    if (rank == 0) {
        double sum_means = 0.0;
        for (const auto& mean : local_means) {
            sum_means += mean;
        }
        double global_mean = sum_means / size;
        std::cout << "A média global é: " << global_mean << std::endl;
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
