#include <iostream>
#include <vector>
#include <algorithm> // para std::shuffle
#include <random>    // para std::default_random_engine
#include <chrono>    // para medir o tempo de execução

struct Item {
    int peso;
    int valor;
};

int shuffleAndFillKnapsack(std::vector<Item>& items, int maxWeight) {
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(items.begin(), items.end(), rng);

    int totalWeight = 0;
    int totalValue = 0;

    for (const auto& item : items) {
        if (totalWeight + item.peso <= maxWeight) {
            totalWeight += item.peso;
            totalValue += item.valor;
        }
    }
    return totalValue;
}

int main() {
    std::vector<Item> items = {{1, 50}, {2, 100}, {3, 150}, {4, 200}}; // Exemplo de itens
    int maxWeight = 5;

    auto start = std::chrono::high_resolution_clock::now();
    int result = shuffleAndFillKnapsack(items, maxWeight);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Valor total: " << result << std::endl;
    std::cout << "Tempo de execução: " << elapsed.count() << " segundos" << std::endl;

    return 0;
}
