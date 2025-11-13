#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "../include/vetor.hpp"


struct Ponto {
    double x;
    double y;
};

struct Demanda {
    int id;
    double tempo;
    Ponto origem;
    Ponto destino;
};

struct Parametros {
    int eta;
    double gama;
    double delta;
    double alfa;
    double beta;
    double lambda;
};

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada foi fornecido." << std::endl;
        std::cerr << "Uso: ./tp2.out <caminho_para_o_arquivo_de_entrada>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];

    std::ifstream arquivoEntrada(nomeArquivo.c_str());
    
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << nomeArquivo << std::endl;
        return 1;
    }

    Parametros params;
    arquivoEntrada >> params.eta;
    arquivoEntrada >> params.gama;
    arquivoEntrada >> params.delta;
    arquivoEntrada >> params.alfa;
    arquivoEntrada >> params.beta;
    arquivoEntrada >> params.lambda;

    std::cout << "--- Parâmetros da Simulação Carregados ---" << std::endl;
    std::cout << "Capacidade (eta): " << params.eta << std::endl;
    std::cout << "Velocidade (gama): " << params.gama << std::endl;
    std::cout << "Intervalo (delta): " << params.delta << std::endl;

    int numDemandas;
    arquivoEntrada >> numDemandas;

    std::cout << "\nLendo " << numDemandas << " demandas..." << std::endl;

    // Inicializa o vetor com a capacidade exata
    Vetor listaDeDemandas(numDemandas); 

    for (int i = 0; i < numDemandas; i++) {
        // Aloca uma nova struct Demanda dinamicamente (com 'new')
        Demanda* novaDemanda = new Demanda();

        // Lê os dados da linha diretamente para a struct
        arquivoEntrada >> novaDemanda->id
                       >> novaDemanda->tempo
                       >> novaDemanda->origem.x
                       >> novaDemanda->origem.y
                       >> novaDemanda->destino.x
                       >> novaDemanda->destino.y;

        // Insere o ponteiro (void*) no vetor
        listaDeDemandas.insereNoFim((void*) novaDemanda);
    }

    arquivoEntrada.close();

    std::cout << "Leitura concluída. Total de demandas no vetor: " 
              << listaDeDemandas.getTamanho() << std::endl;

    std::cout << "\n--- Verificando dados lidos (primeiros 5) ---" << std::endl;
    int maxParaImprimir = (listaDeDemandas.getTamanho() < 5) ? listaDeDemandas.getTamanho() : 5;
    
    for (int i = 0; i < maxParaImprimir; i++) {
        // Pega o ponteiro genérico e faz o 'cast' de volta
        Demanda* d = (Demanda*) listaDeDemandas.pegaElemento(i);
        
        std::cout << "  Item " << i << " -> ID: " << d->id 
                  << ", Tempo: " << d->tempo
                  << ", Origem: (" << d->origem.x << ", " << d->origem.y << ")"
                  << std::endl;
    }

    std::cout << "\nLiberando memoria das " << listaDeDemandas.getTamanho() << " demandas..." << std::endl;
    
    for (int i = 0; i < listaDeDemandas.getTamanho(); i++) {
        Demanda* d = (Demanda*) listaDeDemandas.pegaElemento(i);
        delete d;
    }
    
    std::cout << "Concluído." << std::endl;
    return 0;
}