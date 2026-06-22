# Passo 1 - Definir a Interface Publica do GRASP

## Objetivo

Adicionar ao modulo `Comparacao_Oficial/algoritmos.h` a assinatura publica do novo metodo `GRASP`, mantendo o estilo do projeto atual.

## Arquivo principal

- `Comparacao_Oficial/algoritmos.h`

## Estado atual

Hoje o arquivo expoe apenas os metodos ja existentes.

```c
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <time.h>
#include "item.h"

float calcularScore(Item item, int maxPeso, int maxVolume);
float calcularScoreSimples(Item item, int maxPeso, int maxVolume);
int resolverGulosaSimples(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int resolverGulosa(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int aplicarBuscaLocal(Item itens[], int n, int maxPeso, int maxVolume, int pesoInicial, int volumeInicial, int lucroInicial);
int resolverAleatorio(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int resolverDP(Item itens[], int num_itens, int W_max, int V_max);

#endif
```

## Como deve ficar

Adicionar uma nova assinatura para o GRASP.

```c
#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <time.h>
#include "item.h"

float calcularScore(Item item, int maxPeso, int maxVolume);
float calcularScoreSimples(Item item, int maxPeso, int maxVolume);
int resolverGulosaSimples(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int resolverGulosa(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int aplicarBuscaLocal(Item itens[], int n, int maxPeso, int maxVolume, int pesoInicial, int volumeInicial, int lucroInicial);
int resolverAleatorio(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int resolverDP(Item itens[], int num_itens, int W_max, int V_max);

// GRASP: retorna o melhor lucro encontrado.
// alpha controla o tamanho da RCL.
// iteracoes controla quantas construcoes + busca local serao executadas.
int resolverGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                  float alphaGRASP, int iteracoes,
                  int *pesoFinal, int *volumeFinal);

#endif
```

## Comentarios sobre a assinatura

```c
int resolverGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                  float alphaGRASP, int iteracoes,
                  int *pesoFinal, int *volumeFinal);
```

- `itens[]`: vetor de trabalho onde o GRASP vai marcar `selecionado`.
- `n`: quantidade de itens da instancia.
- `maxPeso` e `maxVolume`: capacidades da mochila.
- `alphaGRASP`: controla o nivel de aleatoriedade da RCL.
- `iteracoes`: numero de vezes que o GRASP sera executado.
- `pesoFinal` e `volumeFinal`: retornam o consumo da melhor solucao encontrada.

## Decisao de projeto

Nesta primeira versao, o lucro continua sendo o valor retornado pela funcao, como ja acontece com os outros metodos do projeto.

## Resultado esperado deste passo

Ao final deste passo, o projeto ainda nao executa o GRASP, mas ja possui a interface publica necessaria para a implementacao dos proximos passos.
