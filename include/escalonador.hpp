/*
 * Arquivo: include/Escalonador.hpp
 * Descrição: Interface da classe Escalonador (Fila de Prioridade com Min-Heap).
 * Armazena ponteiros (void*) com uma prioridade (double).
 */

#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#define CAPACIDADE_INICIAL_HEAP 10

class Escalonador {
private:
    // --- Estrutura Interna ---

    /*
     * @brief Nó interno para armazenar o par (prioridade, item).
     * A prioridade (tempo) é o que ordena o heap.
     * O item (evento) é o ponteiro de dados genérico.
     */
    struct EventoNode {
        double tempo; // A chave de prioridade
        void* item;   // O dado (ponteiro para o evento)
    };

    // --- Membros Privados ---

    EventoNode* heap; // Array que armazena a árvore do heap
    int tamanho;
    int capacidade;

    // --- Métodos Privados (Lógica do Heap) ---

    /**
     * @brief Faz um item "subir" no heap para manter a propriedade de min-heap.
     * @param index O índice do item a ser movido para cima.
     */
    void heapifySobe(int index);

    /**
     * @brief Faz um item "descer" no heap para manter a propriedade de min-heap.
     * @param index O índice do item a ser movido para baixo.
     */
    void heapifyDesce(int index);

    /**
     * @brief Troca dois EventoNodes de lugar no array do heap.
     */
    void troca(int i, int j);

    /**
     * @brief Redimensiona o array do heap quando estiver cheio.
     */
    void redimensiona();

    // --- Métodos Privados (Aritmética de Índices) ---

    int getPai(int index) { return (index - 1) / 2; }
    int getFilhoEsquerda(int index) { return (2 * index) + 1; }
    int getFilhoDireita(int index) { return (2 * index) + 2; }

public:
    // --- Construtor e Destrutor ---

    /**
     * @brief Construtor (Inicializa).
     */
    Escalonador(int capacidadeInicial = CAPACIDADE_INICIAL_HEAP);

    /**
     * @brief Destrutor (Finaliza).
     */
    ~Escalonador();

    // --- Métodos Públicos (Interface do PDF) ---

    /**
     * @brief Insere um evento na fila de prioridade.
     * @param tempo A prioridade do evento (ex: tempo de ocorrência).
     * @param item O ponteiro para o evento a ser armazenado.
     */
    void insereEvento(double tempo, void* item);

    /**
     * @brief Retira e retorna o próximo evento (o de menor tempo/prioridade).
     * @return O ponteiro (void*) para o evento, ou nullptr se o heap estiver vazio.
     */
    void* retiraProximoEvento();

    // --- Métodos Auxiliares ---

    /**
     * @brief Verifica se o escalonador está vazio.
     */
    bool estaVazio() const;

    /**
     * @brief Retorna o número de eventos na fila.
     */
    int getTamanho() const;
};

#endif // ESCALONADOR_HPP