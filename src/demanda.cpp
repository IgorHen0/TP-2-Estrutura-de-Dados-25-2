#include "../include/demanda.hpp"

Demanda::Demanda(int id, double tempo, Ponto origem, Ponto destino) :
    id(id),
    tempoSolicitacao(tempo),
    origem(origem),
    destino(destino),
    estado(EstadoDemanda::DEMANDADA), // Estado inicial é sempre DEMANDADA
    corridaAssociada(nullptr)
{
    // O corpo do construtor está vazio, pois a lista de inicialização fez todo o trabalho.
}

Demanda::~Demanda() {
    // Esta classe não é dona do ponteiro 'corridaAssociada', portanto, ela NÃO deve deletá-lo.
}

int Demanda::getId() const {
    return this->id;
}

double Demanda::getTempo() const {
    return this->tempoSolicitacao;
}

Ponto Demanda::getOrigem() const {
    return this->origem;
}

Ponto Demanda::getDestino() const {
    return this->destino;
}

EstadoDemanda Demanda::getEstado() const {
    return this->estado;
}

Corrida* Demanda::getCorrida() const {
    return this->corridaAssociada;
}

void Demanda::setEstado(EstadoDemanda novoEstado) {
    this->estado = novoEstado;
}

void Demanda::setCorrida(Corrida* corrida) {
    this->corridaAssociada = corrida;
}