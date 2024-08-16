#include <iostream>
#include <string>

using namespace std;

int main() {
    string inputString;

    // Pedir ao usuário para inserir uma string
    cout << "Insira uma string: ";
    getline(cin, inputString);

    // Contar o número de caracteres na string
    int numCharacters = inputString.length();

    // Exibir o número de caracteres
    cout << "A string "<< inputString << " possui " << numCharacters << " caracteres." << endl;

    return 0;
}
