#include <iostream>

using namespace std;

int main() {
    const int tamanho = 3;
    int matriz[tamanho][tamanho];
    int somaDiagonal = 0;

    cout << "Insira os elementos da matriz 3x3:" << endl;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            cout << "Elemento [" << i + 1 << "][" << j + 1 << "]: ";
            cin >> matriz[i][j];
        }
    }

    for (int i = 0; i < tamanho; i++) {
        somaDiagonal += matriz[i][i];
    }

    cout << "A soma dos elementos na diagonal principal é: " << somaDiagonal << endl;

    return 0;
}
