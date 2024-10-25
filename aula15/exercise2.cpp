// Exercício 2: Anel de Processos
// Compilar com: mpic++ -o exercise2 exercise2.cpp
// Executar com Slurm (arquivo submit_exercise2.sh):

#include <mpi.h>
#include <iostream>
#include <sstream>
#include <cstring>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, tamanho;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

    if (tamanho < 3) {
        if (rank == 0) {
            std::cerr << "O número de processos deve ser maior que 2." << std::endl;
        }
        MPI_Finalize();
        return 0;
    }

    int proximo = (rank + 1) % tamanho;
    int anterior = (rank - 1 + tamanho) % tamanho;

    const int MAX_STRING = 100;
    char mensagem[MAX_STRING];

    if (rank == 0) {
        // Processo 0 inicia o anel
        strcpy(mensagem, "Mensagem do processo 0");
        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, proximo, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo " << proximo << std::endl;

        // Recebe do último processo para fechar o anel
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, anterior, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo " << anterior << std::endl;
    } else {
        // Processos intermediários
        MPI_Recv(mensagem, MAX_STRING, MPI_CHAR, anterior, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo " << anterior << std::endl;

        // Opcionalmente, modificar a mensagem
        std::stringstream ss;
        ss << "Mensagem do processo " << rank;
        strcpy(mensagem, ss.str().c_str());

        MPI_Send(mensagem, strlen(mensagem)+1, MPI_CHAR, proximo, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo " << proximo << std::endl;
    }

    MPI_Finalize();
    return 0;
}
