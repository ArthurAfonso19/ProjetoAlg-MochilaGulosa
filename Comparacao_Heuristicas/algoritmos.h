#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <time.h>
#include "item.h"

float calcularScore(Item item, int maxPeso, int maxVolume);
int resolverAleatorio(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int resolverGulosa(Item itens[], int n, int maxPeso, int maxVolume, int *pesoFinal, int *volumeFinal);
int aplicarBuscaLocal(Item itens[], int n, int maxPeso, int maxVolume, int pesoInicial, int volumeInicial, int lucroInicial);

#endif