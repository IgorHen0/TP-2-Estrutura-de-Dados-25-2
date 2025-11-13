#include "../include/trecho.hpp"
#include <stdexcept>

Trecho::Trecho(Parada* p1, Parada* p2, double velocidadeVeiculo) :
    paradaOrigem(p1),
    paradaDestino(p2)
{
    if (p1 == nullptr || p2 == nullptr) {
        throw std::runtime_error("Trecho nao pode ser criado com paradas nulas.");
    }

    if (velocidadeVeiculo <= 0) {
        throw std::runtime_error("Velocidade do veiculo deve ser positiva.");
    }

    // Calcula a distância
    this->distancia = Trecho::calculaDistancia(p1, p2);

    // Calcula o tempo (tempo = distancia / velocidade)
    this->tempo = this->distancia / velocidadeVeiculo;

    // Determina o tipo do trecho
    this->tipo = Trecho::determinaTipo(p1, p2);
}

Trecho::~Trecho() {
    // As Paradas serão gerenciadas e liberadas pela classe Corrida.
}

Parada* Trecho::getParadaOrigem() const {
    return this->paradaOrigem;
}

Parada* Trecho::getParadaDestino() const {
    return this->paradaDestino;
}

double Trecho::getDistancia() const {
    return this->distancia;
}

double Trecho::getTempo() const {
    return this->tempo;
}

TipoTrecho Trecho::getTipo() const {
    return this->tipo;
}

double Trecho::calculaDistancia(Parada* p1, Parada* p2) {
    Ponto c1 = p1->getCoordenadas();
    Ponto c2 = p2->getCoordenadas();
    
    // Distância Euclidiana: sqrt((x2-x1)^2 + (y2-y1)^2)
    return std::sqrt(std::pow(c2.x - c1.x, 2) + std::pow(c2.y - c1.y, 2));
}

TipoTrecho Trecho::determinaTipo(Parada* p1, Parada* p2) {
    TipoParada tipo1 = p1->getTipo();
    TipoParada tipo2 = p2->getTipo();

    if (tipo1 == TipoParada::EMBARQUE && tipo2 == TipoParada::EMBARQUE) {
        return TipoTrecho::COLETA;
    }
    else if (tipo1 == TipoParada::DESEMBARQUE && tipo2 == TipoParada::DESEMBARQUE) {
        return TipoTrecho::ENTREGA;
    }
    else if (tipo1 == TipoParada::EMBARQUE && tipo2 == TipoParada::DESEMBARQUE) {
        return TipoTrecho::DESLOCAMENTO;
    }

    return TipoTrecho::INVALIDO;
}