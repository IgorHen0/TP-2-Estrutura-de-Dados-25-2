#ifndef TRECHO_HPP
#define TRECHO_HPP

#include "../include/parada.hpp"
#include <cmath>

enum class TipoTrecho {
    COLETA,
    ENTREGA,
    DESLOCAMENTO,
    INVALIDO
};

class Trecho {
private:
    Parada* paradaOrigem;
    Parada* paradaDestino;
    double distancia;
    double tempo;
    TipoTrecho tipo;

    static double calculaDistancia(Parada* p1, Parada* p2);
    static TipoTrecho determinaTipo(Parada* p1, Parada* p2);

public:
    Trecho(Parada* p1, Parada* p2, double velocidadeVeiculo);
    ~Trecho();
    
    Parada* getParadaOrigem() const;
    Parada* getParadaDestino() const;
    double getDistancia() const;
    double getTempo() const;
    TipoTrecho getTipo() const;
};

#endif // TRECHO_HPP