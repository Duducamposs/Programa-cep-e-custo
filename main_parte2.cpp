#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>

struct FaixaCEP {
    char cidade;
    long long inicio;
    long long fim;
};

struct Estado {
    double distancia;
    char cidade;

    bool operator>(const Estado& outro) const {
        return distancia > outro.distancia;
    }
};

char encontrarCidade(
    long long cep,
    const std::vector<FaixaCEP>& faixas)
{
    for (const FaixaCEP& faixa : faixas) {
        if (cep >= faixa.inicio && cep <= faixa.fim) {
            return faixa.cidade;
        }
    }

    return '\0';
}

int main() {
    std::ifstream arquivo("entrada_parte2.txt");

    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    std::vector<FaixaCEP> faixas;
    std::string linha;

    while (std::getline(arquivo, linha)) {

        if (linha == "--") {
            break;
        }

        std::stringstream ss(linha);

        std::string cidadeStr;
        std::string inicioStr;
        std::string fimStr;

        std::getline(ss, cidadeStr, ',');
        std::getline(ss, inicioStr, ',');
        std::getline(ss, fimStr, ',');

        FaixaCEP faixa;
        faixa.cidade = cidadeStr[0];
        faixa.inicio = std::stoll(inicioStr);
        faixa.fim = std::stoll(fimStr);

        faixas.push_back(faixa);
    }

    std::map<char, std::vector<std::pair<char, double>>> grafo;

    while (std::getline(arquivo, linha)) {

        if (linha == "--") {
            break;
        }

        std::stringstream ss(linha);

        std::string origemStr;
        std::string destinoStr;
        std::string custoStr;

        std::getline(ss, origemStr, ',');
        std::getline(ss, destinoStr, ',');
        std::getline(ss, custoStr, ',');

        char origem = origemStr[0];
        char destino = destinoStr[0];
        double custo = std::stod(custoStr);

        // Bidirecional
        grafo[origem].push_back(
            std::make_pair(destino, custo)
        );

        grafo[destino].push_back(
            std::make_pair(origem, custo)
        );
    }

    std::string linhaFinal;

    std::getline(arquivo, linhaFinal);

    if (linhaFinal.empty()) {
        std::getline(arquivo, linhaFinal);
    }

    std::stringstream ssFinal(linhaFinal);

    std::string cepOrigemStr;
    std::string cepDestinoStr;

    std::getline(ssFinal, cepOrigemStr, ',');
    std::getline(ssFinal, cepDestinoStr, ',');

    long long cepOrigem = std::stoll(cepOrigemStr);
    long long cepDestino = std::stoll(cepDestinoStr);

    char cidadeOrigem =
        encontrarCidade(cepOrigem, faixas);

    char cidadeDestino =
        encontrarCidade(cepDestino, faixas);

    if (cidadeOrigem == '\0') {
        std::cout << "CEP de origem nao encontrado."
                  << std::endl;
        return 1;
    }

    if (cidadeDestino == '\0') {
        std::cout << "CEP de destino nao encontrado."
                  << std::endl;
        return 1;
    }

    std::map<char, double> distancia;
    std::map<char, char> anterior;

    for (char c = 'A'; c <= 'Z'; c++) {
        distancia[c] =
            std::numeric_limits<double>::infinity();
    }

    distancia[cidadeOrigem] = 0.0;

    std::priority_queue<
        Estado,
        std::vector<Estado>,
        std::greater<Estado>
    > fila;

    fila.push({0.0, cidadeOrigem});

    while (!fila.empty()) {

        Estado atual = fila.top();
        fila.pop();

        if (atual.distancia >
            distancia[atual.cidade]) {
            continue;
        }

        for (const auto& aresta :
             grafo[atual.cidade]) {

            char vizinho = aresta.first;
            double custo = aresta.second;

            double novaDistancia =
                distancia[atual.cidade] + custo;

            if (novaDistancia <
                distancia[vizinho]) {

                distancia[vizinho] =
                    novaDistancia;

                anterior[vizinho] =
                    atual.cidade;

                fila.push(
                    {novaDistancia, vizinho}
                );
            }
        }
    }

    if (distancia[cidadeDestino] ==
        std::numeric_limits<double>::infinity()) {

        std::cout << "Nao existe rota."
                  << std::endl;

        return 0;
    }

    std::vector<char> caminho;

    char atual = cidadeDestino;

    while (atual != cidadeOrigem) {
        caminho.push_back(atual);
        atual = anterior[atual];
    }

    caminho.push_back(cidadeOrigem);

    std::reverse(
        caminho.begin(),
        caminho.end()
    );

    std::cout << "Cidade origem: "
              << cidadeOrigem
              << std::endl;

    std::cout << "Cidade destino: "
              << cidadeDestino
              << std::endl;

    std::cout << "Rota: ";

    for (std::size_t i = 0;
         i < caminho.size();
         i++) {

        std::cout << caminho[i];

        if (i + 1 < caminho.size()) {
            std::cout << " -> ";
        }
    }

    std::cout << std::endl;

    std::cout << "Custo total: R$ "
             << std::fixed
             << std::setprecision(2)
             << distancia[cidadeDestino]
             << std::endl;
    

    return 0;
}
