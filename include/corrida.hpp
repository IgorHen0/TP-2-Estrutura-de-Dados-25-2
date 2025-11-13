#ifndef CORRIDA_HPP
#define CORRIDA_HPP

#include "../include/vetor.hpp"
#include "../include/demanda.hpp"
#include "../include/parada.hpp"
#include "../include/trecho.hpp"

class Corrida {
private:
    Vetor* listaDemandas;
    Vetor* listaParadas;
    Vetor* listaTrechos;

    double distanciaTotal;
    double duracaoTotal;
    double eficiencia;
    double velocidadeDoVeiculo;
    double getDistanciaIndividual(Demanda* d) const;

public:
    Corrida(double velocidadeVeiculo);
    ~Corrida();

    void adicionarDemanda(Demanda* demanda);
    void construirRota();
    void calcularEstatisticas();

    Vetor* getListaDemandas() const;
    Vetor* getListaTrechos() const;
    int getNumPassageiros() const;
    int getNumParadas() const;
    double getDistanciaTotal() const;
    double getEficiencia() const;
};

#endif // CORRIDA_HPP