# Análise das Instâncias Geradas

## O que já existe em `CódigosPython`

Hoje existem dois scripts principais:

- `test.py`
- `analise_scores.py`

Eles já formam um começo útil para leitura e inspeção das instâncias geradas em `Com_Geração_Instancia/GeracaoInstancia/`.

## O que `test.py` já faz

O script define uma classe `Instancia` para armazenar:

- `id_instancia`
- `qtd_itens`
- `peso_max`
- `volume_max`
- `lucro_otimo`
- `itens`

O campo `itens` é um `DataFrame` do pandas com as colunas:

- `id`: identificador do item
- `peso`: peso do item
- `volume`: volume do item
- `lucro`: lucro do item
- `score_simples`: `lucro / (peso + volume)`
- `score_otimizado`: `lucro / (alpha * peso + beta * volume)`, com `alpha = 1 / W_max` e `beta = 1 / V_max`

Em seguida, o script tenta ler as 8 instâncias e imprime um resumo com:

- número de itens
- lucro ótimo
- capacidades
- `head()` do `DataFrame`

## O que `analise_scores.py` já faz

O script reaproveita `ler_instancia()` e monta uma análise inicial dos scores heurísticos:

- lê as 8 instâncias
- gera histogramas de `score_simples` e `score_otimizado`
- salva a figura `distribuicao_scores.png`
- imprime estatísticas descritivas por instância:
  - média
  - mediana
  - desvio-padrão
  - mínimo
  - máximo
  - coeficiente de variação

Essa base já é boa para uma primeira investigação da distribuição dos itens e para comparar o critério guloso simples com o critério ponderado pelas capacidades.

## Limitações já observadas no código atual

### 1. Caminhos absolutos antigos

Os dois scripts usam o caminho:

```python
C:/temp/ProjetoAlg-MochilaGulosa/GeracaoInstancia
```

Mas no workspace atual as instâncias estão em:

```text
Com_Geração_Instancia/GeracaoInstancia/
```

Isso precisa ser ajustado para caminho relativo, senão a análise não roda neste repositório sem edição manual.

### 2. Efeito colateral ao importar `test.py`

`analise_scores.py` faz:

```python
from test import ler_instancia
```

Mas `test.py` não contém apenas funções e classes: ele executa leitura e impressão no corpo principal do arquivo. Na prática, ao importar `ler_instancia`, todo o bloco final também é executado. Funciona, mas mistura reutilização com execução automática.

### 3. Dependência do pandas/matplotlib

Os scripts assumem ambiente com `pandas` e `matplotlib`. No estado atual do ambiente, isso deve ser verificado antes de executar a análise.

### 4. A análise atual é centrada só nos scores

O foco inicial está correto, mas ainda faltam análises sobre:

- correlação entre `peso`, `volume` e `lucro`
- densidade das instâncias
- saturação esperada das capacidades
- presença de outliers
- dificuldade estrutural para heurísticas gulosas
- variação entre instâncias simétricas e assimétricas

## Análise inicial das instâncias geradas

Usei os arquivos já gerados em `Com_Geração_Instancia/GeracaoInstancia/` para extrair algumas estatísticas estruturais.

## Resumo por instância

| Inst. | m | W | V | Lucro ótimo | Peso médio | Volume médio | Lucro médio |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| 1 | 50 | 1000 | 1000 | 2274 | 45.62 | 106.04 | 163.42 |
| 2 | 250 | 8000 | 8000 | 31284 | 420.02 | 779.20 | 1298.02 |
| 3 | 500 | 15000 | 15000 | 59123 | 720.10 | 1399.26 | 2273.56 |
| 4 | 1000 | 23000 | 23000 | 92514 | 1078.19 | 2146.22 | 3400.09 |
| 5 | 50 | 800 | 1500 | 3814 | 37.96 | 135.26 | 204.98 |
| 6 | 250 | 12000 | 4000 | 33840 | 579.68 | 394.84 | 1080.36 |
| 7 | 500 | 7000 | 18000 | 54933 | 357.81 | 1680.50 | 2189.73 |
| 8 | 1000 | 23000 | 10000 | 75079 | 1092.21 | 967.73 | 2207.65 |

## Leitura desses números

### 1. Mesmo quando `W = V`, o volume médio sai bem maior que o peso médio

Nas instâncias 1 a 4, o gerador cria itens com:

- `peso` até aproximadamente `W/10`
- `volume` até aproximadamente `V/5`

Então, mesmo nas instâncias simétricas, o volume tende a ser estruturalmente maior que o peso. Isso significa que a geração já induz assimetria no perfil dos itens, mesmo quando as capacidades são iguais.

### 2. O lucro está fortemente amarrado ao tamanho do item

No caso padrão, o lucro é basicamente:

```text
lucro ~ peso + volume + ruido
```

Isso cria correlação forte entre consumo de recurso e benefício. Em termos de análise, isso é bom para controle, mas pode deixar as instâncias menos diversas e menos desafiadoras para distinguir heurísticas.

### 3. Existem itens muito favorecidos pelo mecanismo de "itens de ouro"

O gerador injeta 5% de itens com:

- peso muito baixo
- volume muito baixo
- lucro muito alto

Nas instâncias maiores isso produz vários itens excepcionalmente atrativos. Aproximação observada:

- instância 2: cerca de 12 itens ouro
- instância 3: cerca de 25 itens ouro
- instância 4: cerca de 53 itens ouro
- instância 6: cerca de 14 itens ouro
- instância 7: cerca de 27 itens ouro
- instância 8: cerca de 46 itens ouro

Esses itens podem dominar a solução e favorecer fortemente métodos gulosos baseados em razão lucro/recurso.

### 4. Em instâncias simétricas, `score_otimizado` vira só uma reescala de `score_simples`

Quando `W = V`, temos:

```text
score_otimizado = lucro / ((1/W) * peso + (1/V) * volume)
                 = W * lucro / (peso + volume)
```

Ou seja, a ordenação dos itens é exatamente a mesma de `score_simples`. Na prática, nas instâncias 1 a 4, comparar esses dois scores não mede duas heurísticas diferentes; mede a mesma ideia com escala diferente.

### 5. As instâncias assimétricas são mais informativas para comparar scores

As instâncias 5 a 8 são mais úteis para avaliar se ponderar `peso` e `volume` pelas capacidades realmente muda a ordenação dos itens e o comportamento das heurísticas.

## Considerações sobre sua geração inicial de instâncias

Sua geração inicial já tem qualidades importantes:

- separa casos pequenos, médios e grandes
- inclui cenários com capacidades iguais e diferentes
- grava o lucro ótimo da DP, o que é muito valioso para avaliação posterior
- produz instâncias suficientes para começar uma análise comparativa

Mas também há algumas características que influenciam fortemente os resultados:

- o volume tende a ser maior que o peso já no desenho do gerador
- o lucro depende fortemente de `peso + volume`
- os "itens de ouro" podem facilitar demais a escolha gulosa
- metade das instâncias não diferencia `score_simples` de `score_otimizado` em termos de ranking

## Sugestões de melhoria para a geração

### 1. Separar famílias de instância

Em vez de uma única lógica de geração, vale criar famílias com comportamentos distintos:

- instâncias correlacionadas: lucro cresce com peso e volume
- instâncias fracamente correlacionadas: lucro com ruído maior
- instâncias anticorrelacionadas parciais: itens leves com lucro baixo e alguns pesados com lucro alto
- instâncias com muitos empates de score
- instâncias sem outliers fortes
- instâncias com outliers controlados

Isso ajuda a avaliar robustez das heurísticas, e não só desempenho em um único padrão estatístico.

### 2. Reduzir ou parametrizar os itens de ouro

Em vez de fixar 5%, você pode:

- testar 0%, 1%, 3% e 5%
- limitar a diferença de lucro extremo
- registrar explicitamente quantos itens especiais foram gerados

Assim fica mais fácil medir o quanto os resultados dependem desses itens.

### 3. Tornar peso e volume mais equilibrados nas instâncias simétricas

Se `W = V`, faz sentido considerar distribuições parecidas para `peso` e `volume`, caso o objetivo seja construir casos realmente simétricos.

### 4. Variar o modelo de lucro

Boas alternativas:

- `lucro ~ peso + volume + ruido pequeno`
- `lucro ~ max(peso, volume) + ruido`
- `lucro` quase aleatório dentro de uma faixa
- lucro por grupos de item, simulando categorias

Isso torna a bateria mais expressiva.

### 5. Controlar reprodutibilidade

Como a geração usa `srand(time(NULL))`, os arquivos mudam a cada execução. Para análise experimental, é útil permitir:

- semente fixa
- semente informada por argumento
- gravação da semente usada no arquivo ou no log

### 6. Registrar metadados da instância

Além do conteúdo atual, pode ser útil salvar:

- semente aleatória
- tipo de geração
- percentual de itens especiais
- correlação esperada entre atributos

## Plano de análise em Python

O objetivo aqui é transformar a leitura atual em uma análise reproduzível, comparável e fácil de expandir.

## Passo 1. Corrigir a base de leitura

Criar um módulo de leitura com:

- caminho relativo ao projeto
- uma função para ler uma instância
- uma função para ler todas as instâncias
- bloco `if __name__ == '__main__':` para evitar execução automática em import

Saída esperada:

- lista de objetos `Instancia`
- ou um dicionário por instância

## Passo 2. Padronizar um `DataFrame` por item e um `DataFrame` por instância

Manter dois níveis de análise:

- `df_itens_total`: todos os itens de todas as instâncias, com coluna `instancia`
- `df_resumo_instancias`: uma linha por instância, com estatísticas agregadas

Colunas úteis no `df_itens_total`:

- `instancia`
- `id`
- `peso`
- `volume`
- `lucro`
- `score_simples`
- `score_otimizado`
- `peso_rel = peso / W_max`
- `volume_rel = volume / V_max`
- `lucro_por_peso = lucro / peso`
- `lucro_por_volume = lucro / volume`

## Passo 3. Fazer análise descritiva básica

Para cada instância, calcular:

- média, mediana e desvio-padrão
- mínimos e máximos
- quartis
- coeficiente de variação
- correlações entre `peso`, `volume` e `lucro`

Saídas:

- tabela resumo em `.csv`
- tabela resumo em `.md`

## Passo 4. Gerar visualizações mais informativas

Além dos histogramas de score, gerar:

- histograma de `peso`
- histograma de `volume`
- histograma de `lucro`
- scatter `peso x lucro`
- scatter `volume x lucro`
- scatter `peso_rel x volume_rel`
- boxplots por instância
- heatmap de correlação

Objetivo:

- detectar assimetrias, clusters e outliers

## Passo 5. Medir o quanto as heurísticas de score realmente diferem

Comparar, para cada instância:

- correlação entre `score_simples` e `score_otimizado`
- percentual de itens cuja posição no ranking muda muito
- interseção entre top-k itens de cada score

Isso responde se o score otimizado está de fato trazendo informação nova.

## Passo 6. Relacionar estrutura da instância com dificuldade heurística

Cruzar características da instância com resultados do simulador heurístico:

- gap para o ótimo
- tempo de execução
- estabilidade entre execuções aleatórias

Perguntas úteis:

- instâncias com mais outliers favorecem mais o guloso?
- instâncias mais assimétricas ajudam o score otimizado?
- instâncias com maior dispersão de score facilitam ou dificultam busca local?

## Passo 7. Produzir relatório final automático

Gerar um relatório em Markdown com:

- resumo das instâncias
- gráficos salvos em pasta própria
- principais conclusões
- pontos de atenção para o gerador

## Passos de execução sugeridos

### Pré-requisito

Verificar se o ambiente Python tem os pacotes necessários:

```powershell
.\.venv\Scripts\python.exe -m pip install pandas matplotlib seaborn
```

### Execução recomendada

1. Ajustar os scripts para usar caminhos relativos.
2. Criar um módulo de leitura reutilizável, por exemplo `CódigosPython/leitor_instancias.py`.
3. Gerar um arquivo de resumo tabular, por exemplo `CódigosPython/resumo_instancias.csv`.
4. Gerar gráficos em `CódigosPython/graficos/`.
5. Consolidar tudo em um relatório final `.md`.

### Ordem prática de implementação

1. Refatorar leitura.
2. Montar `df_itens_total`.
3. Montar `df_resumo_instancias`.
4. Validar estatísticas básicas.
5. Gerar gráficos.
6. Cruzar com resultados heurísticos.
7. Escrever conclusões finais.

## Conclusão inicial

Você já começou pela parte certa: leitura estruturada das instâncias e comparação entre dois scores gulosos. Isso já permite sair do nível apenas algorítmico e entrar em análise empírica dos dados.

O próximo passo mais importante é tornar essa análise reproduzível e mais abrangente. Hoje o principal risco é tirar conclusões fortes a partir de instâncias cuja geração já carrega vieses bem marcados, especialmente:

- diferença estrutural entre peso e volume
- forte correlação entre lucro e tamanho
- presença de itens extremamente vantajosos
- equivalência prática entre os dois scores nas instâncias simétricas

Se essas características forem controladas ou ao menos medidas explicitamente, sua seção de análise fica muito mais sólida.
