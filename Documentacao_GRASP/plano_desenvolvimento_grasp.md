# Plano de Desenvolvimento da Metaheuristica GRASP

## Objetivo

Adicionar a metaheuristica `GRASP` (Greedy Randomized Adaptive Search Procedure) ao projeto, mantendo intactos os metodos ja implementados e ampliando o comparativo atual com mais uma estrategia de solucao.

Neste projeto, a ideia natural e usar:

- uma fase construtiva gulosa randomizada
- seguida por uma fase de refinamento com a `Busca Local` ja existente

O primeiro alvo de integracao deve ser a pasta `Comparacao_Oficial/`, pois ela ja trabalha com as instancias oficiais e calcula o otimo por `DP`, o que facilita medir a qualidade do GRASP desde o inicio.

## Ideia Geral de Implementacao

O GRASP sera implementado como um processo iterativo composto por duas etapas:

1. Construcao de uma solucao gulosa randomizada.
2. Aplicacao de busca local sobre a solucao construida.

Em cada iteracao:

- calcula-se um criterio guloso para os itens candidatos
- monta-se uma `RCL` (Restricted Candidate List)
- escolhe-se aleatoriamente um item da `RCL`
- repete-se a construcao enquanto houver insercoes viaveis
- aplica-se `Busca Local` na solucao resultante
- guarda-se a melhor solucao entre todas as iteracoes

Ao final, o GRASP retorna a melhor solucao encontrada e seu tempo total de execucao.

## Estrutura Proposta no Projeto

### 1. Pasta inicial de integracao

Implementar primeiro em `Comparacao_Oficial/`.

Motivos:

- ja existe leitura das instancias oficiais
- o `DP` fornece baseline confiavel
- os metodos atuais ja estao organizados para comparacao direta
- permite validar a qualidade do GRASP antes de replica-lo em outros modulos

### 2. Arquivos que devem ser alterados depois

- `Comparacao_Oficial/algoritmos.h`
- `Comparacao_Oficial/algoritmos.c`
- `Comparacao_Oficial/utils.h`
- `Comparacao_Oficial/utils.c`
- `Comparacao_Oficial/main.c`

## Modelo Conceitual do GRASP

### Fase 1: Construcao Gulosa Randomizada

A construcao deve reaproveitar a ideia dos algoritmos gulosos ja existentes.

Cada item recebe um `score` heuristico. A partir disso:

- filtrar apenas os itens ainda nao selecionados e viaveis
- identificar os melhores candidatos segundo o score
- formar uma lista restrita `RCL`
- sortear um item da `RCL`
- inserir esse item na solucao

Esse processo continua ate que nenhum novo item possa ser inserido sem violar capacidade.

### Fase 2: Busca Local

Depois da construcao, aplicar a funcao ja existente `aplicarBuscaLocal(...)`.

Vantagens:

- reduz retrabalho
- reaproveita uma heuristica ja validada no projeto
- mantem coerencia com a proposta da disciplina

### Fase 3: Multi-start

O GRASP nao executa apenas uma vez. Ele repete varias iteracoes independentes.

Em cada iteracao:

- constroi uma solucao diferente por causa da aleatoriedade
- melhora essa solucao com busca local
- compara com a melhor solucao global ja encontrada

Ao final, mantem a melhor.

## Decisoes de Projeto Recomendadas

### 1. Score da construcao

Usar inicialmente o mesmo score do `Guloso Otimizado`:

`lucro / ((1 / W_max) * peso + (1 / V_max) * volume)`

Motivo:

- ja existe no projeto
- e consistente com o metodo guloso refinado
- facilita comparacao entre `Guloso Otimizado`, `Guloso + BL` e `GRASP`

### 2. Formacao da RCL

Usar abordagem por `alpha`.

Regra sugerida:

- calcular `score_max` e `score_min` entre candidatos viaveis
- incluir na `RCL` os itens com score >= `score_max - alpha * (score_max - score_min)`

Valor inicial recomendado:

- `alpha = 0.20` ou `alpha = 0.30`

Interpretacao:

- `alpha = 0`: comportamento quase totalmente guloso
- `alpha = 1`: comportamento bem mais aleatorio

### 3. Numero de iteracoes

Comecar com numero fixo de iteracoes por instancia.

Valor inicial recomendado:

- `30` iteracoes para testes iniciais

Depois, se necessario:

- aumentar para `50` ou `100`
- ou testar criterio por tempo limite

### 4. Saida do metodo

O GRASP deve devolver pelo menos:

- lucro final da melhor solucao
- peso final
- volume final
- configuracao dos itens selecionados da melhor iteracao

Isso e importante para permitir:

- uso correto da busca local
- impressao de resultados
- comparacao futura com outras metaheuristicas

## Passos de Desenvolvimento

### Passo 1. Definir a interface publica do GRASP

Adicionar em `algoritmos.h` uma funcao com assinatura semelhante ao estilo atual do projeto.

Exemplo de intencao:

- receber vetor de itens, quantidade, capacidades e ponteiros para peso e volume finais
- receber tambem parametros do GRASP, como `alpha` e numero de iteracoes
- retornar o lucro da melhor solucao encontrada

Objetivo deste passo:

- encaixar o GRASP na arquitetura existente sem quebrar os demais metodos

### Passo 2. Implementar a construcao gulosa randomizada

Criar em `algoritmos.c` uma rotina interna responsavel por:

- limpar flags `selecionado`
- calcular score dos itens
- encontrar candidatos viaveis
- formar a `RCL`
- escolher item aleatorio da `RCL`
- atualizar peso, volume e lucro da solucao parcial

Objetivo deste passo:

- produzir uma solucao inicial viavel e diversificada

### Passo 3. Integrar a busca local na iteracao do GRASP

Depois da construcao:

- chamar `aplicarBuscaLocal(...)`
- comparar a solucao refinada com a melhor solucao global do GRASP

Objetivo deste passo:

- transformar a construcao randomizada em uma metaheuristica completa

### Passo 4. Preservar a melhor solucao entre iteracoes

Como a busca local altera o vetor `itens`, sera necessario:

- manter uma copia da melhor configuracao encontrada
- restaurar essa configuracao ao final do processo, se for necessario manter o estado do vetor coerente

Objetivo deste passo:

- garantir que o resultado final retornado pelo GRASP corresponda realmente a melhor iteracao

### Passo 5. Medir o tempo do GRASP no `main.c`

No comparativo principal:

- medir o tempo total do GRASP
- decidir se vale separar tempo de construcao e tempo de busca local

Recomendacao inicial:

- imprimir o tempo total do GRASP como um unico metodo

Objetivo deste passo:

- manter a tabela simples e comparavel com os metodos atuais

### Passo 6. Atualizar a impressao dos resultados

Alterar `resultados(...)` para incluir:

- uma nova linha `GRASP`
- uma nova linha de qualidade `% do otimo`

Objetivo deste passo:

- inserir o GRASP no mesmo padrao visual dos metodos anteriores

### Passo 7. Validar corretude basica

Validar se o GRASP:

- sempre retorna solucao viavel
- nunca excede peso e volume maximos
- melhora ou pelo menos compete com `Guloso + BL` em parte das instancias

Validacoes recomendadas:

- testar primeiro nas instancias menores
- conferir peso, volume e lucro manualmente em alguns casos
- comparar com `DP` para medir qualidade

### Passo 8. Ajustar parametros

Depois da primeira versao funcionando:

- testar diferentes valores de `alpha`
- testar diferentes numeros de iteracoes
- observar impacto em tempo e qualidade

Objetivo deste passo:

- encontrar configuracao razoavel para o experimento da disciplina

## Estrategia de Validacao

### Validacao funcional

Verificar:

- leitura correta das instancias
- execucao sem travamentos
- lucro do GRASP sempre consistente com os itens selecionados
- respeito das restricoes de capacidade

### Validacao comparativa

Comparar o GRASP com:

- `Guloso puro simples`
- `Guloso puro`
- `Guloso + BL`
- `Aleatorio + BL`
- `DP`

Analises desejadas:

- qualidade relativa ao otimo
- custo computacional adicional
- estabilidade dos resultados entre execucoes

### Validacao experimental

Como o GRASP tem aleatoriedade, e importante observar:

- se os resultados variam muito entre execucoes
- se compensa fixar uma semente para reproducao
- se o ganho medio justifica o aumento de tempo

## Riscos Tecnicos

### 1. Estado compartilhado dos itens

As funcoes atuais alteram:

- `score`
- `selecionado`
- ordem do vetor via `qsort`

Por isso, o GRASP deve trabalhar com cuidado sobre copias ou com restauracao de estado.

### 2. RCL mal calibrada

Se a `RCL` ficar muito pequena:

- o GRASP vira quase guloso deterministico

Se ficar muito grande:

- perde qualidade construtiva

### 3. Tempo excessivo

Se o numero de iteracoes crescer demais:

- o metodo pode ficar caro para pouca melhoria

### 4. Busca local fraca

A busca local atual faz troca `1 por 1`.

Isso pode limitar o ganho do GRASP, mesmo quando a construcao randomizada for boa. Ainda assim, para a primeira versao, ela e suficiente e coerente com o projeto atual.

## Ordem Recomendada de Implementacao

1. Implementar o GRASP em `Comparacao_Oficial/`.
2. Validar corretude e qualidade nas 8 instancias oficiais.
3. Ajustar `alpha` e numero de iteracoes.
4. Atualizar a tabela de resultados.
5. So depois considerar portar o GRASP para `Comparacao_Heuristicas/`, se isso ainda fizer sentido para o seu fluxo experimental.

## Resultado Esperado

Ao final desse desenvolvimento, o projeto devera possuir uma nova linha no comparativo:

- `GRASP`

Essa nova metaheuristica devera:

- usar construcao gulosa randomizada
- reaproveitar a busca local ja existente
- produzir resultados comparaveis com os demais metodos
- permitir medir qualidade versus custo computacional

## Proximo Passo Sugerido

Depois deste plano, o proximo passo pratico e implementar uma primeira versao minima do GRASP em `Comparacao_Oficial/`, com:

- score do guloso otimizado
- `alpha = 0.20`
- `30` iteracoes por instancia
- busca local ao fim de cada iteracao

Essa versao ja sera suficiente para testar comportamento, medir tempo e comparar com os metodos atuais.
