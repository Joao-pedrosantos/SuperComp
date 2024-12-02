#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

// Function to create the codon-to-amino-acid mapping
std::unordered_map<std::string, int> createCodonMap() {
    return {
        {"CCA", 1}, {"CCG", 1}, {"CCU", 1}, {"CCC", 1}, // Proline -> 1
        {"UCU", 2}, {"UCA", 2}, {"UCG", 2}, {"UCC", 2}, // Serine -> 2
        {"CAG", 3}, {"CAA", 3},                         // Glutamine -> 3
        {"ACA", 4}, {"ACC", 4}, {"ACU", 4}, {"ACG", 4}, // Threonine -> 4
        {"AUG", 5},                                     // Methionine -> 5 (START codon)
        {"UGA", 0},                                     // STOP -> 0
        {"UGC", 6}, {"UGU", 6},                         // Cysteine -> 6
        {"GUG", 7}, {"GUA", 7}, {"GUC", 7}, {"GUU", 7}  // Valine -> 7
    };
}

// Function to translate a sequence of RNA into amino acids
std::vector<std::vector<int>> translateRNA(const std::string& rna, const std::unordered_map<std::string, int>& codonMap) {
    std::vector<std::vector<int>> proteins;
    size_t length = rna.size();
    bool inProtein = false;
    std::vector<int> currentProtein;

    for (size_t i = 0; i <= length - 3; i += 3) {
        std::string codon = rna.substr(i, 3);
        auto it = codonMap.find(codon);

        if (it != codonMap.end()) {
            int aminoAcid = it->second;

            if (!inProtein) {
                // Looking for START codon
                if (codon == "AUG") {
                    inProtein = true;
                    currentProtein.push_back(aminoAcid);
                }
            } else {
                // Inside a protein
                if (aminoAcid == 0) { // STOP codon
                    // End of protein
                    proteins.push_back(currentProtein);
                    currentProtein.clear();
                    inProtein = false;
                } else {
                    currentProtein.push_back(aminoAcid);
                }
            }
        }
    }

    // If at the end we are still in a protein, we can decide whether to keep it or discard it
    if (inProtein && !currentProtein.empty()) {
        proteins.push_back(currentProtein);
    }

    return proteins;
}

int main(int argc, char* argv[]) {
    // Check arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <RNA file> <Chromosome number (1-22)>" << std::endl;
        return 1;
    }

    int chromosomeNumber = std::stoi(argv[2]);
    if (chromosomeNumber < 1 || chromosomeNumber > 22) {
        std::cerr << "Chromosome number must be between 1 and 22." << std::endl;
        return 1;
    }

    // Read the RNA sequence from file
    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::string sequence;
    std::string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] != '>') { // Skip headers
            sequence += line;
        }
    }
    inputFile.close();

    if (sequence.empty()) {
        std::cerr << "Empty RNA sequence." << std::endl;
        return 1;
    }

    // Create codon map
    std::unordered_map<std::string, int> codonMap = createCodonMap();

    // Translate RNA sequence into proteins
    std::vector<std::vector<int>> proteins = translateRNA(sequence, codonMap);

    // Generate the output file name based on chromosome number
    std::string outputFileName = "proteins_chr" + std::to_string(chromosomeNumber) + ".txt";

    // Output the proteins to the file
    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Error creating output file: " << outputFileName << std::endl;
        return 1;
    }

    int proteinIndex = 1;
    for (const auto& protein : proteins) {
        outputFile << proteinIndex << " ";
        for (int aminoAcid : protein) {
            outputFile << aminoAcid;
        }
        outputFile << std::endl;
        proteinIndex++;
    }

    outputFile.close();

    return 0;
}
