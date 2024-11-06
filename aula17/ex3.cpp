#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <cstdlib>

int main(int argc, char** argv) {
    int rank, size;
    long N = 1000000; // Tamanho do vetor (pode ser alterado)
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cálculo dos elementos para cada processo
    std::vector<long> sendcounts(size);
    std::vector<long> displs(size);

    long base_elements = N / size;
    long extra = N % size;

    long offset = 0;
    for (int i = 0; i < size; ++i) {
        sendcounts[i] = base_elements + (i < extra ? 1 : 0);
        displs[i] = offset;
        offset += sendcounts[i];
    }

    long local_N = sendcounts[rank];

    // Inicialização do vetor
    std::vector<double> local_array(local_N);

    // Opção 1: Inicialização no rank 0 e distribuição
    std::vector<double> array;
    if (rank == 0) {
        array.resize(N);
        srand(time(NULL));
        for (long i = 0; i < N; ++i) {
            array[i] = rand() % 100;
        }
    }

    // Distribuir dados usando MPI_Scatterv
    MPI_Scatterv(array.data(), sendcounts.data(), displs.data(), MPI_DOUBLE,
                 local_array.data(), local_N, MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    double local_sum = 0.0;

    double start_time = MPI_Wtime();

    // Cálculo da soma parcial usando OpenMP
    #pragma omp parallel for reduction(+:local_sum)
    for (long i = 0; i < local_N; ++i) {
        local_sum += local_array[i];
    }

    double end_time = MPI_Wtime();

    // Redução das somas parciais
    double total_sum = 0.0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double mean = total_sum / N;
        std::cout << "Média: " << mean << std::endl;
        std::cout << "Tempo de execução: " << end_time - start_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
