#include <stdio.h>
#include <stdlib.h>
#include "gerador.h"

Item* gerarItensAleatorios(int m, int W_max, int V_max) {
    Item *itens = (Item*) malloc(m * sizeof(Item)); 
    if(itens == NULL) {
        printf("Erro de alocação de memória!\n"); 
        exit(1);
    }

    for(int i = 0; i < m; i++) {
        itens[i].id = i + 1; 
        itens[i].peso = (rand() % (W_max/10)) + 1;
        itens[i].volume = (rand() % (V_max/5)) + 1;

        //Cria um tamanho base (uma média de impacto na mochila)
        int tamanho_base = (itens[i].peso + itens[i].volume);

        //Cria um ruído aleatório (variação de -10% até 20% do valor)
        int variacao_maxima = tamanho_base * 0.3; 
        int ruido = (rand() % (variacao_maxima + 1)) - (variacao_maxima / 3);
        // Lucro proporcional ao tamanho + uma variação de até 200
        itens[i].lucro = tamanho_base + ruido;
        
        if(itens[i].lucro <= 0)
        {
            itens[i].lucro = 1;
        }

        itens[i].score = 0; 
        itens[i].selecionado = 0;
    }

    return itens; 
}

void salvarInstanciaCompleta(const char* nomeArquivo, Item* itens, int m, int W_max, int V_max, int lucro_otimo) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo %s!\n", nomeArquivo);
        exit(1);
    }

    // Cabecalho
    fprintf(arquivo, "%d %d %d\n", m, W_max, V_max);

    // Lista de Itens
    for (int i = 0; i < m; i++) {
        fprintf(arquivo, "%d %d %d %d\n", itens[i].id, itens[i].peso, itens[i].volume, itens[i].lucro);
    }

    // Pula uma linha e escreve o resultado ótimo da DP no final
    fprintf(arquivo, "\n%d\n", lucro_otimo);

    fclose(arquivo);
}