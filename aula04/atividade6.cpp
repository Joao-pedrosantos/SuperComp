#include <iostream>

using namespace std;

int main() {
    const int tamanho = 10;
    int numeros[tamanho];
    int maior = 0;

    cout << "Insira "<< tamanho << " números:" << endl;
    for (int i = 0; i < tamanho; i++) {
        cout << "Número " << i + 1 << ": ";
        cin >> numeros[i];
    }

    for (int i = 0; i < tamanho; i++){
        if(numeros[i] > maior){
            maior = numeros[i];
        }
    }

    cout << "Maior número da lista: " << maior<< endl;
    return 0;
}
