#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Necessário para std::remove_if

using namespace std;

// Estrutura para representar um item no estoque
struct Item {
    string nome;
    int quantidade;
};

void adicionarItem(vector<Item> &estoque) {
    Item novoItem;
    cout << "Digite o nome do item: ";
    cin >> novoItem.nome;
    cout << "Digite a quantidade: ";
    cin >> novoItem.quantidade;

    // Verifica se o item já existe no estoque
    for (auto &item : estoque) {
        if (item.nome == novoItem.nome) {
            item.quantidade += novoItem.quantidade;
            cout << "Quantidade adicionada ao item existente." << endl;
            return;
        }
    }

    // Se o item não existe, adiciona como novo
    estoque.push_back(novoItem);
    cout << "Item adicionado ao estoque." << endl;
}

// Função para remover um item do estoque
void removerItem(vector<Item> &estoque) {
    string nomeItem;
    int quantidadeRemover;
    cout << "Digite o nome do item a ser removido: ";
    cin >> nomeItem;
    cout << "Digite a quantidade a ser removida: ";
    cin >> quantidadeRemover;

    // Usar um lambda para encontrar e remover o item
    auto it = find_if(estoque.begin(), estoque.end(), [&](Item &item) {
        return item.nome == nomeItem;
    });

    if (it != estoque.end()) {
        if (it->quantidade >= quantidadeRemover) {
            it->quantidade -= quantidadeRemover;
            cout << "Quantidade removida do estoque." << endl;
            if (it->quantidade == 0) {
                estoque.erase(it);
                cout << "Item removido do estoque, pois a quantidade chegou a zero." << endl;
            }
        } else {
            cout << "Quantidade insuficiente no estoque." << endl;
        }
    } else {
        cout << "Item não encontrado no estoque." << endl;
    }
}

// Função para visualizar o estoque atual
void visualizarEstoque(const vector<Item> &estoque) {
    if (estoque.empty()) {
        cout << "O estoque está vazio." << endl;
    } else {
        cout << "Estoque atual:" << endl;
        for (const auto &item : estoque) {
            cout << "Item: " << item.nome << ", Quantidade: " << item.quantidade << endl;
        }
    }
}

int main() {
    vector<Item> estoque;
    int escolha;

    do {
        cout << "\nSistema de Controle de Estoque" << endl;
        cout << "1. Adicionar item ao estoque" << endl;
        cout << "2. Remover item do estoque" << endl;
        cout << "3. Visualizar estoque atual" << endl;
        cout << "4. Sair" << endl;
        cout << "Escolha uma opção: ";
        cin >> escolha;

        switch (escolha) {
            case 1:
                adicionarItem(estoque);
                break;
            case 2:
                removerItem(estoque);
                break;
            case 3:
                visualizarEstoque(estoque);
                break;
            case 4:
                cout << "Saindo do sistema." << endl;
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    } while (escolha != 4);

    return 0;
}
