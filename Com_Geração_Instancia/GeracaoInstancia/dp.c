#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dp.h"

typedef struct Estado{
    int w; 
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int resolverDP(Item itens[], int num_itens, int W_max, int V_max) {
    
    int (*dp)[V_max + 1] = calloc(W_max + 1, sizeof(*dp));
    
    if (dp == NULL) {
        printf("Erro: Faltou memória RAM para a matriz DP!\n");
        return -1; 
    }

    for(int i = 0; i < num_itens; i++) {

        int p = itens[i].peso;
        int v_i = itens[i].volume;
        int l = itens[i].lucro;

        for(int w = W_max; w >= p; w--) {
            for (int v = V_max; v >= v_i; v--){
                dp[w][v] = max(dp[w][v], dp[w - p][v - v_i] + l);
            }
        }
        printf(" %d itens calculados!\n", i);
    }

    int resultado = dp[W_max][V_max]; 
    free(dp);
    return resultado; 
}
