// Exercício 6: Alternância de Mensagens
// Compilar com: mpic++ -o exercise6 exercise6.cpp
// Executar com Slurm (arquivo submit_exercise6.sh):

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
        // Processo 0 se comunica com cada processo em ordem
        for (int dest = 1; dest < tamanho; ++dest) {
            // Envia mensagem para o processo dest
            std::stringstream ss;
            ss << "Mensagem para processo " << dest;
            strcpy(mensagem, ss.str().c_str());
            MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: " << mensagem << " para o processo " << dest << std::endl;

            // Recebe resposta do processo dest
            MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, dest, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu: " << mensagem << " do processo " << dest << std::endl;
        }
    } else {
        // Outros processos
        // Recebe mensagem do processo 0
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo 0" << std::endl;

        // Envia resposta de volta para o processo 0
        std::stringstream ss;
        ss << "Resposta do processo " << rank;
        strcpy(mensagem, ss.str().c_str());
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo 0" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
