// ex3.cpp
#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include <omp.h> // Biblioteca para OpenMP

int main(int argc, char* argv[]) {
    int rank, size;
    const int N = 1000; // Tamanho total do array
    std::vector<float> data;
    int n_per_proc;
    float max_value = 0.0f;

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

        // Encontra o valor máximo usando OpenMP
        max_value = data[0];
        #pragma omp parallel for reduction(max:max_value)
        for (int i = 0; i < N; i++) {
            if (data[i] > max_value) {
                max_value = data[i];
            }
        }
    }

    // Transmite o valor máximo para todos os processos
    MPI_Bcast(&max_value, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Distribui partes do array para todos os processos
    MPI_Scatter(data.data(), n_per_proc, MPI_FLOAT, sub_data.data(), n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cada processo normaliza sua parte do array
    for (int i = 0; i < n_per_proc; i++) {
        sub_data[i] /= max_value;
    }

    // Coleta os arrays normalizados no processo raiz
    MPI_Gather(sub_data.data(), n_per_proc, MPI_FLOAT, data.data(), n_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Processo raiz imprime o resultado
    if (rank == 0) {
        std::cout << "Array normalizado:" << std::endl;
        for (int i = 0; i < N; i++) {
            std::cout << data[i] << " ";
            if ((i + 1) % 10 == 0) std::cout << std::endl;
        }
    }

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
