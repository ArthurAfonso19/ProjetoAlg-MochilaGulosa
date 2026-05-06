#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
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
    return item.lucro / (item.peso + item.volume);
}

float calcularScore(Item item, int maxPeso, int maxVolume)
{
    float alpha = 1.0 / maxPeso;
    float beta = 1.0 / maxVolume;
    return item.lucro / ((alpha * item.peso) + (beta * item.volume));
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
        printf(" %d itens calculados!\n", i);
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