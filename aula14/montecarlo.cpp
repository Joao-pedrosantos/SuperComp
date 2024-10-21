#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>

int main() {
    int N = 100000000;
    int pointsInsideCircle = 0;
    double x, y;
    auto start = std::chrono::high_resolution_clock::now();
    // Inicializando o gerador de números aleatórios
    std::srand(std::time(0));

    // Gerando pontos aleatórios
    for (int i = 0; i < N; ++i) {
        // Gera coordenadas (x, y) entre 0 e 1
        x = static_cast<double>(std::rand()) / RAND_MAX;
        y = static_cast<double>(std::rand()) / RAND_MAX;

        // Verifica se o ponto está dentro do círculo unitário
        if (x * x + y * y <= 1.0) {
            pointsInsideCircle++;
        }
    }

    // Estimando o valor de pi
    double piEstimate = 4.0 * pointsInsideCircle / N;

    // Exibindo o resultado
    std::cout << "N: " << N << std::endl;
    std::cout << "Estimativa de Pi: " << piEstimate << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;


    return 0;
}
