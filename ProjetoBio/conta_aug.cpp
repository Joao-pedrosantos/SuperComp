// conta_aug.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <omp.h>

void processFileAUG(const std::string& inputFileName) {
    // Open the input file
    std::ifstream inputFile(inputFileName);
    if (!inputFile) {
        std::cerr << "Erro ao abrir o arquivo de entrada: " << inputFileName << std::endl;
        return;
    }

    // Extract chromosome number from the input file name
    std::string chrNumber;
    size_t pos1 = inputFileName.find("chr");
    if (pos1 != std::string::npos) {
        size_t pos2 = inputFileName.find(".txt", pos1);
        if (pos2 != std::string::npos) {
            chrNumber = inputFileName.substr(pos1 + 3, pos2 - pos1 - 3);
        } else {
            chrNumber = inputFileName.substr(pos1 + 3);
        }
    } else {
        std::cerr << "Não foi possível extrair o número do cromossomo do arquivo: " << inputFileName << std::endl;
        return;
    }

    // Name of the output file
    std::string outputFileName = "AUG_chr" + chrNumber + ".txt";

    // Open the output file
    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Erro ao criar o arquivo de saída: " << outputFileName << std::endl;
        return;
    }

    std::string line;
    unsigned long long totalAUGCount = 0;

    // Read the sequence from the input file
    std::string sequence;
    while (std::getline(inputFile, line)) {
        if (line.empty()) {
            continue; // Skip empty lines
        }
        // Concatenate lines to form the full sequence
        sequence += line;
    }

    inputFile.close();

    // Remove initial 'N's from the sequence
    size_t pos = 0;
    while (pos < sequence.size() && (sequence[pos] == 'N' || sequence[pos] == 'n')) {
        ++pos;
    }
    sequence = sequence.substr(pos);

    // Check if the sequence is long enough to contain "AUG"
    if (sequence.size() >= 3) {
        // Convert the sequence to uppercase
        #pragma omp parallel for
        for (size_t i = 0; i < sequence.size(); ++i) {
            sequence[i] = toupper(static_cast<unsigned char>(sequence[i]));
        }

        size_t seq_length = sequence.size();
        unsigned long long localAUGCount = 0;

        // Search for "AUG" in the sequence
        #pragma omp parallel for reduction(+:localAUGCount)
        for (size_t i = 0; i <= seq_length - 3; ++i) {
            if (sequence[i] == 'A' && sequence[i + 1] == 'U' && sequence[i + 2] == 'G') {
                localAUGCount++;
            }
        }

        // Update the total count of "AUG"
        totalAUGCount = localAUGCount;
    }

    // Save the total count of "AUG" in the output file
    outputFile << "Arquivo: " << inputFileName << "\n";
    outputFile << "Quantidade de 'AUG' encontrados: " << totalAUGCount << "\n";

    outputFile.close();

    std::cout << "Processamento concluído para " << inputFileName << ". Total de 'AUG': " << totalAUGCount << "\n";
}

int main(int argc, char* argv[]) {
    // Check if the user provided the input file name
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_RNA_de_entrada>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];

    processFileAUG(inputFileName);

    return 0;
}
