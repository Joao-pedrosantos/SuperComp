#include <iostream>
#include <vector>
#include <algorithm>


struct Item {
    int valor;
    int peso;
};

bool comparaPorPeso(Item a, Item b) {
    return a.peso < b.peso;
}

int mochilaMaisLeve(std::vector<Item>& itens, int capacidade) {
    std::sort(itens.begin(), itens.end(), comparaPorPeso);
    int valorTotal = 0;
    for (auto& item : itens) {
        if (capacidade >= item.peso) {
            capacidade -= item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

bool comparaPorValor(Item a, Item b) {
    return a.valor > b.valor;
}

int mochilaMaisCaro(std::vector<Item>& itens, int capacidade) {
    std::sort(itens.begin(), itens.end(), comparaPorValor);
    int valorTotal = 0;
    for (auto& item : itens) {
        if (capacidade >= item.peso) {
            capacidade -= item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}


int main(){
    std::vector<Item> itens = {{100, 10}, {90, 9}, {80, 8}, {30, 2}, {20, 1}, {15, 1}, {10, 1}};
    int capacidade = 10;
    std::cout << "Mochila mais leve: " << mochilaMaisLeve(itens, capacidade) << std::endl;
    std::cout << "Mochila mais cara: " << mochilaMaisCaro(itens, capacidade) << std::endl;
    return 0;
}