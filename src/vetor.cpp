#include "../include/vetor.hpp"
#include <iostream>
#include <cstdlib>
#include <new>

Vetor::Vetor(int capacidadeInicial) {
    if (capacidadeInicial <= 0) {
        capacidadeInicial = CAPACIDADE_INICIAL_DEFAULT;
    }
    
    this->capacidade = capacidadeInicial;
    this->tamanho = 0;

    // Tenta alocar a memória sem lançar exceção
    this->itens = new (std::nothrow) void*[capacidade];

    // Verifica se a alocação falhou
    if (this->itens == nullptr) {
        std::cerr << "Erro fatal: Falha ao alocar memoria (new[]) no construtor do Vetor." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Vetor::~Vetor() {
    // Libera o array de ponteiros
    delete[] this->itens;
}

void Vetor::redimensiona(int novaCapacidade) {
    // Aloca um novo array
    void** novos_itens = new (std::nothrow) void*[novaCapacidade];
    
    // Verifica se a alocação falhou
    if (novos_itens == nullptr) {
        std::cerr << "Erro fatal: Falha ao alocar memoria (new[]) em redimensiona." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Copia os ponteiros antigos para o novo array
    for (int i = 0; i < this->tamanho; i++) {
        novos_itens[i] = this->itens[i];
    }

    // Libera o array antigo
    delete[] this->itens;

    // Atualiza os ponteiros e a capacidade
    this->itens = novos_itens;
    this->capacidade = novaCapacidade;
}

void Vetor::insereNoFim(void* item) {
    // Se o vetor estiver cheio, dobra a capacidade
    if (this->tamanho == this->capacidade) {
        this->redimensiona(this->capacidade * 2);
    }

    // Insere o ponteiro na próxima posição livre
    this->itens[this->tamanho] = item;
    this->tamanho++;
}

void* Vetor::pegaElemento(int indice) {
    // Checagem de segurança
    if (indice < 0 || indice >= this->tamanho) {
        return nullptr;
    }
    return this->itens[indice];
}

int Vetor::getTamanho() const {
    return this->tamanho;
}

bool Vetor::estaVazio() const {
    return this->tamanho == 0;
}