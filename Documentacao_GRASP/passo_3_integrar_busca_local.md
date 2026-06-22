# Passo 3 - Integrar a Busca Local na Iteracao do GRASP

## Objetivo

Transformar a fase construtiva do passo anterior em uma metaheuristica de fato: cada solucao gerada pelo GRASP deve ser refinada pela busca local ja existente.

## Arquivo principal

- `Comparacao_Oficial/algoritmos.c`

## Estado atual

Hoje a busca local e usada apenas em dois fluxos do projeto:

```c
int lucroBL = aplicarBuscaLocal(itens, m, W_max, V_max, pesoG, volumeG, lucroGuloso);
```

e

```c
int lucroBLAleat = aplicarBuscaLocal(itens, m, W_max, V_max, pesoA, volumeA, lucroAleatorio);
```

Ainda nao existe uma funcao `resolverGRASP(...)` chamando construcao + busca local.

## Como deve ficar

Adicionar uma primeira versao do GRASP completo usando multi-start.

```c
int resolverGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                  float alphaGRASP, int iteracoes,
                  int *pesoFinal, int *volumeFinal)
{
    int melhorLucro = 0;
    int melhorPeso = 0;
    int melhorVolume = 0;

    for (int iteracao = 0; iteracao < iteracoes; iteracao++)
    {
        int pesoAtual = 0;
        int volumeAtual = 0;

        // Gera uma solucao inicial viavel e diversificada.
        int lucroConstrucao = construirSolucaoGRASP(
            itens, n, maxPeso, maxVolume, alphaGRASP, &pesoAtual, &volumeAtual);

        // Refina a solucao usando a busca local ja implementada no projeto.
        int lucroRefinado = aplicarBuscaLocal(
            itens, n, maxPeso, maxVolume, pesoAtual, volumeAtual, lucroConstrucao);

        if (lucroRefinado > melhorLucro)
        {
            melhorLucro = lucroRefinado;
            melhorPeso = pesoAtual;
            melhorVolume = volumeAtual;
        }
    }

    *pesoFinal = melhorPeso;
    *volumeFinal = melhorVolume;
    return melhorLucro;
}
```

## Comentarios importantes

- Esta versao ainda nao preserva a configuracao da melhor iteracao. Esse ajuste entra no passo 4.
- Mesmo assim, ela ja define o fluxo conceitual correto do GRASP:
  construcao randomizada -> busca local -> comparacao global.

## Observacao tecnica

O `lucroRefinado` pode mudar sem atualizar automaticamente `melhorPeso` e `melhorVolume`, porque a busca local atual retorna apenas o lucro. Por isso, neste passo a ideia principal e estrutural. No passo 4 sera necessario preservar melhor o estado da melhor iteracao.

## Resultado esperado deste passo

Ao final deste passo, o GRASP deixa de ser apenas uma construcao aleatoria e passa a funcionar como metaheuristica baseada em multi-start com refinamento local.
