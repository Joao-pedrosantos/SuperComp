#include <iostream>

using namespace std;

int main() {
    double num1, num2;
    char operation;

    // Pedir ao usuário para inserir dois números
    cout << "Insira o primeiro número: ";
    cin >> num1;
    cout << "Insira o segundo número: ";
    cin >> num2;

    // Pedir ao usuário para escolher uma operação
    cout << "Escolha a operação (+, -, *, /): ";
    cin >> operation;

    // Executar a operação escolhida
    switch (operation) {
        case '+':
            cout << "Resultado: " << num1 + num2 << endl;
            break;
        case '-':
            cout << "Resultado: " << num1 - num2 << endl;
            break;
        case '*':
            cout << "Resultado: " << num1 * num2 << endl;
            break;
        case '/':
            if (num2 != 0) {
                cout << "Resultado: " << num1 / num2 << endl;
            } else {
                cout << "Erro: Divisão por zero!" << endl;
            }
            break;
        default:
            cout << "Operação inválida!" << endl;
            break;
    }

    return 0;
}
