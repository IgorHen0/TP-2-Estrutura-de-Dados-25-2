#ifndef PARADA_HPP
#define PARADA_HPP

#include "../include/demanda.hpp" 

enum class TipoParada {
    EMBARQUE,
    DESEMBARQUE
};

class Parada {
private:
    Ponto coordenadas;
    TipoParada tipo;
    Demanda* passageiro;

public:
    Parada(Ponto coords, TipoParada tipo, Demanda* passageiro);

    ~Parada();

    Ponto getCoordenadas() const;
    TipoParada getTipo() const;
    Demanda* getPassageiro() const;
};

#endif // PARADA_HPP