# Passo 5 - Medir o Tempo do GRASP no main.c

## Objetivo

Adicionar ao fluxo principal de `Comparacao_Oficial/main.c` a execucao do GRASP com medicao de tempo em alta precisao, da mesma forma que ja e feito com os outros metodos.

## Arquivo principal

- `Comparacao_Oficial/main.c`

## Estado atual

Hoje o `main.c` mede DP, gulosos e aleatorio+BL, mas ainda nao chama o GRASP.

```c
resultados(id, m, W_max, V_max,
           lucroGulosoSimples, tempoGulosoSimples,
           lucroGuloso, tempoGuloso,
           lucroBL, tempoBL,
           lucroBLAleat, tempoAleat, tempoBLAleat,
           lucroDP, tempoDP);
```

## Como deve ficar

Adicionar uma nova fase antes da chamada de `resultados(...)`.

```c
// ==========================================
// GRASP
// ==========================================
int pesoGRASP = 0, volumeGRASP = 0;
float alphaGRASP = 0.20f;
int iteracoesGRASP = 30;

memcpy(itens, itensOriginal, m * sizeof(Item));
LARGE_INTEGER inicioGRASP, fimGRASP;
QueryPerformanceCounter(&inicioGRASP);
int lucroGRASP = resolverGRASP(
    itens, m, W_max, V_max,
    alphaGRASP, iteracoesGRASP,
    &pesoGRASP, &volumeGRASP);
QueryPerformanceCounter(&fimGRASP);

double tempoGRASP =
    (double)(fimGRASP.QuadPart - inicioGRASP.QuadPart) / frequencia.QuadPart;
```

## Chamada de resultados apos a alteracao

```c
resultados(id, m, W_max, V_max,
           lucroGulosoSimples, tempoGulosoSimples,
           lucroGuloso, tempoGuloso,
           lucroBL, tempoBL,
           lucroBLAleat, tempoAleat, tempoBLAleat,
           lucroGRASP, tempoGRASP,
           lucroDP, tempoDP);
```

## Comentarios de implementacao

- O tempo do GRASP deve ser medido como tempo total do metodo.
- Nessa primeira versao, nao vale a pena separar tempo de construcao e de busca local na tabela principal.
- O `memcpy` e importante para que o GRASP trabalhe sobre uma copia limpa da instancia.

## Resultado esperado deste passo

Ao final deste passo, o fluxo principal do simulador passa a executar tambem o GRASP e a medir seu custo computacional.
