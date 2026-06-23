#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#include <time.h>
#include "item.h"

int max(int a, int b);
void embaralhar(int *vetor, int n);

Item *lerInstancia(const char *nomeArquivo, int *m, int *W_max, int *V_max);

void resultados(int id_instancia, int m, int W_max, int V_max,
               int lucroGulosoSimples, double tempoGulosoSimples,
               int lucroGuloso, double tempoGuloso,
               int lucroBL, double tempoBL,
               int lucroBLAleat, double tempoAleat, double tempoBLAleat,
               int lucroGRASP, double tempoGRASP,
               int lucroDP, double tempoDP);

#endif