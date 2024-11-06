#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <cstdlib>

int main(int argc, char** argv) {
    int rank, size;
    long N = 1000000; // Tamanho do vetor (pode ser alterado)
    int target = 50;  // Valor a ser buscado (pode ser alterado)
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
    std::vector<int> local_array(local_N);

    // Inicialização no rank 0 e distribuição
    std::vector<int> array;
    if (rank == 0) {
        array.resize(N);
        srand(time(NULL));
        for (long i = 0; i < N; ++i) {
            array[i] = rand() % 100;
        }
    }

    // Distribuir dados usando MPI_Scatterv
    MPI_Scatterv(array.data(), sendcounts.data(), displs.data(), MPI_INT,
                 local_array.data(), local_N, MPI_INT,
                 0, MPI_COMM_WORLD);

    // Vetor para armazenar posições locais encontradas
    std::vector<long> local_positions;

    double start_time = MPI_Wtime();

    // Busca paralela usando OpenMP
    #pragma omp parallel
    {
        std::vector<long> thread_positions;

        #pragma omp for nowait
        for (long i = 0; i < local_N; ++i) {
            if (local_array[i] == target) {
                thread_positions.push_back(i + displs[rank]); // Posição global
            }
        }

        // Mesclar resultados das threads
        #pragma omp critical
        local_positions.insert(local_positions.end(), thread_positions.begin(), thread_positions.end());
    }

    double end_time = MPI_Wtime();

    // Primeiro, cada processo envia o número de posições encontradas
    int local_count = local_positions.size();
    std::vector<int> counts(size);
    MPI_Gather(&local_count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Agora, coletamos as posições encontradas no rank 0
    std::vector<int> displs_positions;
    std::vector<long> all_positions;
    if (rank == 0) {
        displs_positions.resize(size);
        int total_positions = 0;
        for (int i = 0; i < size; ++i) {
            displs_positions[i] = total_positions;
            total_positions += counts[i];
        }
        all_positions.resize(total_positions);
    }

    MPI_Gatherv(local_positions.data(), local_count, MPI_LONG,
                all_positions.data(), counts.data(), displs_positions.data(), MPI_LONG,
                0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Valor " << target << " encontrado nas posições: ";
        for (size_t i = 0; i < all_positions.size(); ++i) {
            std::cout << all_positions[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Tempo de execução: " << end_time - start_time << " segundos." << std::endl;
    }

    MPI_Finalize();
    return 0;
}
