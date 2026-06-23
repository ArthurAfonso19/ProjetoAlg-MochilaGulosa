#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int max(int a, int b)
{
    return (a > b) ? a : b;
}

void embaralhar(int *vetor, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = vetor[i];
        vetor[i] = vetor[j];
        vetor[j] = temp;
    }
}

Item *lerInstancia(const char *nomeArquivo, int *m, int *W_max, int *V_max)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao ler o arquivo %s!\n", nomeArquivo);
        return NULL;
    }

    int dim;
    if (fscanf(arquivo, "%d %d", &dim, m) != 2)
    {
        printf("Erro ao ler cabecalho de %s!\n", nomeArquivo);
        fclose(arquivo);
        return NULL;
    }

    int *lucros = (int *)malloc((*m) * sizeof(int));
    if (lucros == NULL)
    {
        fclose(arquivo);
        return NULL;
    }
    for (int i = 0; i < *m; i++)
    {
        if (fscanf(arquivo, "%d", &lucros[i]) != 1)
        {
            printf("Erro ao ler lucros de %s!\n", nomeArquivo);
            free(lucros);
            fclose(arquivo);
            return NULL;
        }
    }

    if (fscanf(arquivo, "%d %d", W_max, V_max) != 2)
    {
        printf("Erro ao ler capacidades de %s!\n", nomeArquivo);
        free(lucros);
        fclose(arquivo);
        return NULL;
    }

    Item *itens = (Item *)malloc((*m) * sizeof(Item));
    if (itens == NULL)
    {
        free(lucros);
        fclose(arquivo);
        return NULL;
    }
    for (int i = 0; i < *m; i++)
    {
        int p, v;
        if (fscanf(arquivo, "%d %d", &p, &v) != 2)
        {
            printf("Erro ao ler tuplas de %s!\n", nomeArquivo);
            free(lucros);
            free(itens);
            fclose(arquivo);
            return NULL;
        }
        itens[i].id = i + 1;
        itens[i].peso = p;
        itens[i].volume = v;
        itens[i].lucro = lucros[i];
        itens[i].selecionado = 0;
        itens[i].score = 0;
    }

    free(lucros);
    fclose(arquivo);
    return itens;
}

void resultados(int id_instancia, int m, int W_max, int V_max,
                int lucroGulosoSimples, double tempoGulosoSimples,
                int lucroGuloso, double tempoGuloso,
                int lucroBL, double tempoBL,
                int lucroBLAleat, double tempoAleat, double tempoBLAleat,
                int lucroGRASP, double tempoGRASP,
                int lucroDP, double tempoDP)
{
    double tempoTotal_GulosoBL = tempoGuloso + tempoBL;
    double tempoTotal_AleatBL = tempoAleat + tempoBLAleat;

    printf("\n=================================================================\n");
    printf("INSTANCIA %d (Itens: %d | W: %d | V: %d | OTIMO(DP): %d)\n", id_instancia, m, W_max, V_max, lucroDP);
    printf("=================================================================\n");
    printf("%-22s | %-10s | %-15s\n", "METODO (ESTRATEGIA)", "LUCRO", "TEMPO TOTAL (s)");
    printf("-----------------------------------------------------------------\n");
    printf("%-22s | %-10d | %-15.6f\n", "Prog Dinamica", lucroDP, tempoDP);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso puro simples", lucroGulosoSimples, tempoGulosoSimples);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso puro", lucroGuloso, tempoGuloso);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso + BL", lucroBL, tempoTotal_GulosoBL);
    printf("%-22s | %-10d | %-15.6f\n", "Aleatorio + BL", lucroBLAleat, tempoTotal_AleatBL);
    printf("%-22s | %-10d | %-15.6f\n", "GRASP", lucroGRASP, tempoGRASP);
    printf("-----------------------------------------------------------------\n");

    float gapGulosoSimples = ((float)(lucroDP - lucroGulosoSimples) / lucroDP) * 100;
    float gapGuloso = ((float)(lucroDP - lucroGuloso) / lucroDP) * 100;
    float gapBL = ((float)(lucroDP - lucroBL) / lucroDP) * 100;
    float gapBLAleat = ((float)(lucroDP - lucroBLAleat) / lucroDP) * 100;
    float gapGRASP = ((float)(lucroDP - lucroGRASP) / lucroDP) * 100;

    printf("QUALIDADE (%% DO OTIMO DP):\n");
    printf("- Guloso simples   : %6.2f%%\n", 100.0 - gapGulosoSimples);
    printf("- Guloso otimizado : %6.2f%%\n", 100.0 - gapGuloso);
    printf("- Guloso + BL      : %6.2f%%\n", 100.0 - gapBL);
    printf("- Aleatorio + BL   : %6.2f%%\n", 100.0 - gapBLAleat);
    printf("- GRASP            : %6.2f%%\n", 100.0 - gapGRASP);
    printf("=================================================================\n");
}