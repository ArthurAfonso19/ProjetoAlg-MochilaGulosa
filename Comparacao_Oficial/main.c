#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOMINMAX
#include <windows.h>

#include "item.h"
#include "utils.h"
#include "algoritmos.h"

int main()
{
    srand(time(NULL));

    LARGE_INTEGER frequencia;
    QueryPerformanceFrequency(&frequencia);

    printf("=== COMPARATIVO DAS INSTANCIAS OFICIAIS (ALTA PRECISAO) ===\n");

    for (int id = 1; id <= 8; id++)
    {
        int W_max;
        int V_max;
        int m;
        char nome_arquivo[256];

        sprintf(nome_arquivo, "../Instancias de Teste/Teste%d.txt", id);

        Item *itensOriginal = lerInstancia(nome_arquivo, &m, &W_max, &V_max);
        if (itensOriginal == NULL)
        {
            printf("Aviso: Instancia %d nao encontrada. Pulando...\n", id);
            continue;
        }

        Item *itens = (Item *)malloc(m * sizeof(Item));
        if (itens == NULL)
        {
            printf("Erro de alocacao na instancia %d. Pulando...\n", id);
            free(itensOriginal);
            continue;
        }

        // ==========================================
        // Programacao Dinamica (otimo/baseline)
        // ==========================================
        LARGE_INTEGER inicioDP, fimDP;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        QueryPerformanceCounter(&inicioDP);
        int lucroDP = resolverDP(itens, m, W_max, V_max);
        QueryPerformanceCounter(&fimDP);
        double tempoDP = (double)(fimDP.QuadPart - inicioDP.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Guloso Simples
        // ==========================================
        int pesoGS = 0, volumeGS = 0;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        LARGE_INTEGER inicioGS, fimGS;
        QueryPerformanceCounter(&inicioGS);
        int lucroGulosoSimples = resolverGulosaSimples(itens, m, W_max, V_max, &pesoGS, &volumeGS);
        QueryPerformanceCounter(&fimGS);
        double tempoGulosoSimples = (double)(fimGS.QuadPart - inicioGS.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Guloso Otimizado
        // ==========================================
        int pesoG = 0, volumeG = 0;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        LARGE_INTEGER inicioG, fimG;
        QueryPerformanceCounter(&inicioG);
        int lucroGuloso = resolverGulosa(itens, m, W_max, V_max, &pesoG, &volumeG);
        QueryPerformanceCounter(&fimG);
        double tempoGuloso = (double)(fimG.QuadPart - inicioG.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Busca Local a partir do Guloso (mesmo estado de itens)
        // ==========================================
        LARGE_INTEGER inicioBL, fimBL;
        QueryPerformanceCounter(&inicioBL);
        int lucroBL = aplicarBuscaLocal(itens, m, W_max, V_max, pesoG, volumeG, lucroGuloso);
        QueryPerformanceCounter(&fimBL);
        double tempoBL = (double)(fimBL.QuadPart - inicioBL.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Construcao Aleatoria
        // ==========================================
        int pesoA = 0, volumeA = 0;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        LARGE_INTEGER inicioA, fimA;
        QueryPerformanceCounter(&inicioA);
        int lucroAleatorio = resolverAleatorio(itens, m, W_max, V_max, &pesoA, &volumeA);
        QueryPerformanceCounter(&fimA);
        double tempoAleat = (double)(fimA.QuadPart - inicioA.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Busca Local a partir da Solucao Aleatoria
        // ==========================================
        LARGE_INTEGER inicioBLA, fimBLA;
        QueryPerformanceCounter(&inicioBLA);
        int lucroBLAleat = aplicarBuscaLocal(itens, m, W_max, V_max, pesoA, volumeA, lucroAleatorio);
        QueryPerformanceCounter(&fimBLA);
        double tempoBLAleat = (double)(fimBLA.QuadPart - inicioBLA.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // GRASP
        // ==========================================
        int pesoGRASP = 0, volumeGRASP = 0;
        float alphaGRASP = 0.20f;
        int iteracoesGRASP = 30; 

        memcpy(itens, itensOriginal, m * sizeof(Item));
        LARGE_INTEGER inicioGRASP, fimGRASP; 
        QueryPerformanceCounter(&inicioGRASP);
        int lucroGRASP = resolverGRASP(
            itens, m, W_max, V_max,
            alphaGRASP, iteracoesGRASP,
            &pesoGRASP, &volumeGRASP
        );
        QueryPerformanceCounter(&fimGRASP);

        double tempoGRASP = (double)(fimGRASP.QuadPart - inicioGRASP.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Imprimindo comparativo
        // ==========================================
        resultados(id, m, W_max, V_max,
                   lucroGulosoSimples, tempoGulosoSimples,
                   lucroGuloso, tempoGuloso,
                   lucroBL, tempoBL,
                   lucroBLAleat, tempoAleat, tempoBLAleat,
                   lucroGRASP, tempoGRASP,
                   lucroDP, tempoDP);

        free(itens);
        free(itensOriginal);
    }

    printf("\n=== TODOS OS TESTES FORAM CONCLUIDOS ===\n");
    return 0;
}