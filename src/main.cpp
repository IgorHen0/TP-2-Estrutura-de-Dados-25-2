#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>

#include "../include/vetor.hpp"
#include "../include/escalonador.hpp"
#include "../include/demanda.hpp"
#include "../include/parada.hpp"
#include "../include/trecho.hpp"
#include "../include/corrida.hpp"
struct Parametros {
    int eta;
    double gama;
    double delta;
    double alfa;
    double beta;
    double lambda;
};
struct Evento {
    Corrida* corrida;
    int indiceTrecho;
    double tempoChegada;
};

double distancia(Ponto p1, Ponto p2) {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

void gerarEstatisticas(Corrida* corrida, double tempoConclusao) {
    // Configura a formatação de saída para 2 casas decimais
    std::cout << std::fixed << std::setprecision(2);
    
    std::cout << tempoConclusao;

    std::cout << " " << corrida->getDistanciaTotal();

    Vetor* trechos = corrida->getListaTrechos();
    int numParadas = trechos->getTamanho() + 1;
    std::cout << " " << numParadas;

    for (int i = 0; i < trechos->getTamanho(); i++) {
        Trecho* t = (Trecho*) trechos->pegaElemento(i);
        Ponto p = t->getParadaOrigem()->getCoordenadas();
        std::cout << " " << p.x << " " << p.y;
    }
    
    Trecho* ultimoTrecho = (Trecho*) trechos->pegaElemento(trechos->getTamanho() - 1);
    Ponto pFinal = ultimoTrecho->getParadaDestino()->getCoordenadas();
    std::cout << " " << pFinal.x << " " << pFinal.y;

    std::cout << std::endl;
}

void agrupaCorridas(Vetor* listaDemandas, Vetor* listaCorridas, Escalonador* escalonador, const Parametros& params) {
    
    for (int i = 0; i < listaDemandas->getTamanho(); i++) {
        Demanda* demandaAtual = (Demanda*) listaDemandas->pegaElemento(i);

        // Se a demanda já foi combinada, pula para a próxima
        if (demandaAtual->getEstado() != EstadoDemanda::DEMANDADA) {
            continue;
        }

        // Cria uma nova corrida para esta demanda
        Corrida* novaCorrida = new Corrida(params.gama);
        novaCorrida->adicionarDemanda(demandaAtual);
        demandaAtual->setEstado(EstadoDemanda::INDIVIDUAL);
        
        // Tenta adicionar mais passageiros (loop de combinação)
        for (int j = i + 1; j < listaDemandas->getTamanho(); j++) {
            Demanda* demandaCandidata = (Demanda*) listaDemandas->pegaElemento(j);

            // Critério de Capacidade (eta)
            if (novaCorrida->getNumPassageiros() >= params.eta) {
                break;
            }

            // Critério de Intervalo (delta)
            if (demandaCandidata->getTempo() - demandaAtual->getTempo() >= params.delta) {
                break;
            }
            
            // Critério de Estado (só pode agrupar demandas não processadas)
            if (demandaCandidata->getEstado() != EstadoDemanda::DEMANDADA) {
                continue;
            }

            // Critérios de Distância (alfa e beta)
            bool criteriosOk = true;
            Vetor* demandasJaNaCorrida = novaCorrida->getListaDemandas();
            for (int k = 0; k < demandasJaNaCorrida->getTamanho(); k++) {
                Demanda* d = (Demanda*) demandasJaNaCorrida->pegaElemento(k);
                
                // Critério Alfa (origens)
                if (distancia(demandaCandidata->getOrigem(), d->getOrigem()) > params.alfa) {
                    criteriosOk = false;
                    break;
                }
                // Critério Beta (destinos)
                if (distancia(demandaCandidata->getDestino(), d->getDestino()) > params.beta) {
                    criteriosOk = false;
                    break;
                }
            }

            if (!criteriosOk) {
                continue;
            }

            // Critério de Eficiência (lambda)
            Corrida* corridaTeste = new Corrida(params.gama);
            for (int k = 0; k < demandasJaNaCorrida->getTamanho(); k++) {
                corridaTeste->adicionarDemanda((Demanda*) demandasJaNaCorrida->pegaElemento(k));
            }
            corridaTeste->adicionarDemanda(demandaCandidata);
            
            corridaTeste->construirRota();
            corridaTeste->calcularEstatisticas();

            if (corridaTeste->getEficiencia() > params.lambda) {
                novaCorrida->adicionarDemanda(demandaCandidata);
                demandaCandidata->setEstado(EstadoDemanda::COMBINADA);
            } else {
                delete corridaTeste;
                break;
            }
            delete corridaTeste;
        }

        // Finaliza a 'novaCorrida' (seja individual ou combinada)
        novaCorrida->construirRota();
        novaCorrida->calcularEstatisticas();
        listaCorridas->insereNoFim((void*) novaCorrida);
        
        // Agenda o primeiro evento da corrida no escalonador
        Vetor* trechos = novaCorrida->getListaTrechos();
        if (trechos->getTamanho() > 0) {
            Trecho* primeiroTrecho = (Trecho*) trechos->pegaElemento(0);
            double tempoPartida = demandaAtual->getTempo();
            double tempoChegada = tempoPartida + primeiroTrecho->getTempo();
            
            Evento* primeiroEvento = new Evento();
            primeiroEvento->corrida = novaCorrida;
            primeiroEvento->indiceTrecho = 0;
            primeiroEvento->tempoChegada = tempoChegada;

            escalonador->insereEvento(tempoChegada, (void*) primeiroEvento);
        }
    }
}

void executaSimulacao(Escalonador* escalonador) {
    
    while (!escalonador->estaVazio()) {
        // Retira o próximo evento (o de menor tempo)
        Evento* eventoAtual = (Evento*) escalonador->retiraProximoEvento();
        
        double tempoAtual = eventoAtual->tempoChegada;
        Corrida* corrida = eventoAtual->corrida;
        int indiceTrechoAtual = eventoAtual->indiceTrecho;
        Vetor* trechosDaCorrida = corrida->getListaTrechos();

        // Verifica se é o último evento da corrida
        if (indiceTrechoAtual == trechosDaCorrida->getTamanho() - 1) {
            gerarEstatisticas(corrida, tempoAtual);
        } else {
            int proximoIndice = indiceTrechoAtual + 1;
            Trecho* proximoTrecho = (Trecho*) trechosDaCorrida->pegaElemento(proximoIndice);
            
            double proximoTempoChegada = tempoAtual + proximoTrecho->getTempo();

            Evento* proximoEvento = new Evento();
            proximoEvento->corrida = corrida;
            proximoEvento->indiceTrecho = proximoIndice;
            proximoEvento->tempoChegada = proximoTempoChegada;

            escalonador->insereEvento(proximoTempoChegada, (void*) proximoEvento);
        }

        // Libera o evento atual da memória
        delete eventoAtual;
    }
}

void limpaMemoria(Vetor* listaDemandas, Vetor* listaCorridas) {
    // Libera cada objeto Demanda
    for (int i = 0; i < listaDemandas->getTamanho(); i++) {
        delete (Demanda*) listaDemandas->pegaElemento(i);
    }
    // Libera o vetor de demandas
    delete listaDemandas;

    // Libera cada objeto Corrida
    for (int i = 0; i < listaCorridas->getTamanho(); i++) {
        delete (Corrida*) listaCorridas->pegaElemento(i);
    }
    // Libera o vetor de corridas
    delete listaCorridas;
}


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Erro: Nenhum arquivo de entrada foi fornecido." << std::endl;
        std::cerr << "Uso: ./tp2.out <caminho_para_o_arquivo_de_entrada>" << std::endl;
        return 1;
    }

    // Abertura do Arquivo
    std::string nomeArquivo = argv[1];
    std::ifstream arquivoEntrada(nomeArquivo.c_str());
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo: " << nomeArquivo << std::endl;
        return 1;
    }

    // Inicialização dos TADs
    Parametros params;
    Vetor* listaDemandas = new Vetor();
    Vetor* listaCorridas = new Vetor();
    Escalonador* escalonador = new Escalonador();

    // Leitura dos Parâmetros
    arquivoEntrada >> params.eta >> params.gama >> params.delta 
                   >> params.alfa >> params.beta >> params.lambda;

    // Leitura das Demandas
    int numDemandas;
    arquivoEntrada >> numDemandas;
    for (int i = 0; i < numDemandas; i++) {
        int id;
        double tempo;
        Ponto origem, destino;
        arquivoEntrada >> id >> tempo
                       >> origem.x >> origem.y
                       >> destino.x >> destino.y;
        
        Demanda* novaDemanda = new Demanda(id, tempo, origem, destino);
        listaDemandas->insereNoFim((void*) novaDemanda);
    }
    arquivoEntrada.close();

    // Execução das Fases
    agrupaCorridas(listaDemandas, listaCorridas, escalonador, params);
    executaSimulacao(escalonador);

    // Limpeza de Memória
    limpaMemoria(listaDemandas, listaCorridas);
    delete escalonador;

    return 0;
}