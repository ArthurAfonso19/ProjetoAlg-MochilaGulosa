#include <stdio.h>
#include <stdlib.h>
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

Item *lerInstancia(const char *nomeArquivo, int *m, int *W_max, int *V_max, int *lucroOtimo, double *tempoDP)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao ler o arquivo %s!\n", nomeArquivo);
        return NULL;
    }

    fscanf(arquivo, "%d %d %d", m, W_max, V_max);

    Item *itens = (Item *)malloc((*m) * sizeof(Item));
    for (int i = 0; i < *m; i++)
    {
        fscanf(arquivo, "%d %d %d %d", &itens[i].id, &itens[i].peso, &itens[i].volume, &itens[i].lucro);
        itens[i].selecionado = 0;
        itens[i].score = 0;
    }

    // Lê o último valor do arquivo (Lucro Ótimo da DP)
    if (fscanf(arquivo, "%d", lucroOtimo) != 1)
    {
        *lucroOtimo = 0;
    }

    if (fscanf(arquivo, "%lf", tempoDP) != 1)
    {
        *tempoDP = 0.0;
    }

    fclose(arquivo);
    return itens;
}

void resultados(int id_instancia, int m, int W_max, int V_max,
                int lucroGuloso, double tempoGuloso,
                int lucroBL, double tempoBL,
                int lucroBLAleat, double tempoAleat, double tempoBLAleat,
                int lucroGRASP, double tempoGRASP,
                int lucroGulosoSimples, double tempoGulosoSimples,
                int lucroOtimo, double tempoDP)
{
    // Para metodos compostos, o tempo total soma construcao e refinamento.
    double tempoTotal_GulosoBL = tempoGuloso + tempoBL;
    double tempoTotal_AleatBL = tempoAleat + tempoBLAleat;

    // Speedup > 1 indica que o metodo foi mais rapido do que a DP.
    double speedupGulosoSimples = (tempoGulosoSimples > 0.0) ? (tempoDP / tempoGulosoSimples) : 0.0;
    double speedupGuloso = (tempoGuloso > 0.0) ? (tempoDP / tempoGuloso) : 0.0;
    double speedupBL = (tempoTotal_GulosoBL > 0.0) ? (tempoDP / tempoTotal_GulosoBL) : 0.0;
    double speedupBLAleat = (tempoTotal_AleatBL > 0.0) ? (tempoDP / tempoTotal_AleatBL) : 0.0;
    double speedupGRASP = (tempoGRASP > 0.0) ? (tempoDP / tempoGRASP) : 0.0;

    printf("\n=================================================================\n");
    printf("INSTANCIA %d (Itens: %d | W: %d | V: %d | OTIMO: %d)\n", id_instancia, m, W_max, V_max, lucroOtimo);
    printf("=================================================================\n");
    printf("%-22s | %-10s | %-15s\n", "METODO (ESTRATEGIA)", "LUCRO", "TEMPO TOTAL (s)");
    printf("-----------------------------------------------------------------\n");
    printf("%-22s | %-10d | %-15.6f\n", "Prog Dinamica", lucroOtimo, tempoDP);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso puro simples", lucroGulosoSimples, tempoGulosoSimples);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso puro", lucroGuloso, tempoGuloso);
    printf("%-22s | %-10d | %-15.6f\n", "Guloso + BL", lucroBL, tempoTotal_GulosoBL);
    printf("%-22s | %-10d | %-15.6f\n", "Aleatorio + BL", lucroBLAleat, tempoTotal_AleatBL);
    printf("%-22s | %-10d | %-15.6f\n", "GRASP", lucroGRASP, tempoGRASP);
    printf("-----------------------------------------------------------------\n");

    // Gap percentual em relacao ao lucro otimo salvo na instancia gerada.
    float gapGuloso = ((float)(lucroOtimo - lucroGuloso) / lucroOtimo) * 100;
    float gapBL = ((float)(lucroOtimo - lucroBL) / lucroOtimo) * 100;
    float gapBLAleat = ((float)(lucroOtimo - lucroBLAleat) / lucroOtimo) * 100;
    float gapGulosoSimples = ((float)(lucroOtimo - lucroGulosoSimples) / lucroOtimo) * 100;
    float gapGRASP = ((float)(lucroOtimo - lucroGRASP) / lucroOtimo) * 100;

    printf("QUALIDADE (%% DO OTIMO):\n");
    printf("- Guloso simples   : %6.2f%%\n", 100.0 - gapGulosoSimples);
    printf("- Guloso otimizado : %6.2f%%\n", 100.0 - gapGuloso);
    printf("- Guloso + BL      : %6.2f%%\n", 100.0 - gapBL);
    printf("- Aleatorio + BL   : %6.2f%%\n", 100.0 - gapBLAleat);
    printf("- GRASP            : %6.2f%%\n", 100.0 - gapGRASP);
    printf("SPEEDUP (BASELINE = DP):\n");
    printf("- Guloso simples   : %8.2fx\n", speedupGulosoSimples);
    printf("- Guloso otimizado : %8.2fx\n", speedupGuloso);
    printf("- Guloso + BL      : %8.2fx\n", speedupBL);
    printf("- Aleatorio + BL   : %8.2fx\n", speedupBLAleat);
    printf("- GRASP            : %8.2fx\n", speedupGRASP);
    printf("=================================================================\n");
}
