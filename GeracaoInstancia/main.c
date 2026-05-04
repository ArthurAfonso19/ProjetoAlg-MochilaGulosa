#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gerador.h"
#include "dp.h"

typedef struct{
    int qtd;
    int W;
    int V; 
}Config;

int main() {
    srand(time(NULL));

    // Definição dos 8 casos de teste
    Config testes[8] = {
        // --- Capacidades IGUAIS (W == V) ---
        {50,    1000,  1000},  // 1. Pequeno
        {250,   8000,  8000},  // 2. Médio
        {500,   15000, 15000}, // 3. Grande
        {1000,  23000, 23000}, // 4. Máximo (Novo limite de memória)

        // --- Capacidades DISTINTAS (W != V) ---
        {50,    800,   1500},  // 5. Pequeno (Assimetria leve)
        {250,   12000, 4000},  // 6. Médio (Foco em Peso)
        {500,   7000,  18000}, // 7. Grande (Foco em Volume)
        {1000,  23000, 10000}  // 8. Máximo (Limite em W, Volume mais folgado)
    };

    printf("Iniciando bateria de 8 testes automatizados...\n\n");

    for (int i = 0; i < 8; i++) {
        char nome_arq[20];
        sprintf(nome_arq, "instancia%d.txt", i + 1);

        printf("[%d/8] Gerando %s (Itens: %d | W: %d | V: %d)...\n", 
                i+1, nome_arq, testes[i].qtd, testes[i].W, testes[i].V);

        Item* itens = gerarItensAleatorios(testes[i].qtd, testes[i].W, testes[i].V);

        printf("      Resolvendo DP... (Aguarde, alocando memoria)\n");
        int lucro = resolverDP(itens, testes[i].qtd, testes[i].W, testes[i].V);

        if (lucro == -1) {
            printf("      ERRO: Memoria insuficiente para este teste!\n");
        } else {
            salvarInstanciaCompleta(nome_arq, itens, testes[i].qtd, testes[i].W, testes[i].V, lucro);
            printf("      Sucesso! Lucro: %d\n", lucro);
        }

        free(itens);
        printf("----------------------------------------------------\n");
    }

    printf("Bateria de testes concluida!\n");
    return 0;
}