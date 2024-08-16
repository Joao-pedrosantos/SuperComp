#include <iostream>
#include <string>

using namespace std;

int main() {
    string palavra, palavraInvertida;

    cout << "Insira uma palavra: ";
    cin >> palavra;

    palavraInvertida = string(palavra.rbegin(), palavra.rend());

    if (palavra == palavraInvertida) {
        cout << "A palavra '" << palavra << "' é um palíndromo." << endl;
    } else {
        cout << "A palavra '" << palavra << "' não é um palíndromo." << endl;
    }

    return 0;
}
