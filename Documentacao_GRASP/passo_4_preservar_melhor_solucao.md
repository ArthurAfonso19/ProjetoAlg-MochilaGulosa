# Passo 4 - Preservar a Melhor Solucao Entre Iteracoes

## Objetivo

Garantir que a melhor iteracao do GRASP nao se perca, ja que o vetor `itens[]` e alterado continuamente pelas proximas iteracoes.

## Arquivo principal

- `Comparacao_Oficial/algoritmos.c`

## Problema do estado atual

Se o GRASP rodar varias iteracoes sobre o mesmo vetor, a melhor configuracao de `selecionado` pode ser sobrescrita.

Exemplo da estrutura anterior:

```c
for (int iteracao = 0; iteracao < iteracoes; iteracao++)
{
    int lucroConstrucao = construirSolucaoGRASP(...);
    int lucroRefinado = aplicarBuscaLocal(...);

    if (lucroRefinado > melhorLucro)
    {
        melhorLucro = lucroRefinado;
    }
}
```

Esse trecho guarda apenas o lucro, mas nao guarda quais itens estavam na melhor solucao.

## Como deve ficar

Criar uma copia da melhor configuracao encontrada.

```c
int resolverGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                  float alphaGRASP, int iteracoes,
                  int *pesoFinal, int *volumeFinal)
{
    int melhorLucro = 0;
    int melhorPeso = 0;
    int melhorVolume = 0;
    Item *melhorItens = malloc(n * sizeof(Item));

    if (melhorItens == NULL)
    {
        *pesoFinal = 0;
        *volumeFinal = 0;
        return 0;
    }

    for (int iteracao = 0; iteracao < iteracoes; iteracao++)
    {
        int pesoAtual = 0;
        int volumeAtual = 0;

        int lucroConstrucao = construirSolucaoGRASP(
            itens, n, maxPeso, maxVolume, alphaGRASP, &pesoAtual, &volumeAtual);

        int lucroRefinado = aplicarBuscaLocal(
            itens, n, maxPeso, maxVolume, pesoAtual, volumeAtual, lucroConstrucao);

        if (lucroRefinado > melhorLucro)
        {
            melhorLucro = lucroRefinado;
            melhorPeso = pesoAtual;
            melhorVolume = volumeAtual;

            // Salva a melhor configuracao encontrada ate agora.
            memcpy(melhorItens, itens, n * sizeof(Item));
        }
    }

    // Restaura no vetor principal a melhor configuracao do GRASP.
    memcpy(itens, melhorItens, n * sizeof(Item));

    free(melhorItens);
    *pesoFinal = melhorPeso;
    *volumeFinal = melhorVolume;
    return melhorLucro;
}
```

## Ajuste recomendado

Para deixar `pesoFinal` e `volumeFinal` realmente coerentes com a solucao final, vale adicionar uma rotina que recalcule peso e volume a partir dos itens selecionados.

```c
static void recalcularMedidasSolucao(Item itens[], int n, int *pesoTotal, int *volumeTotal)
{
    *pesoTotal = 0;
    *volumeTotal = 0;

    for (int i = 0; i < n; i++)
    {
        if (itens[i].selecionado)
        {
            *pesoTotal += itens[i].peso;
            *volumeTotal += itens[i].volume;
        }
    }
}
```

E depois usar:

```c
memcpy(itens, melhorItens, n * sizeof(Item));
recalcularMedidasSolucao(itens, n, &melhorPeso, &melhorVolume);
```

## Resultado esperado deste passo

Ao final deste passo, o GRASP passa a devolver nao apenas o melhor lucro, mas tambem o estado real da melhor solucao encontrada.
