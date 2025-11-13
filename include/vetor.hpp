#ifndef VETOR_HPP
#define VETOR_HPP

// Define a capacidade inicial padrão
#define CAPACIDADE_INICIAL_DEFAULT 10

class Vetor {
private:
    
    void** itens;
    int tamanho;
    int capacidade;

    void redimensiona(int novaCapacidade);

public:

    Vetor(int capacidadeInicial = CAPACIDADE_INICIAL_DEFAULT);
    ~Vetor();

    void insereNoFim(void* item);
    void* pegaElemento(int indice);
    int getTamanho() const;
    bool estaVazio() const;
};

#endif // VETOR_HPP