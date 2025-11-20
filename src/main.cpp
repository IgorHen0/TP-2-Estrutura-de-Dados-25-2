#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <stdexcept>

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

        if (!demandaAtual) continue;

        // Se a demanda já foi combinada, pula para a próxima
        if (demandaAtual->getEstado() != EstadoDemanda::DEMANDADA) {
            continue;
        }

        // Cria uma nova corrida para esta demanda
        Corrida* novaCorrida = new Corrida(params.gama);
        novaCorrida->adicionarDemanda(demandaAtual);
        demandaAtual->setEstado(EstadoDemanda::INDIVIDUAL);
        
        // Tenta adicionar mais passageiros
        for (int j = i + 1; j < listaDemandas->getTamanho(); j++) {
            Demanda* demandaCandidata = (Demanda*) listaDemandas->pegaElemento(j);
            
            if (!demandaCandidata) continue;

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
        
        if (!eventoAtual) continue;

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
            
            if (proximoTrecho) {
                double proximoTempoChegada = tempoAtual + proximoTrecho->getTempo();

                Evento* proximoEvento = new Evento();
                proximoEvento->corrida = corrida;
                proximoEvento->indiceTrecho = proximoIndice;
                proximoEvento->tempoChegada = proximoTempoChegada;

                escalonador->insereEvento(proximoTempoChegada, (void*) proximoEvento);
            }
        }

        // Libera o evento atual da memória
        delete eventoAtual;
    }
}

void limpaMemoria(Vetor* listaDemandas, Vetor* listaCorridas) {
    if (listaDemandas) {
        for (int i = 0; i < listaDemandas->getTamanho(); i++) {
            Demanda* d = (Demanda*) listaDemandas->pegaElemento(i);
            if (d) delete d;
        }
        delete listaDemandas;
    }

    if (listaCorridas) {
        for (int i = 0; i < listaCorridas->getTamanho(); i++) {
            Corrida* c = (Corrida*) listaCorridas->pegaElemento(i);
            if (c) delete c;
        }
        delete listaCorridas;
    }
}

int main() {
    // Inicialização dos TADs
    Parametros params;
    Vetor* listaDemandas = nullptr;
    Vetor* listaCorridas = nullptr;
    Escalonador* escalonador = nullptr;

    try {
        listaDemandas = new Vetor();
        listaCorridas = new Vetor();
        escalonador = new Escalonador();

        // Leitura dos Parâmetros e Validação de Entrada
        if (!(std::cin >> params.eta >> params.gama >> params.delta 
                      >> params.alfa >> params.beta >> params.lambda)) {
            throw std::runtime_error("Falha na leitura dos parâmetros de entrada.");
        }

        // Leitura das Demandas
        int numDemandas;
        if (!(std::cin >> numDemandas)) {
            throw std::runtime_error("Falha na leitura do número de demandas.");
        }

        for (int i = 0; i < numDemandas; i++) {
            int id;
            double tempo;
            Ponto origem, destino;
            
            if (!(std::cin >> id >> tempo >> origem.x >> origem.y >> destino.x >> destino.y)) {
                 throw std::runtime_error("Erro de formatação ou fim de arquivo inesperado ao ler demandas.");
            }
            
            Demanda* novaDemanda = new Demanda(id, tempo, origem, destino);
            listaDemandas->insereNoFim((void*) novaDemanda);
        }

        // Execução das Fases
        agrupaCorridas(listaDemandas, listaCorridas, escalonador, params);
        executaSimulacao(escalonador);

    } catch (const std::exception& e) {
        // Tratamento de erro robusto: imprime no stderr e limpa o que for possível
        std::cerr << "Erro durante a execução: " << e.what() << std::endl;
        
        // Limpeza de Memória em caso de erro
        limpaMemoria(listaDemandas, listaCorridas);
        if (escalonador) delete escalonador;
        
        return EXIT_FAILURE;
    }

    // Limpeza de Memória
    limpaMemoria(listaDemandas, listaCorridas);
    if (escalonador) delete escalonador;

    return 0;
}