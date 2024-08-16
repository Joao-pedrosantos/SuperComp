#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    string nomeArquivoEntrada, nomeArquivoSaida;
    cout << "Digite o nome do arquivo de entrada: ";
    cin >> nomeArquivoEntrada;
    cout << "Digite o nome do arquivo de saída: ";
    cin >> nomeArquivoSaida;

    ifstream arquivoEntrada(nomeArquivoEntrada);
    ofstream arquivoSaida(nomeArquivoSaida);

    if (!arquivoEntrada) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    if (!arquivoSaida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return 1;
    }

    string linha;
    int numeroTotalPalavras = 0;
    int numeroTotalLinhas = 0;
    map<string, int> frequenciaPalavras;

    while (getline(arquivoEntrada, linha)) {
        numeroTotalLinhas++;
        stringstream ss(linha);
        string palavra;
        int palavrasNaLinha = 0;

        while (ss >> palavra) {
            numeroTotalPalavras++;
            palavrasNaLinha++;
            // Incrementa a contagem da palavra no mapa
            frequenciaPalavras[palavra]++;
        }
    }

    // Calcula o número médio de palavras por linha
    double mediaPalavrasPorLinha = static_cast<double>(numeroTotalPalavras) / numeroTotalLinhas;

    // Encontrar a palavra mais frequente
    string palavraMaisFrequente;
    int maxFrequencia = 0;
    for (const auto &par : frequenciaPalavras) {
        if (par.second > maxFrequencia) {
            maxFrequencia = par.second;
            palavraMaisFrequente = par.first;
        }
    }

    // Escrever as estatísticas no arquivo de saída
    arquivoSaida << "Estatísticas do texto:" << endl;
    arquivoSaida << "Número total de palavras: " << numeroTotalPalavras << endl;
    arquivoSaida << "Número total de linhas: " << numeroTotalLinhas << endl;
    arquivoSaida << "Número médio de palavras por linha: " << mediaPalavrasPorLinha << endl;
    arquivoSaida << "Palavra mais frequente: " << palavraMaisFrequente << " (ocorrências: " << maxFrequencia << ")" << endl;

    cout << "Estatísticas calculadas e gravadas no arquivo " << nomeArquivoSaida << "." << endl;

    arquivoEntrada.close();
    arquivoSaida.close();

    return 0;
}
