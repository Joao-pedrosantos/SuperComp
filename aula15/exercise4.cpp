// Exercício 4: Comunicação Múltipla
// Compilar com: mpic++ -o exercise4 exercise4.cpp
// Executar com Slurm (arquivo submit_exercise4.sh):

#include <mpi.h>
#include <iostream>
#include <sstream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, tamanho;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    const int MAX_STRING = 100;
    char mensagem[MAX_STRING];

    if (rank == 0) {
        // Processo 0 envia mensagens para todos os outros processos
        for (int dest = 1; dest < tamanho; ++dest) {
            std::stringstream ss;
            ss << "Mensagem para o processo " << dest;
            strcpy(mensagem, ss.str().c_str());
            MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: " << mensagem << " para o processo " << dest << std::endl;
        }
    } else {
        // Outros processos recebem suas mensagens
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << std::endl;
    }

    MPI_Finalize();
    return 0;
}
