#include <iostream>

using namespace std;

int main() {
    int num1;

    cout << "Insira o número: ";
    cin >> num1;
    
    if(num1 % 2 == 0){
        cout << "Par" << endl;
    }
    else{
        cout << "Impar" << endl;
    }

    return 0;
}
