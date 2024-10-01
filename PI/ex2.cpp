#include <iostream>
#include <vector>
#include <chrono>

bool subsetSum(const std::vector<int>& set, int sum) {
    int n = set.size();
    bool dp[n + 1][sum + 1];

    // Inicialização da tabela DP
    for (int i = 0; i <= n; i++) {
        dp[i][0] = true; // Subconjunto vazio tem soma 0
    }
    for (int s = 1; s <= sum; s++) {
        dp[0][s] = false; // Sem elementos, não há como formar uma soma diferente de 0
    }

    // Preenchendo a tabela DP
    for (int i = 1; i <= n; i++) {
        for (int s = 1; s <= sum; s++) {
            if (set[i - 1] > s) {
                dp[i][s] = dp[i - 1][s]; // Não incluir o elemento
            } else {
                dp[i][s] = dp[i - 1][s] || dp[i - 1][s - set[i - 1]]; // Incluir ou não incluir o elemento
            }
        }
    }

    return dp[n][sum];
}

int main() {
    // Definindo 10 cenários de teste
    std::vector<std::vector<int>> test_cases = {
        {1, 2, 3, 4},                    // Pequeno
        {5, 7, 12, 1},                // Pequeno
        {10, 20, 15},                // Pequeno
        {20, 15, 10, 5},             // Pequeno
        {2, 3, 7, 8, 10},                // Médio
        {12, 45, 78, 98, 123, 54},           // Médio
        {15, 30, 45, 60, 90, 65, 4, 20},            // Médio
        {100, 200, 150, 50, 75, 25},     // Médio
        {500, 1000, 750, 125, 250, 120, 800, 500, 300, 625, 950},      // Grande
        {1000, 2000, 1500, 500, 10000, 1200, 4500, 8000, 1250, 1450, 1650},  // Grande
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
        
        bool found = subsetSum(set, sum);
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Teste " << i + 1 << " - Soma " << sum 
                  << ": " << (found ? "Encontrado" : "Não encontrado") 
                  << " em " << duration.count() << " segundos" << std::endl;
    }

    return 0;
}

