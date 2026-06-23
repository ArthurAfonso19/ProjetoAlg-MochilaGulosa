# Melhorias Iniciais no Gerador de Instâncias

## Objetivo

Antecipar ajustes pequenos no gerador de instâncias para tornar a base experimental mais útil para análise, sem mudar drasticamente a lógica atual e sem perder comparabilidade com o que já foi construído.

A ideia aqui não é redesenhar todo o gerador, mas corrigir alguns pontos que hoje podem enviesar demais os resultados:

- excesso de influência dos itens de ouro
- simetria incompleta entre `peso` e `volume` nas instâncias com `W = V`
- forte correlação entre `lucro` e `peso + volume`
- pouca rastreabilidade da geração

## Estratégia recomendada

Fazer mudanças em camadas, da menor para a maior:

1. primeiro mudanças de controle e rastreabilidade
2. depois ajustes leves nos parâmetros
3. por fim pequenas variações no modelo de geração

Assim você consegue medir o efeito de cada alteração sem perder o histórico experimental.

## Melhorias recomendadas agora

## Etapa 1. Tornar a geração reproduzível

### Objetivo

Permitir repetir exatamente uma instância gerada.

### Mudança

Substituir ou complementar o uso de:

```c
srand(time(NULL));
```

por uma opção com semente fixa ou semente informada.

### Implementação sugerida

- aceitar uma semente por argumento em `main`
- usar uma semente padrão quando nenhum argumento for passado
- imprimir a semente usada no terminal
- opcionalmente salvar a semente no arquivo da instância ou em log separado

### Benefício

- reprodutibilidade
- comparabilidade entre execuções
- facilidade para depuração e análise posterior

## Etapa 2. Parametrizar os itens de ouro

### Objetivo

Reduzir o risco de a solução ser dominada por poucos itens excepcionalmente bons.

### Problema atual

Hoje existe uma regra fixa de aproximadamente 5% dos itens com:

- peso muito baixo
- volume muito baixo
- lucro muito alto

Em instâncias grandes, isso pode gerar dezenas de itens extremamente vantajosos.

### Mudança

Transformar isso em parâmetro configurável.

### Implementação sugerida

- criar uma constante para o percentual de itens especiais
- testar valores como `0%`, `1%`, `3%` e `5%`
- manter inicialmente o restante da lógica igual

### Benefício

- mais controle sobre a dificuldade das instâncias
- possibilidade de comparar cenários com e sem outliers fortes

## Etapa 3. Equilibrar melhor peso e volume nas instâncias simétricas

### Objetivo

Fazer com que instâncias com `W = V` sejam realmente mais simétricas também na distribuição dos itens.

### Problema atual

Mesmo quando `W = V`, o gerador usa limites diferentes:

- `peso` até `W/10`
- `volume` até `V/5`

Isso faz o volume médio tender a ser maior que o peso médio.

### Mudança

Nas instâncias simétricas, usar faixas equivalentes para `peso` e `volume`.

### Implementação sugerida

- quando `W == V`, gerar ambos com a mesma escala
- por exemplo, ambos até `capacidade/10`
- manter as instâncias assimétricas com tratamento diferente

### Benefício

- simetria mais real
- comparação mais limpa entre `score_simples` e `score_otimizado`

## Etapa 4. Reduzir um pouco a rigidez do lucro

### Objetivo

Evitar que o lucro fique excessivamente previsível a partir de `peso + volume`.

### Problema atual

O lucro padrão é basicamente derivado do tamanho do item, com ruído relativamente pequeno. Isso pode fazer as instâncias ficarem homogêneas demais do ponto de vista estrutural.

### Mudança

Manter a ideia de lucro correlacionado ao consumo, mas com um pouco mais de variação.

### Implementação sugerida

- aumentar moderadamente o ruído
- ou usar fatores diferentes para `peso` e `volume`
- por exemplo:

```text
lucro ~ a * peso + b * volume + ruido
```

com `a` e `b` distintos conforme o tipo de instância

### Benefício

- maior diversidade entre itens
- melhor teste para heurísticas gulosas

## Etapa 5. Registrar metadados mínimos da geração

### Objetivo

Deixar claro como cada instância foi produzida.

### Mudança

Registrar pelo menos:

- semente usada
- percentual de itens de ouro
- tipo da instância
- parâmetros da fórmula de lucro

### Implementação sugerida

Você pode fazer isso de duas formas:

1. gravando comentários no início do arquivo da instância
2. gerando um arquivo auxiliar de metadados, como `.csv` ou `.txt`

### Benefício

- organização experimental
- rastreabilidade
- facilidade de escrita na documentação

## O que eu não recomendo mudar agora

Para esta fase, eu evitaria mudanças muito grandes, como:

- criar muitas famílias completamente novas de instância
- trocar totalmente a fórmula de lucro
- mexer ao mesmo tempo em distribuição, outliers e escalas
- reestruturar o formato do arquivo de saída sem necessidade

Isso pode ser deixado para uma segunda rodada, depois que você tiver resultados das melhorias iniciais.

## Ordem prática de implementação

Sugestão de ordem para alterar o gerador com segurança:

1. adicionar controle de semente
2. parametrizar percentual de itens de ouro
3. equilibrar `peso` e `volume` nos casos simétricos
4. ajustar levemente a fórmula do lucro
5. registrar metadados da geração

## Plano de execução

## Fase 1. Ajustes mínimos

Implementar:

- semente controlável
- percentual de itens de ouro por constante
- impressão dos parâmetros usados

Resultado esperado:

- mesmo gerador, mas com muito mais controle experimental

## Fase 2. Ajustes estruturais leves

Implementar:

- equilíbrio entre `peso` e `volume` em instâncias simétricas
- pequena revisão do cálculo do lucro

Resultado esperado:

- instâncias mais informativas sem perder o espírito da geração atual

## Fase 3. Validação

Depois de gerar a nova bateria:

1. comparar médias de `peso`, `volume` e `lucro`
2. medir quantos itens especiais aparecem
3. comparar distribuição dos scores
4. verificar se as instâncias simétricas continuam produzindo rankings equivalentes
5. rodar as heurísticas e medir gap para o ótimo

## Critério de sucesso

As melhorias serão úteis se produzirem pelo menos parte destes efeitos:

- instâncias mais reproduzíveis
- menor dependência de outliers extremos
- melhor distinção entre tipos de instância
- análise heurística mais rica
- documentação experimental mais clara

## Recomendação final

Se sua intenção é antecipar mudanças sem exagerar, eu faria apenas estas quatro agora:

1. controle de semente
2. parametrização dos itens de ouro
3. correção da assimetria artificial em casos `W = V`
4. registro de metadados

Deixaria a mudança na fórmula do lucro como quinto passo, logo em seguida, caso a análise mostre que a correlação atual ainda está forte demais.
