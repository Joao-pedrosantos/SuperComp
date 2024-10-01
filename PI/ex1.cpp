#include <iostream>
#include <vector>

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
    // Cenários de teste
    std::vector<int> set1 = {6, 50, 94, 40, 10, 62};
    int target_sum1 = 100;

    std::vector<int> set2 = {5, 10, 12, 13, 15, 18};
    int target_sum2 = 30;

    std::vector<int> set3 = {3, 34, 1, 12, 5, 2};
    int target_sum3 = 9;

    std::vector<int> set4 = {3, 34, 4, 12, 5, 2}; //não existe subconjunto que some 35
    int target_sum4 = 35; 

    // Testando os cenários
    std::cout << "Teste 1 - Subconjunto para soma 100: "
              << (subsetSum(set1, target_sum1) ? "Encontrado" : "Não encontrado") << std::endl;

    std::cout << "Teste 2 - Subconjunto para soma 30: "
              << (subsetSum(set2, target_sum2) ? "Encontrado" : "Não encontrado") << std::endl;

    std::cout << "Teste 3 - Subconjunto para soma 9: "
              << (subsetSum(set3, target_sum3) ? "Encontrado" : "Não encontrado") << std::endl;

    std::cout << "Teste 4 - Subconjunto para soma 35: "
              << (subsetSum(set4, target_sum4) ? "Encontrado" : "Não encontrado") << std::endl;

    return 0;
}