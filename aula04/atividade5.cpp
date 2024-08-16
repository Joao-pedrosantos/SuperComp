#include <iostream>

using namespace std;

int main() {
    const int tamanho = 5;
    int numeros[tamanho];
    int soma = 0;

    cout << "Insira 5 números:" << endl;
    for (int i = 0; i < tamanho; i++) {
        cout << "Número " << i + 1 << ": ";
        cin >> numeros[i];
        soma += numeros[i];
    }

    cout << "A soma dos números é: " << soma << endl;

    return 0;
}
