// dna_para_rna.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <omp.h>

void processFileRNA(const std::string& inputFileName) {
    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << inputFileName << std::endl;
        return;
    }

    // Nome do arquivo de saída baseado no nome do arquivo de entrada
    std::string outputFileName = "RNA_" + inputFileName + ".txt";

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

            // Processa a sequência em paralelo
            #pragma omp parallel for
            for (size_t i = 0; i < seq_length; ++i) {
                char c = toupper(static_cast<unsigned char>(sequence[i]));
                if (c == 'T') {
                    sequence[i] = 'U';
                } else {
                    sequence[i] = c;
                }
            }

            outputFile << sequence << '\n';
        }
    }

    inputFile.close();
    outputFile.close();
}

int main(int argc, char* argv[]) {
    // Verifica se o usuário forneceu o nome do arquivo
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];

    processFileRNA(inputFileName);

    return 0;
}
