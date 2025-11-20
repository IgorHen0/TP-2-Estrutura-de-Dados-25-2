# TP 2 Estrutura de Dados - Sistema de Despacho de Transporte por Aplicativo

Este projeto implementa um Simulador de Eventos Discretos (SED) para um sistema de despacho de transporte por aplicativo, com foco na otimização de corridas por meio de compartilhamento.

---

## Problema e Contexto

A empresa de transporte por aplicativo Cabe Aí (filial da multinacional CabAl) propôs uma inovação: a sugestão de corridas compartilhadas. O problema consiste em, dada uma sequência cronológica de demandas de corrida, implementar um sistema de despacho capaz de selecionar o veículo e, mais importante, investigar e formar grupos de corridas que possam ser compartilhadas.

O agrupamento de corridas é regido por um conjunto de critérios e parâmetros de simulação:

| Parâmetro | Descrição |
| :--- | :--- |
| **η** (eta) | **Capacidade do veículo** (número máximo de passageiros). |
| **δ** (delta) | **Intervalo entre partidas** (janela temporal máxima para combinar corridas). |
| **α** (alfa) | **Distância máxima entre origens** (para pares de corridas a serem combinadas). |
| **β** (beta) | **Distância máxima entre destinos** (para pares de corridas a serem combinadas). |
| **λ** (lambda) | **Eficiência mínima** (limiar mínimo de eficiência da corrida compartilhada). |
| **γ** (gama) | **Velocidade dos veículos** (usada para calcular a duração do trecho). |  

---  

## Solução e Arquitetura  

O sistema é implementado em C++11 seguindo uma arquitetura modular, com foco na implementação manual de estruturas de dados (vetado o uso de containers da STL, como *std::vector* ou *std::queue*).

**Componentes Chave**  

| Componente | Função | Estrutura de Dados |
| :--- | :--- | :--- |
| **Simulador (SED)** | Gerencia o avanço do tempo e o processamento dos eventos na ordem cronológica. | - |
| `Escalonador` | Fila de prioridade central do SED. Armazena eventos de chegada de veículos (fim de trecho) e recupera o evento de menor tempo de ocorrência. | **Min-Heap** |
| `Vetor` | Estrutura de array dinâmico genérico, utilizada para armazenar a lista de demandas, corridas, paradas e trechos. | **Array Dinâmico Redimensionável** |
| **Lógica de Agrupamento** | Implementa uma **heurística gulosa** (`agrupaCorridas`) que tenta combinar demandas sequenciais se satisfizerem os critérios η, δ, α, β, λ. | - |
| **TADs de Domínio** | Modelam as entidades do negócio: `Demanda`, `Corrida`, `Trecho`, `Parada`. | - |  

**Fases de Execução**  

1. **Agrupamento** (agrupaCorridas): Ocorre antes da simulação. Percorre as demandas cronologicamente, tentando adicionar demandas futuras ci​ à corrida corrente C=c0​ até que um critério de capacidade (η), tempo (δ) ou eficiência (λ) seja violado.  
2. **Simulação** (executaSimulacao): Processa os eventos (chegadas de veículos ao final de um trecho) em ordem de tempo, extraindo-os do Escalonador e agendando o próximo evento daquela corrida.  

---  

## Funcionalidades do Programa  

* Leitura de Entradas: Recebe parâmetros de simulação (η,γ,δ,α,β,λ) e a lista de demandas (id, tempo de solicitação, origem, destino) via stdin.

* Formação de Corridas Compartilhadas: Agrupa demandas de forma gulosa, garantindo a satisfação de todos os critérios de compartilhamento (capacidade, temporal, distância de origem/destino, e eficiência mínima).

* Modelagem de Rota: Constrói a sequência de paradas (embarque e desembarque) e trechos (coleta, deslocamento, entrega) para cada corrida.

* Simulação de Eventos Discretos: Executa a simulação em ordem cronológica de eventos, utilizando um Min-Heap para priorizar o processamento.

* Geração de Saída (Estatísticas): Para cada corrida concluída (chegada ao destino final do último passageiro), o programa imprime na saída padrão (stdout):  
    1. Tempo de conclusão da corrida;  
    2. Distância total percorrida;  
    3. Número de paradas;  
    4. Sequência de coordenadas das paradas.  

---  

## Instruções de Uso  

**Estrutura do Projeto**  
O projeto segue a estrutura de diretórios padrão:  

├── src/ (Arquivos fonte *.cpp)  
├── include/ (Arquivos de cabeçalho *.hpp)  
├── bin/ (Executável tp1.out)  
├── obj/ (Arquivos objeto *.o)  
└── Makefile (Script de compilação)  

**Compilação**  

1. Acesse o diretório raiz do projeto.
2. Execute o *Makefile* com o comando:  
```
make all
```  
3. O comando irá compilar o código e gerar o executável *tp2.out* dentro do diretório *bin*.  

**Execução**  

O programa lê a entrada padrão da **entrada padrão**. Para executar o simulador, redirecione o arquivo de entrada (*.txt*) para o executável:  
```
./bin/tp2.out < caminho/para/seu/arquivo_de_entrada.txt
```
Para salvar a saída em um arquivo, use o redirecionamento de saída:  
```
./bin/tp2.out < caminho/para/seu/arquivo_de_entrada.txt > arquivo_de_saida.txt
```