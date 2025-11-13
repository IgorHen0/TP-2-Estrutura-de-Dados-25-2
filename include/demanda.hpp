#ifndef DEMANDA_HPP
#define DEMANDA_HPP

class Corrida; 

struct Ponto {
    double x;
    double y;
};

enum class EstadoDemanda {
    DEMANDADA,
    INDIVIDUAL,
    COMBINADA,
    CONCLUIDA
};

class Demanda {
private:
    int id;
    double tempoSolicitacao;
    Ponto origem;
    Ponto destino;

    EstadoDemanda estado;
    Corrida* corridaAssociada;

public:
    Demanda(int id, double tempo, Ponto origem, Ponto destino);

    virtual ~Demanda();
    
    int getId() const;
    double getTempo() const;
    Ponto getOrigem() const;
    Ponto getDestino() const;
    EstadoDemanda getEstado() const;
    Corrida* getCorrida() const;

    void setEstado(EstadoDemanda novoEstado);
    void setCorrida(Corrida* corrida);
};

#endif // DEMANDA_HPP