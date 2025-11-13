#include "../include/corrida.hpp"
#include <cmath>

Corrida::Corrida(double velocidadeVeiculo) :
    distanciaTotal(0.0),
    duracaoTotal(0.0),
    eficiencia(0.0),
    velocidadeDoVeiculo(velocidadeVeiculo)
{
    // Inicializa os três vetores
    listaDemandas = new Vetor();
    listaParadas = new Vetor();
    listaTrechos = new Vetor();
}

Corrida::~Corrida() {
    // Libera o conteúdo de listaParadas
    for (int i = 0; i < listaParadas->getTamanho(); i++) {
        delete (Parada*) listaParadas->pegaElemento(i);
    }

    // Libera o conteúdo de listaTrechos
    for (int i = 0; i < listaTrechos->getTamanho(); i++) {
        delete (Trecho*) listaTrechos->pegaElemento(i);
    }

    delete listaDemandas;
    delete listaParadas;
    delete listaTrechos;
}

void Corrida::adicionarDemanda(Demanda* demanda) {
    listaDemandas->insereNoFim((void*) demanda);
    // Associa esta corrida à demanda
    demanda->setCorrida(this);
}

void Corrida::construirRota() {
    int numDemandas = listaDemandas->getTamanho();
    if (numDemandas == 0) return;

    // Cria listas temporárias de paradas de embarque e desembarque
    Vetor* paradasEmbarque = new Vetor(numDemandas);
    Vetor* paradasDesembarque = new Vetor(numDemandas);

    for (int i = 0; i < numDemandas; i++) {
        Demanda* d = (Demanda*) listaDemandas->pegaElemento(i);

        // Cria parada de embarque
        Parada* pEmbarque = new Parada(d->getOrigem(), TipoParada::EMBARQUE, d);
        paradasEmbarque->insereNoFim((void*) pEmbarque);
        listaParadas->insereNoFim((void*) pEmbarque);

        // Cria parada de desembarque
        Parada* pDesembarque = new Parada(d->getDestino(), TipoParada::DESEMBARQUE, d);
        paradasDesembarque->insereNoFim((void*) pDesembarque);
        listaParadas->insereNoFim((void*) pDesembarque);
    }

    // Cria os Trechos de COLETA
    for (int i = 0; i < numDemandas - 1; i++) {
        Parada* p1 = (Parada*) paradasEmbarque->pegaElemento(i);
        Parada* p2 = (Parada*) paradasEmbarque->pegaElemento(i+1);
        Trecho* t = new Trecho(p1, p2, velocidadeDoVeiculo);
        listaTrechos->insereNoFim((void*) t);
    }

    // Cria o Trecho de DESLOCAMENTO
    Parada* ultimaColeta = (Parada*) paradasEmbarque->pegaElemento(numDemandas - 1);
    Parada* primeiraEntrega = (Parada*) paradasDesembarque->pegaElemento(0);
    Trecho* tDeslocamento = new Trecho(ultimaColeta, primeiraEntrega, velocidadeDoVeiculo);
    listaTrechos->insereNoFim((void*) tDeslocamento);

    // Cria os Trechos de ENTREGA
    for (int i = 0; i < numDemandas - 1; i++) {
        Parada* p1 = (Parada*) paradasDesembarque->pegaElemento(i);
        Parada* p2 = (Parada*) paradasDesembarque->pegaElemento(i+1);
        Trecho* t = new Trecho(p1, p2, velocidadeDoVeiculo);
        listaTrechos->insereNoFim((void*) t);
    }

    // Limpa os vetores temporários (não seu conteúdo)
    delete paradasEmbarque;
    delete paradasDesembarque;
}

void Corrida::calcularEstatisticas() {
    // Soma distância e duração dos trechos
    distanciaTotal = 0.0;
    duracaoTotal = 0.0;
    for (int i = 0; i < listaTrechos->getTamanho(); i++) {
        Trecho* t = (Trecho*) listaTrechos->pegaElemento(i);
        distanciaTotal += t->getDistancia();
        duracaoTotal += t->getTempo();
    }

    // Soma as distâncias individuais (origem-destino)
    double somaDistanciasIndividuais = 0.0;
    for (int i = 0; i < listaDemandas->getTamanho(); i++) {
        Demanda* d = (Demanda*) listaDemandas->pegaElemento(i);
        somaDistanciasIndividuais += getDistanciaIndividual(d);
    }

    // Calcula a eficiência
    if (distanciaTotal > 0) {
        eficiencia = somaDistanciasIndividuais / distanciaTotal;
    } else {
        // Evita divisão por zero (caso de uma corrida sem distância)
        eficiencia = 1.0; 
    }
}

double Corrida::getDistanciaIndividual(Demanda* d) const {
    Ponto o = d->getOrigem();
    Ponto dest = d->getDestino();
    return std::sqrt(std::pow(dest.x - o.x, 2) + std::pow(dest.y - o.y, 2));
}

Vetor* Corrida::getListaDemandas() const {
    return listaDemandas;
}

Vetor* Corrida::getListaTrechos() const {
    return listaTrechos;
}

int Corrida::getNumPassageiros() const {
    return listaDemandas->getTamanho();
}

int Corrida::getNumParadas() const {
    return listaParadas->getTamanho();
}

double Corrida::getDistanciaTotal() const {
    return distanciaTotal;
}

double Corrida::getEficiencia() const {
    return eficiencia;
}