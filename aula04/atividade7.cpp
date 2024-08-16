#include <iostream>

using namespace std;

void bubbleSort(int vetor[], int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            if (vetor[j] > vetor[j + 1]) {
                // Trocar os elementos de posição
                int temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

int main() {
    const int tamanho = 5;
    int numeros[tamanho];

    // Pedir ao usuário para inserir 5 números
    cout << "Insira 5 números:" << endl;
    for (int i = 0; i < tamanho; i++) {
        cout << "Número " << i + 1 << ": ";
        cin >> numeros[i];
    }

    // Ordenar o vetor usando Bubble Sort
    bubbleSort(numeros, tamanho);

    // Exibir o vetor ordenado
    cout << "Vetor ordenado em ordem crescente: ";
    for (int i = 0; i < tamanho; i++) {
        cout << numeros[i] << " ";
    }
    cout << endl;

    return 0;
}
