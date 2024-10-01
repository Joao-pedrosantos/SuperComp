#include <iostream>
#include <vector>
#include <algorithm>

// Função heurística que tenta encontrar um subconjunto cuja soma seja igual a 'sum'
bool subsetSum(const std::vector<int>& set, int sum) {
    // Pré-processamento: Ordenar o conjunto em ordem decrescente
    std::vector<int> sorted_set = set;
    std::sort(sorted_set.begin(), sorted_set.end(), std::greater<int>());
    
    int current_sum = 0;
    
    // Heurística: Tentar aproximar-se da soma alvo (sum) com base nos elementos ordenados
    for (int num : sorted_set) {
        if (current_sum + num <= sum) {
            current_sum += num;  // Incluir o número no subconjunto
        }
        
        // Verificar se já atingimos a soma alvo
        if (current_sum == sum) {
            return true;  // Subconjunto encontrado
        }
    }
    
    // Caso nenhum subconjunto tenha atingido a soma alvo
    return false;
}

int main() {
    // Cenários de teste
    std::vector<int> set1 = {1, 50, 94, 40, 39, 62};
    int target_sum1 = 100;
    
    std::vector<int> set2 = {5, 10, 12, 13, 15, 18};
    int target_sum2 = 30;
    
    std::vector<int> set3 = {3, 34, 4, 12, 5, 2};
    int target_sum3 = 9;

    std::vector<int> set4 = {3, 34, 4, 12, 5, 2};
    int target_sum4 = 35;  // Exemplo em que não há solução
    
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
