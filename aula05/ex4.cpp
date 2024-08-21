#include <iostream>
#include <stdexcept>  // Inclui a biblioteca para exceções padrão.

class Vector {
public:
    // Construtor que inicializa o vetor com um tamanho inicial.
    Vector(int tamanho) : tam(tamanho), capacidade(tamanho) {
        dados = new int[capacidade];
    }

    // Destrutor para liberar a memória alocada dinamicamente.
    ~Vector() {
        delete[] dados;
    }

    // Função para inicializar todos os elementos do vetor com um valor específico.
    void inicializa(int valor) {
        for (int i = 0; i < tam; ++i) {
            dados[i] = valor;
        }
    }

    // Função para acessar o valor de um elemento do vetor.
    int get(int index) const {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Index fora do alcance");
        }
        return dados[index];
    }

    // Função para modificar o valor de um elemento do vetor.
    void set(int index, int valor) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Index fora do alcance");
        }
        dados[index] = valor;
    }

    // Função para inserir um novo valor em uma posição específica do vetor.
    void inserir(int index, int valor) {
        if (index < 0 || index > tam) {
            throw std::out_of_range("Index fora do alcance");
        }
        if (tam == capacidade) {
            redimensiona(2 * capacidade);
        }
        for (int i = tam; i > index; --i) {
            dados[i] = dados[i - 1];
        }
        dados[index] = valor;
        ++tam;
    }

    // Função para remover um elemento de uma posição específica do vetor.
    void remover(int index) {
        if (index < 0 || index >= tam) {
            throw std::out_of_range("Index fora do alcance");
        }
        for (int i = index; i < tam - 1; ++i) {
            dados[i] = dados[i + 1];
        }
        --tam;
    }

    // Função para imprimir todos os elementos do vetor.
    void imprime() const {
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int* dados;      // Ponteiro para o array dinâmico que armazena os dados.
    int tam;         // Número atual de elementos no vetor.
    int capacidade;  // Capacidade total do vetor (tamanho máximo antes de redimensionar).

    // Função para redimensionar o vetor quando necessário.
    void redimensiona(int novaCapacidade) {
        int* novoDados = new int[novaCapacidade];
        for (int i = 0; i < tam; ++i) {
            novoDados[i] = dados[i];
        }
        delete[] dados;
        dados = novoDados;
        capacidade = novaCapacidade;
    }
};

// Implementação das funções...

int main() {
    Vector vec(5);          // Cria um vetor de tamanho 5.
    vec.inicializa(0);      // Inicializa todos os elementos com 0.
    vec.imprime();          // Imprime os elementos do vetor.

    vec.set(2, 10);         // Define o valor na posição 2 como 10.
    vec.imprime();          // Imprime os elementos do vetor.

    vec.inserir(3, 20);     // Insere o valor 20 na posição 3.
    vec.imprime();          // Imprime os elementos do vetor.

    vec.remover(1);         // Remove o elemento na posição 1.
    vec.imprime();          // Imprime os elementos do vetor.

    return 0;
}
