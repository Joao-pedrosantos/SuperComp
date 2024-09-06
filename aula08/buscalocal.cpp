#include <iostream>
#include <vector>
#include <algorithm>    // std::shuffle, std::max
#include <random>       // std::default_random_engine
#include <chrono>       // Para medir o tempo de execução
#include <fstream>      // Para ler arquivos
#include <string>       // Para manipular strings
#include <sstream>      // Para parse de strings

struct Item {
    int peso;
    int valor;
};

// Função para ler itens de um arquivo
bool lerItensDoArquivo(const std::string& nomeArquivo, std::vector<Item>& itens, int& capacidadeMochila) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
        return false;
    }

    itens.clear();
    capacidadeMochila = 0;

    std::string linha;
    while (std::getline(arquivo, linha)) {
        // Ignora linhas vazias
        if (linha.empty()) continue;

        std::istringstream iss(linha);
        int peso, valor;
        if (!(iss >> peso >> valor)) {
            std::cerr << "Formato inválido no arquivo: " << nomeArquivo << " Linha: " << linha << std::endl;
            return false;
        }
        itens.push_back(Item{peso, valor});
    }

    arquivo.close();

    // Defina a capacidade da mochila
    // Aqui, você pode definir uma capacidade fixa ou determinar com base nos dados
    // Para este exemplo, vamos definir como 50% do peso total disponível
    int pesoTotal = 0;
    for (const auto& item : itens) {
        pesoTotal += item.peso;
    }
    capacidadeMochila = pesoTotal / 2;

    return true;
}

// Heurística 1: Embaralhar e Preencher a Mochila
int shuffleAndFillKnapsack(std::vector<Item> itens, int maxWeight) {
    // Embaralha os itens
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(itens.begin(), itens.end(), rng);

    int totalPeso = 0;
    int totalValor = 0;

    for (const auto& item : itens) {
        if (totalPeso + item.peso <= maxWeight) {
            totalPeso += item.peso;
            totalValor += item.valor;
        }
    }
    return totalValor;
}

// Heurística 2: Seleção Aleatória Baseada em Probabilidade
int probabilisticSelectionKnapsack(const std::vector<Item>& itens, int maxWeight, double threshold) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int totalPeso = 0;
    int totalValor = 0;

    for (const auto& item : itens) {
        double prob = dist(rng);
        if (prob > threshold && totalPeso + item.peso <= maxWeight) {
            totalPeso += item.peso;
            totalValor += item.valor;
        }
    }
    return totalValor;
}

// Método de Busca Exaustiva (Recursivo)
int buscaExaustiva(const std::vector<Item>& itens, int n, int maxWeight, int indice = 0, int pesoAtual = 0, int valorAtual = 0) {
    if (indice == n) {
        return valorAtual;
    }

    // Não incluir o item atual
    int valorSem = buscaExaustiva(itens, n, maxWeight, indice + 1, pesoAtual, valorAtual);

    // Incluir o item atual, se possível
    int valorCom = 0;
    if (pesoAtual + itens[indice].peso <= maxWeight) {
        valorCom = buscaExaustiva(itens, n, maxWeight, indice + 1, pesoAtual + itens[indice].peso, valorAtual + itens[indice].valor);
    }

    return std::max(valorSem, valorCom);
}

int main() {
    std::vector<std::string> arquivosEntrada = {"entrada1.txt", "entrada.txt", "entrada3.txt", "entrada4.txt"};

    // Parâmetros das heurísticas
    const int numExecucoes = 5;
    const double threshold = 0.5;

    for (const auto& arquivo : arquivosEntrada) {
        std::vector<Item> itens;
        int capacidadeMochila = 0;

        std::cout << "Processando arquivo: " << arquivo << std::endl;

        if (!lerItensDoArquivo(arquivo, itens, capacidadeMochila)) {
            std::cerr << "Falha ao ler o arquivo: " << arquivo << ". Pulando para o próximo." << std::endl;
            continue;
        }

        std::cout << "Capacidade da Mochila: " << capacidadeMochila << std::endl;
        std::cout << "Número de Itens: " << itens.size() << std::endl;

        // 1. Busca Exaustiva
        std::cout << "\n--- Busca Exaustiva ---" << std::endl;
        auto start_exaustiva = std::chrono::high_resolution_clock::now();
        int valorExaustivo = buscaExaustiva(itens, itens.size(), capacidadeMochila);
        auto end_exaustiva = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> tempoExaustivo = end_exaustiva - start_exaustiva;
        std::cout << "Valor Total (Exaustiva): " << valorExaustivo << std::endl;
        std::cout << "Tempo de Execução (Exaustiva): " << tempoExaustivo.count() << " segundos" << std::endl;

        // 2. Heurística 1: Embaralhar e Preencher a Mochila
        std::cout << "\n--- Heurística 1: Embaralhar e Preencher a Mochila ---" << std::endl;
        int melhorValorHeuristica1 = 0;
        double tempoTotalHeuristica1 = 0.0;

        for (int i = 0; i < numExecucoes; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            int valor = shuffleAndFillKnapsack(itens, capacidadeMochila);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            tempoTotalHeuristica1 += elapsed.count();
            melhorValorHeuristica1 = std::max(melhorValorHeuristica1, valor);

            std::cout << "Execução " << i + 1 << ": Valor = " << valor << ", Tempo = " << elapsed.count() << " segundos" << std::endl;
        }
        std::cout << "Melhor Valor Heurística 1: " << melhorValorHeuristica1 << std::endl;
        std::cout << "Tempo Médio Heurística 1: " << tempoTotalHeuristica1 / numExecucoes << " segundos" << std::endl;

        // 3. Heurística 2: Seleção Aleatória Baseada em Probabilidade
        std::cout << "\n--- Heurística 2: Seleção Aleatória Baseada em Probabilidade ---" << std::endl;
        int melhorValorHeuristica2 = 0;
        double tempoTotalHeuristica2 = 0.0;

        for (int i = 0; i < numExecucoes; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            int valor = probabilisticSelectionKnapsack(itens, capacidadeMochila, threshold);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            tempoTotalHeuristica2 += elapsed.count();
            melhorValorHeuristica2 = std::max(melhorValorHeuristica2, valor);

            std::cout << "Execução " << i + 1 << ": Valor = " << valor << ", Tempo = " << elapsed.count() << " segundos" << std::endl;
        }
        std::cout << "Melhor Valor Heurística 2: " << melhorValorHeuristica2 << std::endl;
        std::cout << "Tempo Médio Heurística 2: " << tempoTotalHeuristica2 / numExecucoes << " segundos" << std::endl;

        // Comparação
        std::cout << "\n--- Comparação dos Métodos ---" << std::endl;
        std::cout << "Valor Exaustivo: " << valorExaustivo << std::endl;
        std::cout << "Melhor Valor Heurística 1: " << melhorValorHeuristica1 << std::endl;
        std::cout << "Melhor Valor Heurística 2: " << melhorValorHeuristica2 << std::endl;

        std::cout << "\nTempo Exaustivo: " << tempoExaustivo.count() << " segundos" << std::endl;
        std::cout << "Tempo Médio Heurística 1: " << tempoTotalHeuristica1 / numExecucoes << " segundos" << std::endl;
        std::cout << "Tempo Médio Heurística 2: " << tempoTotalHeuristica2 / numExecucoes << " segundos" << std::endl;

        std::cout << "\n========================================\n" << std::endl;
    }

    return 0;
}
