#include <iostream>
#include <random>
#include <omp.h>
#include <chrono>

int main() {
    int N = 100000000;
    int pointsInsideCircle = 0;
    auto start = std::chrono::high_resolution_clock::now();

    // Paralelizando o for com OpenMP e aplicando redução na variável pointsInsideCircle
    #pragma omp parallel for reduction(+:pointsInsideCircle)
    for (int i = 0; i < N; ++i) {
        // Cada thread usa um gerador de números aleatórios independente
        std::mt19937 generator(omp_get_thread_num() + std::time(0)); 
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        double x = distribution(generator);
        double y = distribution(generator);

        // Verifica se o ponto está dentro do círculo unitário
        if (x * x + y * y <= 1.0) {
            pointsInsideCircle++;
        }
    }

    // Estimando o valor de pi
    double piEstimate = 4.0 * pointsInsideCircle / N;

    std::cout << "N: " << N << std::endl;
    // Exibindo o resultado
    std::cout << "Estimativa de Pi (paralela com gerador adequado): " << piEstimate << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;

    return 0;
}
