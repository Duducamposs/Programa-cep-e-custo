#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main() {
    std::ifstream arquivo("entrada.txt");

    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    std::string linha;
    long long cepConsulta;

    while (std::getline(arquivo, linha)) {
        if (linha == "--") {
            break;
        }
    }

    arquivo >> cepConsulta;

    arquivo.clear();
    arquivo.seekg(0);

    bool encontrou = false;

    while (std::getline(arquivo, linha)) {
        if (linha == "--") {
            break;
        }

        std::stringstream ss(linha);

        std::string nome;
        std::string inicioStr;
        std::string fimStr;

        std::getline(ss, nome, ',');
        std::getline(ss, inicioStr, ',');
        std::getline(ss, fimStr, ',');

        long long inicio = std::stoll(inicioStr);
        long long fim = std::stoll(fimStr);

        if (cepConsulta >= inicio && cepConsulta <= fim) {
            std::cout << nome << std::endl;
            encontrou = true;
            break;
        }
    }

    if (!encontrou) {
        std::cout << "CEP nao encontrado" << std::endl;
    }

    arquivo.close();

    return 0;
}