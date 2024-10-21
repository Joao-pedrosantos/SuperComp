#include <iostream>
#include <random>
#include <omp.h>
#include <chrono>

int main() {
    int N = 100000000;
    int pointsInsideCircle = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // Paralelizando o sorteio de pontos e garantindo que cada thread tenha seu próprio gerador
    #pragma omp parallel reduction(+:pointsInsideCircle)
    {
        // Criando um gerador de números aleatórios único para cada thread, baseado no número da thread
        std::mt19937 generator(omp_get_thread_num() + std::time(0));
        std::uniform_real_distribution<double> distribution(0.0, 1.0);

        #pragma omp for
        for (int i = 0; i < N; ++i) {
            double x = distribution(generator);
            double y = distribution(generator);

            // Verifica se o ponto está dentro do círculo unitário
            if (x * x + y * y <= 1.0) {
                pointsInsideCircle++;
            }
        }
    }

    // Estimando o valor de pi
    double piEstimate = 4.0 * pointsInsideCircle / N;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Exibindo os resultados
    std::cout << "N: " << N << std::endl;;
    std::cout << "Estimativa de Pi (com gerador por thread): " << piEstimate << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
