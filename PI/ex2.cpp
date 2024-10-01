#include <iostream>
#include <vector>
#include <chrono>

// Função recursiva que verifica se existe um subconjunto cuja soma seja igual a 'sum'
bool subsetSumRecursive(const std::vector<int>& set, int n, int sum) {
    // Caso base: se a soma é 0, encontramos o subconjunto
    if (sum == 0) return true;

    // Caso base: se não há mais elementos e a soma não foi atingida, não há solução
    if (n == 0 && sum != 0) return false;

    // Se o último elemento for maior que a soma, ignorá-lo
    if (set[n - 1] > sum) return subsetSumRecursive(set, n - 1, sum);

    // Verificar dois casos:
    // 1. Incluir o último elemento
    // 2. Não incluir o último elemento
    return subsetSumRecursive(set, n - 1, sum) ||
           subsetSumRecursive(set, n - 1, sum - set[n - 1]);
}

int main() {
    // Definindo 10 cenários de teste
    std::vector<std::vector<int>> test_cases = {
        {1, 2, 3, 4},                    // Pequeno
        {5, 7, 12, 1, 2},                // Pequeno
        {10, 20, 15, 30},                // Pequeno
        {20, 15, 10, 5, 30},             // Pequeno
        {2, 3, 7, 8, 10},                // Médio
        {12, 45, 78, 98, 123},           // Médio
        {15, 30, 45, 60, 90},            // Médio
        {100, 200, 150, 50, 75, 25},     // Médio
        {500, 1000, 750, 125, 250},      // Grande
        {1000, 2000, 1500, 500, 10000, 1200, 4500, 8000},  // Grande
    };

    // Alvo de soma para cada cenário
    std::vector<int> target_sums = {5, 8, 35, 25, 14, 246, 90, 450, 1875, 11200};

    // Testando as 10 entradas
    for (size_t i = 0; i < test_cases.size(); ++i) {
        auto& set = test_cases[i];
        int sum = target_sums[i];
        int n = set.size();

        // Medindo o tempo de execução
        auto start = std::chrono::high_resolution_clock::now();
        
        bool found = subsetSumRecursive(set, n, sum);
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Teste " << i + 1 << " - Soma " << sum 
                  << ": " << (found ? "Encontrado" : "Não encontrado") 
                  << " em " << duration.count() << " segundos" << std::endl;
    }

    return 0;
}
