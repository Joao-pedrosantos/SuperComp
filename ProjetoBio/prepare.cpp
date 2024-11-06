#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <omp.h>

void processFile(const std::string& inputFileName, const std::string& outputFileName) {
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

    // Variáveis para armazenar a contagem total de cada base
    unsigned long long total_counts[4] = {0, 0, 0, 0}; // Índices: 0-A, 1-T, 2-C, 3-G

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

    // Imprime a contagem total de bases
    std::cout << "Contagem de bases no arquivo " << inputFileName << ":\n";
    std::cout << "A: " << total_counts[0] << "\n";
    std::cout << "T: " << total_counts[1] << "\n";
    std::cout << "C: " << total_counts[2] << "\n";
    std::cout << "G: " << total_counts[3] << "\n";
}

int main(int argc, char* argv[]) {
    // Verifica se o usuário forneceu o nome do arquivo
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = inputFileName + "_processed.fa";

    processFile(inputFileName, outputFileName);

    return 0;
}
