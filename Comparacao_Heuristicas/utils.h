#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <time.h>
#include "item.h"

int max(int a, int b);
void embaralhar(int *vetor, int n);

// Nova função para ler os dados do arquivo gerado
Item* lerInstancia(const char* nomeArquivo, int *m, int *W_max, int *V_max, int *lucroOtimo);

// Sua função adaptada
void resultados(int id_instancia, int m, int W_max, int V_max,
                int lucroGuloso, double tempoGuloso, 
                int lucroBL, double tempoBL, 
                int lucroBLAleat, double tempoAleat, double tempoBLAleat, 
                int lucroGulosoSimples, double tempoGulosoSimples,
                int lucroOtimo);

#endif