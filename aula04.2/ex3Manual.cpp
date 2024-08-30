#include <iostream>
#include <chrono>     // Inclui a biblioteca para medição de tempo, usada para medir a performance do código.

int** alocarMatriz(int N) {
    int** matriz = new int*[N];
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }
    return matriz;
}

void liberarMatriz(int** matriz, int N) {
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

long long somarMatriz(int** matriz, int N) {
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

    // Medição do tempo para alocação dinâmica manual
    auto inicio = std::chrono::high_resolution_clock::now();

    int** matriz = alocarMatriz(N);
    long long soma = somarMatriz(matriz, N);

    liberarMatriz(matriz, N);

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Soma (alocação manual): " << soma << std::endl;
    std::cout << "Tempo (alocação manual): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
