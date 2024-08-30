#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

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

    Result result = knapsack(W, weights, values, N);

    cout << "Arquivo: " << filename << endl;
    cout << "Valor máximo que pode ser carregado: " << result.maxValue << endl;
    cout << "Peso total dos itens escolhidos: " << result.totalWeight << " kg" << endl << endl;
}

int main() {
    vector<string> filenames = {"in1.txt", "in2.txt", "in3.txt", "in4.txt"};

    for (const auto& filename : filenames) {
        processFile(filename);
    }

    return 0;
}
