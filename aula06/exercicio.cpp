#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct Result {
    int maxValue;
    int totalWeight;
};

Result knapsack(int W, vector<int>& weights, vector<int>& values, int n) {
    if (n == 0 || W == 0)
        return {0, 0};

    if (weights[n-1] > W)
        return knapsack(W, weights, values, n-1);
    else {
        Result include = knapsack(W - weights[n-1], weights, values, n-1);
        include.maxValue += values[n-1];
        include.totalWeight += weights[n-1];

        Result exclude = knapsack(W, weights, values, n-1);

        if (include.maxValue > exclude.maxValue) {
            return include;
        } else {
            return exclude;
        }
    }
}

void processFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Não foi possível abrir o arquivo " << filename << endl;
        return;
    }

    int N, W;
    infile >> N >> W;

    vector<int> weights(N);
    vector<int> values(N);

    for (int i = 0; i < N; i++) {
        infile >> weights[i] >> values[i];
    }

    auto start = high_resolution_clock::now();

    Result result = knapsack(W, weights, values, N);

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Arquivo: " << filename << endl;
    cout << "Valor máximo que pode ser carregado: " << result.maxValue << endl;
    cout << "Peso total dos itens escolhidos: " << result.totalWeight << " kg" << endl;
    cout << "Tempo de execução: " << duration.count() << " µs" << endl << endl;
}

int main() {
    vector<string> filenames = {"in1.txt", "in2.txt", "in3.txt", "in4.txt", "in5.txt"};

    for (const auto& filename : filenames) {
        processFile(filename);
    }

    return 0;
}
