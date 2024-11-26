#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>

double media(const std::vector<double>& v) {
    double sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    return sum / v.size();
}

double maior_valor(const std::vector<double>& v) {
    double max = v[0];
    #pragma omp parallel for reduction(max:max)
    for (int i = 1; i < v.size(); i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

double produto(const std::vector<double>& v) {
    double prod = 1;
    #pragma omp parallel for reduction(*:prod)
    for (int i = 0; i < v.size(); i++) {
        prod *= v[i];
    }
    return prod;
}

double media_seq(const std::vector<double>& v) {
    double sum = 0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    return sum / v.size();
}

double maior_valor_seq(const std::vector<double>& v) {
    double max = v[0];
    for (int i = 1; i < v.size(); i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

double produto_seq(const std::vector<double>& v) {
    double prod = 1;
    for (int i = 0; i < v.size(); i++) {
        prod *= v[i];
    }
    return prod;
}

int main() {
    //std::vector<double> v = {10, 4, 1, 2, 5, 12, 7, 9, 3, 8};
    std::vector<double> v = {10, 4, 1, 2, 5, 12, 7, 9, 3, 11, 8, 14};
    // como é um vetor pequeno, paralelizar deixa o código mais lento já que existe um overhead
    // se colocar um vetor muito grande, a diferença começa a ficar mais nítida

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Media: " << media(v) << std::endl;
    std::cout << "Maior valor: " << maior_valor(v) << std::endl;
    std::cout << "Produto: " << produto(v) << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = end - start;
    std::cout << "Tempo paralelizado: " << duration.count() << " us" << std::endl;


    start = std::chrono::high_resolution_clock::now();
    std::cout << "Media: " << media_seq(v) << std::endl;
    std::cout << "Maior valor: " << maior_valor_seq(v) << std::endl;
    std::cout << "Produto: " << produto_seq(v) << std::endl;

    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Tempo sequencial: " << duration.count() << " us" << std::endl;
    return 0;
}