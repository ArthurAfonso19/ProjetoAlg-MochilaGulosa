# Passo 2 - Implementar a Construcao Gulosa Randomizada

## Objetivo

Criar, em `Comparacao_Oficial/algoritmos.c`, a parte construtiva do GRASP: uma solucao inicial viavel baseada em score guloso, mas com escolha aleatoria dentro de uma `RCL`.

## Arquivo principal

- `Comparacao_Oficial/algoritmos.c`

## Estado atual

Hoje existe apenas a construcao gulosa deterministica.

```c
int resolverGulosa(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal)
{
    int pesoAtual = 0;
    int volumeAtual = 0;
    int lucroTotal = 0;

    for (int i = 0; i < n; i++)
    {
        itens[i].score = calcularScore(itens[i], maxPeso, maxVolume);
        itens[i].selecionado = 0;
    }

    qsort(itens, n, sizeof(Item), comparar);

    for (int i = 0; i < n; i++)
    {
        if ((pesoAtual + itens[i].peso <= maxPeso) && (volumeAtual + itens[i].volume <= maxVolume))
        {
            pesoAtual += itens[i].peso;
            volumeAtual += itens[i].volume;
            lucroTotal += itens[i].lucro;
            itens[i].selecionado = 1;
        }
    }

    *pesoFinal = pesoAtual;
    *volumeFinal = volumeAtual;
    return lucroTotal;
}
```

Esse metodo ordena todos os itens e escolhe sempre o melhor item disponivel. No GRASP, isso precisa ser parcialmente aleatorizado.

## Como deve ficar

Adicionar helpers internos para a construcao GRASP.

```c
static void prepararItensGRASP(Item itens[], int n, int maxPeso, int maxVolume)
{
    for (int i = 0; i < n; i++)
    {
        // Reaproveita o mesmo score do guloso otimizado.
        itens[i].score = calcularScore(itens[i], maxPeso, maxVolume);
        itens[i].selecionado = 0;
    }
}

static int construirSolucaoGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                                 float alphaGRASP, int *pesoFinal, int *volumeFinal)
{
    int pesoAtual = 0;
    int volumeAtual = 0;
    int lucroTotal = 0;
    int *rcl = malloc(n * sizeof(int));

    if (rcl == NULL)
    {
        *pesoFinal = 0;
        *volumeFinal = 0;
        return 0;
    }

    prepararItensGRASP(itens, n, maxPeso, maxVolume);

    while (1)
    {
        float melhorScore = -1.0f;
        float piorScore = -1.0f;
        int haCandidato = 0;

        // Primeiro passo: localizar apenas candidatos viaveis.
        for (int i = 0; i < n; i++)
        {
            if (itens[i].selecionado == 0 &&
                pesoAtual + itens[i].peso <= maxPeso &&
                volumeAtual + itens[i].volume <= maxVolume)
            {
                if (!haCandidato)
                {
                    melhorScore = itens[i].score;
                    piorScore = itens[i].score;
                    haCandidato = 1;
                }
                else
                {
                    if (itens[i].score > melhorScore)
                        melhorScore = itens[i].score;
                    if (itens[i].score < piorScore)
                        piorScore = itens[i].score;
                }
            }
        }

        if (!haCandidato)
        {
            break;
        }

        float limiteRCL = melhorScore - alphaGRASP * (melhorScore - piorScore);
        int tamanhoRCL = 0;

        // Segundo passo: guardar na RCL apenas os candidatos fortes.
        for (int i = 0; i < n; i++)
        {
            if (itens[i].selecionado == 0 &&
                pesoAtual + itens[i].peso <= maxPeso &&
                volumeAtual + itens[i].volume <= maxVolume &&
                itens[i].score >= limiteRCL)
            {
                rcl[tamanhoRCL] = i;
                tamanhoRCL++;
            }
        }

        if (tamanhoRCL == 0)
        {
            break;
        }

        // Escolha aleatoria dentro da lista restrita.
        int escolhido = rcl[rand() % tamanhoRCL];
        itens[escolhido].selecionado = 1;
        pesoAtual += itens[escolhido].peso;
        volumeAtual += itens[escolhido].volume;
        lucroTotal += itens[escolhido].lucro;
    }

    free(rcl);
    *pesoFinal = pesoAtual;
    *volumeFinal = volumeAtual;
    return lucroTotal;
}
```

## Comentarios de implementacao

- `prepararItensGRASP(...)` limpa o estado anterior dos itens.
- `construirSolucaoGRASP(...)` nao substitui ainda o GRASP completo; ela implementa apenas a fase construtiva.
- A `RCL` e montada a cada iteracao da construcao.
- O score usado e o mesmo do guloso refinado, para manter comparabilidade.

## Resultado esperado deste passo

Ao final deste passo, o projeto tera uma construcao inicial mais diversificada do que o guloso puro, pronta para ser refinada com busca local.
