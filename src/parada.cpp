#include "../include/parada.hpp"

Parada::Parada(Ponto coords, TipoParada tipo, Demanda* passageiro) :
    coordenadas(coords),
    tipo(tipo),
    passageiro(passageiro)
{
    // O construtor inicializa tudo através da lista de inicialização.
}

Parada::~Parada() {
    // A 'Demanda' real será liberada pelo 'Vetor' no 'main'.
}

Ponto Parada::getCoordenadas() const {
    return this->coordenadas;
}

TipoParada Parada::getTipo() const {
    return this->tipo;
}

Demanda* Parada::getPassageiro() const {
    return this->passageiro;
}