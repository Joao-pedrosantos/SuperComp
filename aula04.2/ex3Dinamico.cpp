#include <iostream>
#include <chrono>     // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.
#include <vector>     // Inclui a biblioteca para uso do std::vector.

long long somarMatrizVector(std::vector<std::vector<int>>& matriz, int N) {
    long long soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;  // Exemplo de inicialização
            soma += matriz[i][j];
        }
    }
    return soma;
}

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N

    // Medição do tempo para alocação com std::vector
    auto inicio = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<int>> matriz(N, std::vector<int>(N));
    long long soma = somarMatrizVector(matriz, N);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Soma (std::vector): " << soma << std::endl;
    std::cout << "Tempo (std::vector): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
