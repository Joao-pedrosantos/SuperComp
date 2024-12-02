#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <omp.h>
#include <mpi.h>

void processFile(const std::string& inputFileName, const std::string& outputFileName,
                 unsigned long long total_counts[4], const std::string& countFileName) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << inputFileName << std::endl;
        return;
    }

    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Erro ao criar o arquivo de saída: " << outputFileName << std::endl;
        return;
    }

    std::string line;
    bool isSequence = false;

    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Ignora linhas vazias
        }

        // Se a linha é um cabeçalho (começa com '>'), escreve sem alterações
        if (line[0] == '>') {
            outputFile << line << '\n';
            isSequence = true; // Próximas linhas são sequências
        } else if (isSequence) {
            // Remove sequência inicial de 'N's
            size_t pos = 0;
            while (pos < line.size() && (line[pos] == 'N' || line[pos] == 'n')) {
                ++pos;
            }
            std::string sequence = line.substr(pos);

            size_t seq_length = sequence.size();

            // Contagem local das bases para cada thread
            unsigned long long local_counts[4] = {0, 0, 0, 0}; // Índices: 0-A, 1-T, 2-C, 3-G

            // Processa a sequência em paralelo
            #pragma omp parallel for reduction(+:local_counts[:4])
            for (size_t i = 0; i < seq_length; ++i) {
                char c = toupper(static_cast<unsigned char>(sequence[i]));
                sequence[i] = c;

                if (c == 'A') {
                    local_counts[0]++;
                } else if (c == 'T') {
                    local_counts[1]++;
                } else if (c == 'C') {
                    local_counts[2]++;
                } else if (c == 'G') {
                    local_counts[3]++;
                }
            }

            // Atualiza a contagem total
            for (int i = 0; i < 4; ++i) {
                total_counts[i] += local_counts[i];
            }

            outputFile << sequence << '\n';
        }
    }

    inputFile.close();
    outputFile.close();

    // Salva a contagem total de bases para este arquivo em um arquivo separado
    std::ofstream countFile(countFileName);
    if (!countFile) {
        std::cerr << "Erro ao criar o arquivo de contagem: " << countFileName << std::endl;
        return;
    }
    countFile << "Contagem de bases no arquivo " << inputFileName << ":\n";
    countFile << "A: " << total_counts[0] << "\n";
    countFile << "T: " << total_counts[1] << "\n";
    countFile << "C: " << total_counts[2] << "\n";
    countFile << "G: " << total_counts[3] << "\n";
    countFile.close();
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verifica se o usuário forneceu os nomes dos arquivos
    if (argc < 2) {
        if (rank == 0) {
            std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada1> [arquivo_de_entrada2] ..." << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    int numFiles = argc - 1;
    std::vector<std::string> inputFiles;

    for (int i = 1; i < argc; ++i) {
        inputFiles.push_back(argv[i]);
    }

    // Distribui os arquivos entre os processos
    std::vector<std::string> localFiles;
    for (int i = 0; i < numFiles; ++i) {
        if (i % size == rank) {
            localFiles.push_back(inputFiles[i]);
        }
    }

    // Processa cada arquivo local
    for (size_t idx = 0; idx < localFiles.size(); ++idx) {
        const auto& inputFileName = localFiles[idx];
        std::string outputFileName = inputFileName + "_processed.fa";

        // Cria um nome para o arquivo de contagem
        std::string countFileName = "count_" + std::to_string(rank) + "_" + std::to_string(idx) + ".txt";

        unsigned long long total_counts[4] = {0, 0, 0, 0}; // Índices: 0-A, 1-T, 2-C, 3-G

        processFile(inputFileName, outputFileName, total_counts, countFileName);
    }

    MPI_Finalize();
    return 0;
}
