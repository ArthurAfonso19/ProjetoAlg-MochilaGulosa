#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "algoritmos.h"
#include "utils.h"

int comparar(const void *a, const void *b)
{
    Item *i1 = (Item *)a;
    Item *i2 = (Item *)b;
    if (i2->score > i1->score)
        return 1;
    if (i2->score < i1->score)
        return -1;
    return 0;
}

float calcularScoreSimples(Item item, int maxPeso, int maxVolume)
{
    int custo = item.peso + item.volume;

    // Evita divisao por zero para itens com peso e volume nulos.
    if (custo == 0)
    {
        return (item.lucro > 0) ? 1.0e30f : 0.0f;
    }

    return (float)item.lucro / custo;
}

float calcularScore(Item item, int maxPeso, int maxVolume)
{
    float alpha = 1.0f / maxPeso;
    float beta = 1.0f / maxVolume;
    float custoNormalizado = (alpha * item.peso) + (beta * item.volume);

    // Mantem itens de custo nulo muito atrativos, sem gerar inf/NaN no GRASP.
    if (custoNormalizado == 0.0f)
    {
        return (item.lucro > 0) ? 1.0e30f : 0.0f;
    }

    return item.lucro / custoNormalizado;
}

int resolverDP(Item itens[], int num_itens, int W_max, int V_max)
{

    int (*dp)[V_max + 1] = calloc(W_max + 1, sizeof(*dp));

    if (dp == NULL)
    {
        printf("Erro: Faltou memória RAM para a matriz DP!\n");
        return -1;
    }

    for (int i = 0; i < num_itens; i++)
    {

        int p = itens[i].peso;
        int v_i = itens[i].volume;
        int l = itens[i].lucro;

        for (int w = W_max; w >= p; w--)
        {
            for (int v = V_max; v >= v_i; v--)
            {
                dp[w][v] = max(dp[w][v], dp[w - p][v - v_i] + l);
            }
        }
    }

    int resultado = dp[W_max][V_max];
    free(dp);
    return resultado;
}

int resolverGulosaSimples(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal)
{
    int pesoAtual = 0;
    int volumeAtual = 0;
    int lucroTotal = 0;

    // Calcula o score de todos antes de ordenar
    for (int i = 0; i < n; i++)
    {
        itens[i].score = calcularScoreSimples(itens[i], maxPeso, maxVolume);
        itens[i].selecionado = 0; // garante que a flag esta limpa
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

int resolverGulosa(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal)
{
    int pesoAtual = 0;
    int volumeAtual = 0;
    int lucroTotal = 0;

    // Calcula o score de todos antes de ordenar
    for (int i = 0; i < n; i++)
    {
        itens[i].score = calcularScore(itens[i], maxPeso, maxVolume);
        itens[i].selecionado = 0; // garante que a flag esta limpa
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

int aplicarBuscaLocal(Item itens[], int n, int maxPeso, int maxVolume, int pesoInicial, int volumeInicial, int lucroInicial)
{
    int pesoAtual = pesoInicial;
    int volumeAtual = volumeInicial;
    int lucroAtual = lucroInicial;
    int houveMelhoria = 1;

    while (houveMelhoria)
    {
        houveMelhoria = 0;

        for (int i = 0; i < n; i++)
        {
            if (itens[i].selecionado == 1)
            {
                for (int j = 0; j < n; j++)
                {
                    if (itens[j].selecionado == 0)
                    {
                        if (itens[j].lucro > itens[i].lucro)
                        {
                            int novoPeso = pesoAtual - itens[i].peso + itens[j].peso;
                            int novoVolume = volumeAtual - itens[i].volume + itens[j].volume;

                            if (novoPeso <= maxPeso && novoVolume <= maxVolume)
                            {
                                itens[i].selecionado = 0;
                                itens[j].selecionado = 1;

                                pesoAtual = novoPeso;
                                volumeAtual = novoVolume;
                                lucroAtual = lucroAtual - itens[i].lucro + itens[j].lucro;

                                houveMelhoria = 1;
                                j = n;
                            }
                        }
                    }
                }
                if (houveMelhoria)
                {
                    i = n;
                }
            }
        }
    }

    return lucroAtual;
}

int resolverAleatorio(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal)
{
    int pesoAtual = 0;
    int volumeAtual = 0;
    int lucroTotal = 0;

    int *indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        indices[i] = i;
        itens[i].selecionado = 0;
    }
    embaralhar(indices, n);

    for (int i = 0; i < n; i++)
    {
        int idx = indices[i];
        if ((pesoAtual + itens[idx].peso <= maxPeso) && (volumeAtual + itens[idx].volume <= maxVolume))
        {
            pesoAtual += itens[idx].peso;
            volumeAtual += itens[idx].volume;
            lucroTotal += itens[idx].lucro;
            itens[idx].selecionado = 1;
        }
    }

    *pesoFinal = pesoAtual;
    *volumeFinal = volumeAtual;
    free(indices);

    return lucroTotal;
}

static void prepararItensGRASP(Item itens[], int n, int maxPeso, int maxVolume)
{
    for (int i = 0; i < n; i++)
    {
        // Reaproveita o mesmo score do guloso otimizado na fase construtiva.
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
                    {
                        melhorScore = itens[i].score;
                    }
                    if (itens[i].score < piorScore)
                    {
                        piorScore = itens[i].score;
                    }
                }
            }
        }

        if (!haCandidato)
        {
            break;
        }

        float limiteRCL = melhorScore - alphaGRASP * (melhorScore - piorScore);
        int tamanhoRCL = 0;

        // Segundo passo: guardar na RCL apenas candidatos fortes.
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

static void recalcularMedidasSolucao(Item itens[], int n, int *pesoTotal, int *volumeTotal)
{
    *pesoTotal = 0;
    *volumeTotal = 0;

    // Reconstroi peso e volume a partir do estado real da melhor solucao restaurada.
    for (int i = 0; i < n; i++)
    {
        if (itens[i].selecionado)
        {
            *pesoTotal += itens[i].peso;
            *volumeTotal += itens[i].volume;
        }
    }
}

int resolverGRASP(Item itens[], int n, int maxPeso, int maxVolume,
                  float alphaGRASP, int interacoes,
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

    for (int interacao = 0; interacao < interacoes; interacao++)
    {
        int pesoAtual = 0;
        int volumeAtual = 0;

        // Gera uma solucao inicial viavel e diversificada.
        int lucroConstrucao = construirSolucaoGRASP(
            itens, n, maxPeso, maxVolume, alphaGRASP, &pesoAtual, &volumeAtual);

        // Refina a solucao corrente e preserva apenas a melhor iteracao.
        int lucroRefinado = aplicarBuscaLocal(
            itens, n, maxPeso, maxVolume, pesoAtual, volumeAtual, lucroConstrucao);

        if (lucroRefinado > melhorLucro)
        {
            melhorLucro = lucroRefinado;
            // Salva a configuracao completa da melhor solucao encontrada ate aqui.
            memcpy(melhorItens, itens, n * sizeof(Item));
        }
    }

    // Restaura no vetor principal exatamente a melhor configuracao do GRASP.
    memcpy(itens, melhorItens, n * sizeof(Item));
    recalcularMedidasSolucao(itens, n, &melhorPeso, &melhorVolume);

    free(melhorItens);

    *pesoFinal = melhorPeso;
    *volumeFinal = melhorVolume;
    return melhorLucro;
}
