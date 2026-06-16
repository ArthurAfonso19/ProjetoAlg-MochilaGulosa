# Planejamento de Análise dos Dados das Instâncias

## Contexto

O projeto compara quatro heurísticas para o problema da mochila 0/1 bidimensional (peso + volume). Observou-se que, em muitos casos, o **Guloso Simples** supera o **Guloso Otimizado**, o que é contraintuitivo. O objetivo desta análise é entender **por que isso acontece** e se a forma como as instâncias são geradas favorece algum método em detrimento de outro.

---

## Hipótese Inicial: Viés na Geração de Instâncias

No `gerador.c`, os itens comuns (95%) seguem o padrão:

- `peso ∈ [1, W_max/10]`
- `volume ∈ [1, V_max/5]` (intervalo 2x maior que peso)
- `lucro ≈ peso + volume ± ruído pequeno`

Isso gera dois problemas:

1. **Lucro quase proporcional a (peso + volume)**: o score simples `lucro/(peso+volume)` fica próximo de 1 para quase todos os itens, fazendo a ordenação depender quase exclusivamente de quem tem menor denominador.

2. **Desbalanceamento peso/volume**: o volume dos itens é sistematicamente maior que o peso (`V_max/5` vs `W_max/10`). A normalização no score otimizado (`α·peso + β·volume` com `α=1/W_max`, `β=1/V_max`) pode estar sobre-compensando essa diferença, penalizando incorretamente itens com volume alto mas lucro proporcionalmente bom.

---

## Análises Propostas

### 1. Distribuição dos Scores (Simples vs Otimizado)

**Objetivo**: Verificar se as duas funções de score produzem rankings significativamente diferentes.

**Método**:
- Para cada instância, calcular o score simples e otimizado de todos os itens
- Plotar histogramas lado a lado das distribuições de score
- Ver se os scores se concentram em torno de valores próximos (o que reduziria a diferença entre os métodos)

**O que procurar**: Se ambos os scores têm distribuição quase uniforme/constante, a ordenação é essencialmente arbitrária e qualquer vantagem é acidental.

---

### 2. Concordância de Rankings (Correlação de Kendall)

**Objetivo**: Medir quantitativamente o quanto os rankings dos dois métodos divergem.

**Método**:
- Ordenar itens por cada score
- Calcular a correlação tau de Kendall entre os dois rankings
- Repetir para as 8 instâncias

**O que procurar**:
- Se tau ≈ 1: os métodos escolhem quase os mesmos itens; diferenças de lucro vêm de poucos itens nas bordas da mochila
- Se tau próximo de 0: a normalização muda drasticamente as decisões; investigar qual ranking está mais alinhado com o ótimo

---

### 3. Análise Bidimensional (Peso × Volume)

**Objetivo**: Entender a distribuição dos itens no espaço peso-volume e identificar dominâncias.

**Método**:
- Scatter plot `peso × volume` por instância, com cor/tamanho proporcional ao lucro
- Histograma da razão `volume/peso` dos itens
- Marcar os itens "gold" (5% com lucro 1000-1500 e dimensões mínimas)

**O que procurar**:
- Itens concentrados em uma região do espaço (indicando pouco diversity)
- Se a maioria dos itens tem volume >> peso, o score simples (que não diferencia dimensões) pode estar implicitamente favorecendo itens leves em peso

---

### 4. Taxa de Ocupação por Dimensão

**Objetivo**: Verificar se uma dimensão é consistentemente folgada, o que tornaria a normalização do score otimizado inútil ou prejudicial.

**Método**:
- Comparar, para cada instância, o peso total e volume total dos itens selecionados pelo ótimo vs as capacidades máximas
- Calcular `ocupação_W = peso_usado / W_max` e `ocupação_V = volume_usado / V_max`
- Fazer um gráfico de barras comparando as taxas por instância

**O que procurar**:
- Se W_max é sempre muito folgado (ocupação < 60%), o score otimizado que normaliza por `1/W_max` está super-valorizando a dimensão que não é o gargalo
- Instâncias 1-4 (W=V) dever ter ocupação balanceada; instâncias 5-8 (W≠V) revelam qual dimensão é o gargalo real

---

### 5. Impacto dos Itens Gold

**Objetivo**: Isolar o efeito dos 5% de itens "gold" nos resultados dos métodos gulosos.

**Método**:
- Identificar os itens gold em cada instância (lucro >> 500, peso e volume < 2% da capacidade)
- Rodar análise comparativa: (a) com todos os itens, (b) removendo os golds
- Verificar se os golds são sempre selecionados por ambos os métodos ou se algum método os perde

**O que procurar**:
- Se os golds são sempre selecionados por ambos, a diferença entre os métodos está nos itens comuns
- Se algum método deixa de selecionar golds (pelo score baixo em peso ou volume absoluto), isso indica um problema no score

---

### 6. Gap por Tipo de Instância (Simétrica vs Assimétrica)

**Objetivo**: Verificar se o Guloso Otimizado deveria ser superior nas instâncias assimétricas (W≠V) mas não é.

**Método**:
- Agrupar os resultados em dois blocos: instâncias 1-4 (W=V) e instâncias 5-8 (W≠V)
- Calcular o gap médio (`(ótimo - heurística)/ótimo`) de cada método por bloco
- Plotar o gap por instância com linha distinguishing os dois blocos

**O que procurar**:
- Se o guloso otimizado é consistentemente pior no bloco W≠V, a normalização está contraproducente
- Se não há diferença entre blocos, o problema é mais fundamental (lucro ≈ tamanho)

---

### 7. Itens Selecionados: Score vs Lucro Real

**Objetivo**: Verificar se os itens escolhidos pelo score otimizado têm, de fato, menor lucro que os do simples.

**Método**:
- Simular a seleção gulosa com cada score em Python (reimplementação simples)
- Listar os itens selecionados por cada método
- Comparar: lucro total, peso total, volume total, e lucro individual dos itens
- Identificar em quais pontos da mochila os métodos divergem (qual item um inclui e o outro não)

**O que procurar**:
- Se o método otimizado troca um item de lucro alto (mas score relativamente menor) por um de lucro baixo (mas score relativamente maior), isso confirma que a normalização distorce a ordem real de importância
- Mapear os "itens controversos": selecionados por um método mas não pelo outro

---

## Ordem Sugerida de Execução

1. **Análise 3** (distribuição peso×volume) — diagnóstico fundamental da geração
2. **Análise 4** (taxa de ocupação) — identifica gargalos reais das instâncias
3. **Análise 1** (distribuição dos scores) — revela se os scores são discriminativos
4. **Análise 2** (Kendall tau) — quantifica a divergência entre rankings
5. **Análise 5** (itens gold) — isola o efeito dos outliers
6. **Análise 6** (gap por bloco) — testa a hipótese W=V vs W≠V
7. **Análise 7** (itens selecionados) — diagnóstico fino, item a item

As análises 1-4 são independentes e podem ser feitas em paralelo. As análises 5-7 dependem de entendimento prévio dos resultados de 1-4.

---

## Estrutura do Código Python

O arquivo `CódigosPython/test.py` já possui:
- Classe `Instancia` com atributos: `qtd_itens`, `peso_max`, `volume_max`, `lucro_otimo`, `itens` (DataFrame)
- Função `ler_instancia()` que parseia os arquivos `.txt`

**Extensões necessárias**:
- Adicionar colunas calculadas ao DataFrame de itens: `score_simples`, `score_otimizado`, `razao_volume_peso`
- Adicionar flags: `is_gold` (itens com lucro > 500 e dimensões < 2% da capacidade)
- Implementar simulação gulosa em Python (para análise 7)
- Adicionar `matplotlib` para gráficos (instalar no `.venv`)