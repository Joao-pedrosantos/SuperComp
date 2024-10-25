// Exercício 3: Difusão Linear
// Compilar com: mpic++ -o exercise3 exercise3.cpp
// Executar com Slurm (arquivo submit_exercise3.sh):

#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, tamanho;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    const int MAX_STRING = 100;
    char mensagem[MAX_STRING];

    if (rank == 0) {
        // Processo 0 inicia a difusão
        strcpy(mensagem, "Mensagem inicial");
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo " << rank+1 << std::endl;
    } else if (rank == tamanho -1) {
        // Último processo recebe e exibe a mensagem
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo " << rank-1 << std::endl;
        std::cout << "Processo " << rank << " exibe a mensagem: " << mensagem << std::endl;
    } else {
        // Processos intermediários
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo " << rank-1 << std::endl;
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, rank+1, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo " << rank+1 << std::endl;
    }

    MPI_Finalize();
    return 0;
}
