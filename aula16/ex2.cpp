// ex2.cpp
#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    int num_iterations = 0;
    double result = 0.0;

    MPI_Init(&argc, &argv); // Inicializa o ambiente MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtém o rank do processo
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtém o número total de processos

    // Processo raiz define a configuração
    if (rank == 0) {
        num_iterations = 1000000; // Número de iterações para o cálculo intensivo
    }

    // Transmite a configuração para todos os processos
    MPI_Bcast(&num_iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa o cálculo com o número especificado de iterações
    for (int i = 0; i < num_iterations; i++) {
        result += static_cast<double>(rank) / (i + 1); // Cálculo de exemplo
    }

    // Cada processo imprime seus resultados locais
    std::cout << "Processo " << rank << ": resultado local = " << result << std::endl;

    MPI_Finalize(); // Finaliza o ambiente MPI
    return 0;
}
