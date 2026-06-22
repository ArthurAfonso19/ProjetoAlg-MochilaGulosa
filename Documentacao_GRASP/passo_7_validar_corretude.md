# Passo 7 - Validar Corretude Basica do GRASP

## Objetivo

Verificar se o GRASP gera solucoes validas e se os valores retornados sao coerentes com os itens efetivamente selecionados.

## Natureza deste passo

Este passo pode ser feito sem alterar permanentemente o projeto, mas e util documentar pequenos trechos auxiliares de validacao.

## Arquivo mais provavel para apoio

- `Comparacao_Oficial/main.c`

## Validacao minima recomendada

1. Conferir se o peso final nao excede `W_max`.
2. Conferir se o volume final nao excede `V_max`.
3. Conferir se o lucro retornado bate com os itens marcados em `selecionado`.

## Trecho opcional de apoio

Adicionar helpers temporarios em `main.c` durante a fase de teste.

```c
static int calcularLucroSelecionado(Item itens[], int n)
{
    int lucro = 0;

    for (int i = 0; i < n; i++)
    {
        if (itens[i].selecionado)
        {
            lucro += itens[i].lucro;
        }
    }

    return lucro;
}

static void validarSolucao(const char *nomeMetodo, Item itens[], int n,
                           int pesoFinal, int volumeFinal, int lucroFinal,
                           int maxPeso, int maxVolume)
{
    int lucroRecalculado = calcularLucroSelecionado(itens, n);

    if (pesoFinal > maxPeso || volumeFinal > maxVolume)
    {
        printf("ERRO: %s excedeu capacidade da mochila.\n", nomeMetodo);
    }

    if (lucroRecalculado != lucroFinal)
    {
        printf("ERRO: %s retornou lucro inconsistente.\n", nomeMetodo);
    }
}
```

## Exemplo de uso durante os testes

```c
validarSolucao("GRASP", itens, m, pesoGRASP, volumeGRASP, lucroGRASP, W_max, V_max);
```

## Antes e depois

### Antes

Nao existe nenhuma validacao explicita do retorno do GRASP, porque o metodo ainda nao foi integrado.

### Depois

Durante a fase de desenvolvimento, o `main.c` pode conter chamadas temporarias de validacao como a acima.

## Comentarios de uso

- Essas funcoes podem ser removidas depois da validacao.
- Se voce preferir, elas tambem podem ficar comentadas no codigo como apoio para futuras depuracoes.
- O mais importante neste passo e provar que a solucao devolvida pelo GRASP e viavel e coerente.

## Resultado esperado deste passo

Ao final deste passo, voce tera seguranca de que o GRASP nao esta apenas gerando um bom lucro, mas esta respeitando corretamente as restricoes do problema.
