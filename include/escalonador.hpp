#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#define CAPACIDADE_INICIAL_HEAP 10

class Escalonador {
private:
    struct EventoNode {
        double tempo;
        void* item;
    };

    EventoNode* heap;
    int tamanho;
    int capacidade;

    void heapifySobe(int index);
    void heapifyDesce(int index);
    void troca(int i, int j);
    void redimensiona();

    int getPai(int index) { return (index - 1) / 2; }
    int getFilhoEsquerda(int index) { return (2 * index) + 1; }
    int getFilhoDireita(int index) { return (2 * index) + 2; }

public:
    Escalonador(int capacidadeInicial = CAPACIDADE_INICIAL_HEAP);
    ~Escalonador();

    void insereEvento(double tempo, void* item);
    void* retiraProximoEvento();
    bool estaVazio() const;
    int getTamanho() const;
};

#endif // ESCALONADOR_HPP