// Exercício 1: Comunicação entre Dois Processos
// Compilar com: mpic++ -o exercise1 exercise1.cpp
// Executar com Slurm (arquivo submit_exercise1.sh):

#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    const int MAX_STRING = 100;
    char mensagem[MAX_STRING];

    if (rank == 0) {
        // Processo 0 envia "Olá" para o processo 1
        strcpy(mensagem, "Olá");
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0 enviou: " << mensagem << " para o processo 1" << std::endl;
        
        // Processo 0 recebe resposta do processo 1
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu: " << mensagem << " do processo 1" << std::endl;
    }
    else if (rank == 1) {
        // Processo 1 recebe mensagem do processo 0
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 1 recebeu: " << mensagem << " do processo 0" << std::endl;
        
        // Processo 1 envia "Oi" para o processo 0
        strcpy(mensagem, "Oi");
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        std::cout << "Processo 1 enviou: " << mensagem << " para o processo 0" << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
