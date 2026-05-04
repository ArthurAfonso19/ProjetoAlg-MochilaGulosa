#include <stdio.h>
#include <stdlib.h>
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

    for (int id = 1; id <= 8; id++) {
        int W_max;
        int V_max; 
        int m; 
        int lucroOtimo;
        char nome_arquivo[30];
        
        sprintf(nome_arquivo, "instancia%d.txt", id);

        Item *itens = lerInstancia(nome_arquivo, &m, &W_max, &V_max, &lucroOtimo);
        if (itens == NULL) {
            printf("Aviso: Instancia %d nao encontrada. Pulando...\n", id);
            continue; 
        }

        // ==========================================
        // Execução da Heuristica Gulosa
        // ==========================================
        int pesoGuloso = 0; 
        int volumeGuloso = 0; 
        int lucroGuloso = 0; 
        double tempoGuloso = 0; 
        
        LARGE_INTEGER inicioGuloso, fimGuloso;
        QueryPerformanceCounter(&inicioGuloso);
        
        lucroGuloso = resolverGulosa(itens, m, W_max, V_max, &pesoGuloso, &volumeGuloso);
        
        QueryPerformanceCounter(&fimGuloso);
        tempoGuloso = (double)(fimGuloso.QuadPart - inicioGuloso.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Execução da Busca Local a partir do guloso
        // ==========================================
        LARGE_INTEGER inicioBL, fimBL;
        QueryPerformanceCounter(&inicioBL); 
        
        int lucroBL = aplicarBuscaLocal(itens, m, W_max, V_max, pesoGuloso, volumeGuloso, lucroGuloso);
        
        QueryPerformanceCounter(&fimBL); 
        double tempoBL = (double)(fimBL.QuadPart - inicioBL.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Execução da Construção Aleatória
        // ==========================================
        int pesoAleat = 0, volumeAleat = 0;
        LARGE_INTEGER inicioAleat, fimAleat;
        
        QueryPerformanceCounter(&inicioAleat);
        int lucroAleatorio = resolverAleatorio(itens, m, W_max, V_max, &pesoAleat, &volumeAleat);
        QueryPerformanceCounter(&fimAleat);
        
        double tempoAleat = (double)(fimAleat.QuadPart - inicioAleat.QuadPart) / frequencia.QuadPart;

        // ==========================================
        // Busca Local iniciada a partir da Solução Aleatória 
        // ==========================================
        LARGE_INTEGER inicioBLAleat, fimBLAleat;
        QueryPerformanceCounter(&inicioBLAleat); 
        
        int lucroBLAleat = aplicarBuscaLocal(itens, m, W_max, V_max, pesoAleat, volumeAleat, lucroAleatorio);
        
        QueryPerformanceCounter(&fimBLAleat); 
        double tempoBLAleat = (double)(fimBLAleat.QuadPart - inicioBLAleat.QuadPart) / frequencia.QuadPart; 

        // ==========================================
        // Imprimindo com a sua funcao
        // ==========================================
        resultados(id, m, W_max, V_max,
                   lucroGuloso, tempoGuloso, 
                   lucroBL, tempoBL, 
                   lucroBLAleat, tempoAleat, tempoBLAleat, 
                   lucroOtimo);

        free(itens); // Libera memoria para a proxima instancia
    }

    printf("\n=== TODOS OS TESTES FORAM CONCLUIDOS ===\n");
    return 0;
}