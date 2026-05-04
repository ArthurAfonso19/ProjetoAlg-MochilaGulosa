#ifndef GERADOR_H
#define GERADOR_H
#include "item.h"

Item* gerarItensAleatorios(int m, int W_max, int V_max);

// Agora a função de salvar recebe o lucro_otimo como último parâmetro
void salvarInstanciaCompleta(const char* nomeArquivo, Item* itens, int m, int W_max, int V_max, int lucro_otimo);

#endif