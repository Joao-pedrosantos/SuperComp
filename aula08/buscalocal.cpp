#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>
#include <random>

// Definição de uma estrutura para armazenar os itens
struct Item {
    int peso;
    int valor;
};

// Função para calcular o valor total dos itens na mochila
int calculaValor(const std::vector<Item>& itens, const std::vector<bool>& mochila) {
    int valorTotal = 0;
    for (size_t i = 0; i < itens.size(); ++i) {
        if (mochila[i]) {
            valorTotal += itens[i].valor;
        }
    }
    return valorTotal;
}

// Função para calcular o peso total dos itens na mochila
int calculaPeso(const std::vector<Item>& itens, const std::vector<bool>& mochila) {
    int pesoTotal = 0;
    for (size_t i = 0; i < itens.size(); ++i) {
        if (mochila[i]) {
            pesoTotal += itens[i].peso;
        }
    }
    return pesoTotal;
}

// Função para gerar uma solução aleatória
std::vector<bool> geraSolucaoAleatoria(const std::vector<Item>& itens, int capacidade) {
    std::vector<bool> solucao(itens.size(), false);
    int pesoAtual = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            solucao[i] = (rand() % 2 == 1); // Escolhe aleatoriamente se o item será incluído
            if (solucao[i]) {
                pesoAtual += itens[i].peso;
            }
        }
    }
    return solucao;
}

// Implementação da heurística "Embaralhar e Preencher a Mochila"
std::vector<bool> heuristicaEmbaralhar(const std::vector<Item>& itens, int capacidade) {
    std::vector<bool> solucao(itens.size(), false);
    std::vector<size_t> indices(itens.size());
    std::iota(indices.begin(), indices.end(), 0);
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    int pesoAtual = 0;
    for (size_t i : indices) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            solucao[i] = true;
            pesoAtual += itens[i].peso;
        }
    }
    return solucao;
}

// Implementação da heurística "Seleção Aleatória Baseada em Probabilidade"
std::vector<bool> heuristicaProbabilistica(const std::vector<Item>& itens, int capacidade) {
    std::vector<bool> solucao(itens.size(), false);
    int pesoAtual = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        float probabilidade = static_cast<float>(rand()) / RAND_MAX;
        if (probabilidade > 0.5 && (pesoAtual + itens[i].peso <= capacidade)) {
            solucao[i] = true;
            pesoAtual += itens[i].peso;
        }
    }
    return solucao;
}

// Implementação da estratégia Mochila Cheia
std::vector<bool> mochilaCheia(const std::vector<Item>& itens, int capacidade) {
    std::vector<bool> mochila(itens.size(), false);
    int pesoAtual = 0;

    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoAtual + itens[i].peso <= capacidade) {
            mochila[i] = true;
            pesoAtual += itens[i].peso;
        }
    }
    return mochila;
}

// Implementação da estratégia Substituição de Objeto
std::vector<bool> substituicaoObjeto(std::vector<Item>& itens, int capacidade) {
    std::vector<bool> solucao = geraSolucaoAleatoria(itens, capacidade);
    solucao = mochilaCheia(itens, capacidade);

    bool melhoria = true;
    while (melhoria) {
        melhoria = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            if (solucao[i]) continue;

            for (size_t j = 0; j < itens.size(); ++j) {
                if (!solucao[j]) continue;

                std::vector<bool> novaSolucao = solucao;
                novaSolucao[j] = false;
                novaSolucao[i] = true;

                if (calculaPeso(itens, novaSolucao) <= capacidade &&
                    calculaValor(itens, novaSolucao) > calculaValor(itens, solucao)) {
                    solucao = novaSolucao;
                    melhoria = true;
                }
            }
        }
    }
    return solucao;
}

// Implementação da estratégia Hill Climbing
std::vector<bool> hillClimbing(const std::vector<Item>& itens, int capacidade) {
    std::vector<bool> solucao = geraSolucaoAleatoria(itens, capacidade);
    
    bool melhoria = true;
    while (melhoria) {
        melhoria = false;
        for (size_t i = 0; i < itens.size(); ++i) {
            std::vector<bool> vizinho = solucao;
            vizinho[i] = !vizinho[i]; 

            if (calculaPeso(itens, vizinho) <= capacidade &&
                calculaValor(itens, vizinho) > calculaValor(itens, solucao)) {
                solucao = vizinho;
                melhoria = true;
            }
        }
    }
    return solucao;
}

// Função para ler os itens do arquivo de entrada
std::vector<Item> lerEntrada(const std::string& nomeArquivo, int& capacidade) {
    std::ifstream arquivo(nomeArquivo);
    std::vector<Item> itens;
    if (arquivo.is_open()) {
        arquivo >> capacidade;
        int peso, valor;
        while (arquivo >> peso >> valor) {
            itens.push_back({peso, valor});
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo: " << nomeArquivo << std::endl;
    }
    return itens;
}

int main() {
    srand(time(0));

    // Lista de arquivos de entrada
    std::vector<std::string> arquivos = {"entrada1.txt", "entrada2.txt", "entrada3.txt"};
    
    for (const std::string& nomeArquivo : arquivos) {
        int capacidade;
        std::vector<Item> itens = lerEntrada(nomeArquivo, capacidade);

        std::cout << "Processando " << nomeArquivo << " com capacidade da mochila: " << capacidade << "\n";

        // Executar as heurísticas e estratégias
        std::vector<bool> melhorSolucao;
        int melhorValor = 0;

        // Heurística 1: Embaralhar e Preencher a Mochila
        std::vector<bool> solucaoEmbaralhar = heuristicaEmbaralhar(itens, capacidade);
        int valorEmbaralhar = calculaValor(itens, solucaoEmbaralhar);
        std::cout << "Valor Embaralhar: " << valorEmbaralhar << "\n";

        // Heurística 2: Seleção Aleatória Baseada em Probabilidade
        std::vector<bool> solucaoProbabilistica = heuristicaProbabilistica(itens, capacidade);
        int valorProbabilistico = calculaValor(itens, solucaoProbabilistica);
        std::cout << "Valor Probabilística: " << valorProbabilistico << "\n";

        // Algoritmo 3: Mochila Cheia
        std::vector<bool> solucaoCheia = mochilaCheia(itens, capacidade);
        int valorCheia = calculaValor(itens, solucaoCheia);
        std::cout << "Valor Mochila Cheia: " << valorCheia << "\n";

        // Algoritmo 4: Substituição de Objeto
        std::vector<bool> solucaoSubstituicao = substituicaoObjeto(itens, capacidade);
        int valorSubstituicao = calculaValor(itens, solucaoSubstituicao);
        std::cout << "Valor Substituição de Objeto: " << valorSubstituicao << "\n";

        // Algoritmo 5: Hill Climbing
        for (int i = 0; i < 10; ++i) {
            std::vector<bool> solucaoHill = hillClimbing(itens, capacidade);
            int valorHill = calculaValor(itens, solucaoHill);
            if (valorHill > melhorValor) {
                melhorValor = valorHill;
                melhorSolucao = solucaoHill;
            }
        }
        std::cout << "Melhor valor Hill Climbing: " << melhorValor << "\n";
    }

    return 0;
}
