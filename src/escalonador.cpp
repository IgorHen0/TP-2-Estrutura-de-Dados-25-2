#include "../include/escalonador.hpp"
#include <iostream>
#include <cstdlib>
#include <new>

Escalonador::Escalonador(int capacidadeInicial) {
    if (capacidadeInicial <= 0) {
        capacidadeInicial = CAPACIDADE_INICIAL_HEAP;
    }
    
    this->capacidade = capacidadeInicial;
    this->tamanho = 0;

    // Aloca o array inicial para os nós do heap
    this->heap = new (std::nothrow) EventoNode[capacidade];
    
    // Verifica se a alocação falhou
    if (this->heap == nullptr) {
        std::cerr << "Erro fatal: Falha ao alocar memoria (new[]) no construtor do Escalonador." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

Escalonador::~Escalonador() {
    delete[] this->heap;
}

void Escalonador::insereEvento(double tempo, void* item) {
    // Redimensiona se o heap estiver cheio
    if (tamanho == capacidade) {
        redimensiona();
    }

    // Insere o novo evento no final do heap
    EventoNode& novoNode = heap[tamanho];
    novoNode.tempo = tempo;
    novoNode.item = item;
    
    this->tamanho++;

    // Faz o novo evento "subir" para sua posição correta
    heapifySobe(tamanho - 1);
}

void* Escalonador::retiraProximoEvento() {
    // Se estiver vazio, não há o que retirar
    if (estaVazio()) {
        return nullptr;
    }

    // O próximo evento é sempre a raiz (índice 0)
    void* proximoItem = heap[0].item;

    // Move o último evento do heap para a raiz
    heap[0] = heap[tamanho - 1];
    this->tamanho--;

    // Faz o evento (que era o último) "descer" para sua posição correta
    heapifyDesce(0);

    return proximoItem;
}

bool Escalonador::estaVazio() const {
    return this->tamanho == 0;
}

int Escalonador::getTamanho() const {
    return this->tamanho;
}

void Escalonador::heapifySobe(int index) {
    while (index > 0 && heap[index].tempo < heap[getPai(index)].tempo) {
        troca(index, getPai(index));
        index = getPai(index);
    }
}

void Escalonador::heapifyDesce(int index) {
    int indexMenorFilho = index;

    while (true) {
        int filhoEsquerda = getFilhoEsquerda(index);
        int filhoDireita = getFilhoDireita(index);
        
        // Verifica se o filho da esquerda existe e se é menor que o pai
        if (filhoEsquerda < tamanho && heap[filhoEsquerda].tempo < heap[indexMenorFilho].tempo) {
            indexMenorFilho = filhoEsquerda;
        }

        // Verifica se o filho da direita existe e se é menor que o (pai ou filho da esquerda)
        if (filhoDireita < tamanho && heap[filhoDireita].tempo < heap[indexMenorFilho].tempo) {
            indexMenorFilho = filhoDireita;
        }

        if (indexMenorFilho == index) {
            break; 
        }

        troca(index, indexMenorFilho);
        index = indexMenorFilho;
    }
}

void Escalonador::troca(int i, int j) {
    EventoNode temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void Escalonador::redimensiona() {
    int novaCapacidade = capacidade * 2;
    EventoNode* novoHeap = new (std::nothrow) EventoNode[novaCapacidade];

    if (novoHeap == nullptr) {
        std::cerr << "Erro fatal: Falha ao alocar memoria (new[]) em redimensiona." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Copia os itens antigos para o novo array
    for (int i = 0; i < tamanho; i++) {
        novoHeap[i] = heap[i];
    }

    delete[] heap;
    this->heap = novoHeap;
    this->capacidade = novaCapacidade;
}