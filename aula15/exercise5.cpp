// Exercício 5: Somas Paralelas
// Compilar com: mpic++ -o exercise5 exercise5.cpp
// Executar com Slurm (arquivo submit_exercise5.sh):

#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, tamanho;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    int meu_valor = rank + 1; // Valor exclusivo de cada processo
    int soma = 0;

    if (rank == 0) {
        // Processo 0 inicia a soma
        soma = meu_valor;
        MPI_Send(&soma, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou soma parcial " << soma << " para o processo " << rank+1 << std::endl;
    } else if (rank == tamanho - 1) {
        // Último processo recebe e exibe a soma total
        MPI_Recv(&soma, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        soma += meu_valor;
        std::cout << "Processo " << rank << " recebeu soma parcial " << soma - meu_valor << " do processo " << rank-1 << std::endl;
        std::cout << "Processo " << rank << " soma seu valor " << meu_valor << " para obter soma total " << soma << std::endl;
        std::cout << "A soma total é " << soma << std::endl;
    } else {
        // Processos intermediários
        MPI_Recv(&soma, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu soma parcial " << soma << " do processo " << rank-1 << std::endl;
        soma += meu_valor;
        MPI_Send(&soma, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " soma seu valor " << meu_valor << " e envia soma parcial " << soma << " para o processo " << rank+1 << std::endl;
    }

    MPI_Finalize();
    return 0;
}
