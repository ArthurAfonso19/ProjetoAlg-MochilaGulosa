#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Evita conflito das funções max() e min() da windows.h com as suas
#define NOMINMAX
#include <windows.h>

#include "item.h"
#include "utils.h"
#include "algoritmos.h"

int main()
{
    srand(time(NULL));

    // Variável para guardar a frequência do processador (usado no cálculo do tempo)
    LARGE_INTEGER frequencia;
    QueryPerformanceFrequency(&frequencia);

    printf("=== INICIANDO COMPARATIVO DAS 8 INSTANCIAS (ALTA PRECISAO) ===\n");

    for (int id = 1; id <= 8; id++)
    {
        int W_max;
        int V_max;
        int m;
        int lucroOtimo;
        double tempoDP;
        char nome_arquivo[256];

        // Usa o diretorio vizinho GeracaoInstancia para carregar as instancias geradas.
        sprintf(nome_arquivo, "../GeracaoInstancia/instancia%d.txt", id);

        Item *itensOriginal = lerInstancia(nome_arquivo, &m, &W_max, &V_max, &lucroOtimo, &tempoDP);
        if (itensOriginal == NULL)
        {
            printf("Aviso: Instancia %d nao encontrada. Pulando...\n", id);
            continue;
        }

        // Buffer auxiliar para isolar cada metodo e evitar contaminacao por qsort/selecionado.
        Item *itens = (Item *)malloc(m * sizeof(Item));
        if (itens == NULL)
        {
            printf("Erro de alocacao na instancia %d. Pulando...\n", id);
            free(itensOriginal);
            continue;
        }

        int pesoGuloso = 0;
        int volumeGuloso = 0;
        int lucroGuloso = 0;
        double tempoGuloso = 0;
        int lucroGulosoSimples = 0;
        double tempoGulosoSimples = 0;

        // ==========================================
        // Execução da Heuristica Gulosa
        // ==========================================
        // Guloso Simples
        LARGE_INTEGER inicioGulosoSimples, fimGulosoSimples;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        QueryPerformanceCounter(&inicioGulosoSimples);
        lucroGulosoSimples = resolverGulosaSimples(itens, m, W_max, V_max, &pesoGuloso, &volumeGuloso);
        QueryPerformanceCounter(&fimGulosoSimples);
        tempoGulosoSimples = (double)(fimGulosoSimples.QuadPart - inicioGulosoSimples.QuadPart) / frequencia.QuadPart;

        // Guloso Otimizado
        LARGE_INTEGER inicioGuloso, fimGuloso;
        memcpy(itens, itensOriginal, m * sizeof(Item));
        QueryPerformanceCounter(&inicioGuloso);
        lucroGuloso = resolverGulosa(itens, m, W_max, V_max, &pesoGuloso, &volumeGuloso);
        QueryPerformanceCounter(&fimGuloso);
        tempoGuloso = (double)(fimGuloso.QuadPart - inicioGuloso.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Execução da Busca Local a partir do guloso
        // ==========================================
        LARGE_INTEGER inicioBL, fimBL;
        QueryPerformanceCounter(&inicioGuloso);
        memcpy(itens, itensOriginal, m * sizeof(Item));
        lucroGuloso = resolverGulosa(itens, m, W_max, V_max, &pesoGuloso, &volumeGuloso);
        QueryPerformanceCounter(&fimGuloso);
        tempoGuloso = (double)(fimGuloso.QuadPart - inicioGuloso.QuadPart) / frequencia.QuadPart;

        QueryPerformanceCounter(&inicioBL);
        int lucroBL = aplicarBuscaLocal(itens, m, W_max, V_max, pesoGuloso, volumeGuloso, lucroGuloso);
        QueryPerformanceCounter(&fimBL);
        double tempoBL = (double)(fimBL.QuadPart - inicioBL.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Execução da Construção Aleatória
        // ==========================================
        int pesoAleat = 0, volumeAleat = 0;
        LARGE_INTEGER inicioAleat, fimAleat;

        memcpy(itens, itensOriginal, m * sizeof(Item));
        QueryPerformanceCounter(&inicioAleat);
        int lucroAleatorio = resolverAleatorio(itens, m, W_max, V_max, &pesoAleat, &volumeAleat);
        QueryPerformanceCounter(&fimAleat);
        double tempoAleat = (double)(fimAleat.QuadPart - inicioAleat.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Busca Local iniciada a partir da Solução Aleatória
        // ==========================================
        LARGE_INTEGER inicioBLAleat, fimBLAleat;
        QueryPerformanceCounter(&inicioAleat);
        memcpy(itens, itensOriginal, m * sizeof(Item));
        lucroAleatorio = resolverAleatorio(itens, m, W_max, V_max, &pesoAleat, &volumeAleat);
        QueryPerformanceCounter(&fimAleat);
        tempoAleat = (double)(fimAleat.QuadPart - inicioAleat.QuadPart) / frequencia.QuadPart;

        QueryPerformanceCounter(&inicioBLAleat);
        int lucroBLAleat = aplicarBuscaLocal(itens, m, W_max, V_max, pesoAleat, volumeAleat, lucroAleatorio);
        QueryPerformanceCounter(&fimBLAleat);
        double tempoBLAleat = (double)(fimBLAleat.QuadPart - inicioBLAleat.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Execução do GRASP
        // ==========================================
        int pesoGRASP = 0, volumeGRASP = 0;
        float alphaGRASP = 0.20f;
        int iteracoesGRASP = 30;
        LARGE_INTEGER inicioGRASP, fimGRASP;

        memcpy(itens, itensOriginal, m * sizeof(Item));
        QueryPerformanceCounter(&inicioGRASP);
        int lucroGRASP = resolverGRASP(
            itens, m, W_max, V_max,
            alphaGRASP, iteracoesGRASP,
            &pesoGRASP, &volumeGRASP);
        QueryPerformanceCounter(&fimGRASP);
        double tempoGRASP = (double)(fimGRASP.QuadPart - inicioGRASP.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Imprimindo com a sua funcao
        // ==========================================
        resultados(id, m, W_max, V_max,
                   lucroGuloso, tempoGuloso,
                   lucroBL, tempoBL,
                   lucroBLAleat, tempoAleat, tempoBLAleat,
                   lucroGRASP, tempoGRASP,
                   lucroGulosoSimples, tempoGulosoSimples,
                   lucroOtimo, tempoDP);

        free(itens); // Libera memoria para a proxima instancia
        free(itensOriginal);
    }

    printf("\n=== TODOS OS TESTES FORAM CONCLUIDOS ===\n");
    return 0;
}
