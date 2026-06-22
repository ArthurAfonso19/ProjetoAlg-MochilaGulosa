# Passo 8 - Ajustar Parametros do GRASP

## Objetivo

Organizar a calibracao inicial dos parametros do GRASP, principalmente `alpha` e numero de iteracoes.

## Arquivo principal

- `Comparacao_Oficial/main.c`

## Estado inicial esperado apos o passo 5

No primeiro momento, os parametros podem estar escritos diretamente no bloco de execucao do GRASP.

```c
int pesoGRASP = 0, volumeGRASP = 0;
float alphaGRASP = 0.20f;
int iteracoesGRASP = 30;

int lucroGRASP = resolverGRASP(
    itens, m, W_max, V_max,
    alphaGRASP, iteracoesGRASP,
    &pesoGRASP, &volumeGRASP);
```

## Como pode ficar melhor organizado

Promover esses valores a constantes nomeadas no topo do arquivo.

```c
#define GRASP_ALPHA_INICIAL 0.20f
#define GRASP_ITERACOES_INICIAIS 30
```

E depois usar no bloco principal:

```c
int pesoGRASP = 0, volumeGRASP = 0;

int lucroGRASP = resolverGRASP(
    itens, m, W_max, V_max,
    GRASP_ALPHA_INICIAL,
    GRASP_ITERACOES_INICIAIS,
    &pesoGRASP, &volumeGRASP);
```

## Comentarios sobre calibracao

### alpha

- `0.10f`: mais guloso, menos diversidade
- `0.20f`: valor inicial recomendado
- `0.30f`: mais variedade sem perder muito criterio
- `0.50f` ou mais: comportamento bem mais aleatorio

### iteracoes

- `10`: rapido, mas ainda pouco estavel
- `30`: bom ponto inicial
- `50`: melhor chance de ganho, com mais custo
- `100`: pode ser util em estudo final, mas precisa avaliar tempo

## Estrategia pratica de ajuste

Rodar as 8 instancias oficiais em pequenos blocos de teste:

1. `alpha = 0.20`, `iteracoes = 30`
2. `alpha = 0.30`, `iteracoes = 30`
3. `alpha = 0.20`, `iteracoes = 50`
4. `alpha = 0.30`, `iteracoes = 50`

Comparar:

- lucro medio
- percentual medio do otimo
- tempo medio por instancia
- estabilidade entre execucoes

## Antes e depois

### Antes

Parametros escritos diretamente no bloco do GRASP.

### Depois

Parametros promovidos para constantes, facilitando ajuste e repeticao de experimentos.

## Resultado esperado deste passo

Ao final deste passo, o GRASP deixa de ser apenas uma implementacao funcional e passa a ter parametros minimamente calibrados para uso nos experimentos do trabalho.
