# Passo 6 - Atualizar a Impressao dos Resultados

## Objetivo

Incluir o GRASP na tabela de resultados e na secao de qualidade percentual em `Comparacao_Oficial/utils.h` e `Comparacao_Oficial/utils.c`.

## Arquivos principais

- `Comparacao_Oficial/utils.h`
- `Comparacao_Oficial/utils.c`

## Estado atual em utils.h

```c
void resultados(int id_instancia, int m, int W_max, int V_max,
               int lucroGulosoSimples, double tempoGulosoSimples,
               int lucroGuloso, double tempoGuloso,
               int lucroBL, double tempoBL,
               int lucroBLAleat, double tempoAleat, double tempoBLAleat,
               int lucroDP, double tempoDP);
```

## Como deve ficar em utils.h

```c
void resultados(int id_instancia, int m, int W_max, int V_max,
               int lucroGulosoSimples, double tempoGulosoSimples,
               int lucroGuloso, double tempoGuloso,
               int lucroBL, double tempoBL,
               int lucroBLAleat, double tempoAleat, double tempoBLAleat,
               int lucroGRASP, double tempoGRASP,
               int lucroDP, double tempoDP);
```

## Estado atual em utils.c

```c
printf("%-22s | %-10d | %-15.6f\n", "Prog Dinamica", lucroDP, tempoDP);
printf("%-22s | %-10d | %-15.6f\n", "Guloso puro simples", lucroGulosoSimples, tempoGulosoSimples);
printf("%-22s | %-10d | %-15.6f\n", "Guloso puro", lucroGuloso, tempoGuloso);
printf("%-22s | %-10d | %-15.6f\n", "Guloso + BL", lucroBL, tempoTotal_GulosoBL);
printf("%-22s | %-10d | %-15.6f\n", "Aleatorio + BL", lucroBLAleat, tempoTotal_AleatBL);
```

e

```c
float gapGulosoSimples = ((float)(lucroDP - lucroGulosoSimples) / lucroDP) * 100;
float gapGuloso = ((float)(lucroDP - lucroGuloso) / lucroDP) * 100;
float gapBL = ((float)(lucroDP - lucroBL) / lucroDP) * 100;
float gapBLAleat = ((float)(lucroDP - lucroBLAleat) / lucroDP) * 100;

printf("QUALIDADE (%% DO OTIMO DP):\n");
printf("- Guloso simples   : %6.2f%%\n", 100.0 - gapGulosoSimples);
printf("- Guloso otimizado : %6.2f%%\n", 100.0 - gapGuloso);
printf("- Guloso + BL      : %6.2f%%\n", 100.0 - gapBL);
printf("- Aleatorio + BL   : %6.2f%%\n", 100.0 - gapBLAleat);
```

## Como deve ficar em utils.c

```c
printf("%-22s | %-10d | %-15.6f\n", "Prog Dinamica", lucroDP, tempoDP);
printf("%-22s | %-10d | %-15.6f\n", "Guloso puro simples", lucroGulosoSimples, tempoGulosoSimples);
printf("%-22s | %-10d | %-15.6f\n", "Guloso puro", lucroGuloso, tempoGuloso);
printf("%-22s | %-10d | %-15.6f\n", "Guloso + BL", lucroBL, tempoTotal_GulosoBL);
printf("%-22s | %-10d | %-15.6f\n", "Aleatorio + BL", lucroBLAleat, tempoTotal_AleatBL);
printf("%-22s | %-10d | %-15.6f\n", "GRASP", lucroGRASP, tempoGRASP);
```

e

```c
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
```

## Comentarios de implementacao

- A linha do `GRASP` entra como mais um metodo da tabela.
- A qualidade deve usar o `lucroDP` como referencia, igual aos demais.
- A assinatura de `resultados(...)` precisa mudar em `utils.h`, `utils.c` e em todos os pontos de chamada do `main.c`.

## Resultado esperado deste passo

Ao final deste passo, o comparativo exibira o GRASP no mesmo padrao visual das demais heuristicas.
