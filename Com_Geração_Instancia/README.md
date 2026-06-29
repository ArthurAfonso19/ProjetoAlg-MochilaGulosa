# Com_Geração_Instancia

Este diretório reúne o fluxo completo baseado em instâncias pseudoaleatórias do problema da mochila bidimensional 0/1, com restrições simultâneas de peso e volume.

Ele está dividido em dois programas independentes:

- `GeracaoInstancia/`: gera 8 instâncias de teste, resolve cada uma com Programação Dinâmica e salva o ótimo junto com o tempo da DP.
- `Comparacao_Heuristicas/`: lê essas instâncias já prontas e compara heurísticas e metaheurísticas sem recalcular a DP.

## Estrutura

- `GeracaoInstancia/`
  - Gera itens pseudoaleatórios.
  - Resolve cada instância com DP.
  - Salva `lucro_otimo` e `tempo_dp` no arquivo da instância.
- `Comparacao_Heuristicas/`
  - Lê os arquivos gerados em `../GeracaoInstancia/`.
  - Executa:
    - Guloso simples
    - Guloso otimizado
    - Guloso + busca local
    - Aleatório + busca local
    - GRASP
  - Usa o ótimo e o tempo da DP gravados no arquivo como baseline.

## Fluxo de uso

1. Compilar e executar `GeracaoInstancia`.
2. Isso gera `instancia1.txt` até `instancia8.txt`.
3. Compilar e executar `Comparacao_Heuristicas`.
4. O comparador lê os arquivos gerados e imprime lucro, qualidade e speedup.

## Build

Os dois projetos usam `gcc` e `make` no Windows.

### GeracaoInstancia

Compila com `-O3` e `--large-address-aware`, porque a DP pode consumir muita memória.

```powershell
cd "C:\temp\ProjetoAlg-MochilaGulosa\Com_Geração_Instancia\GeracaoInstancia"
mingw32-make
```

Executar:

```powershell
.\gerador_mochila.exe
```

### Comparacao_Heuristicas

Compila com `-O0`, já que o objetivo é medir os métodos heurísticos sem otimizações agressivas do compilador.

```powershell
cd "C:\temp\ProjetoAlg-MochilaGulosa\Com_Geração_Instancia\Comparacao_Heuristicas"
mingw32-make
```

Executar:

```powershell
.\simulador.exe
```

## Formato das instâncias geradas

Cada arquivo `instancia*.txt` possui este formato:

```text
m W_max V_max
id peso volume lucro
... (m linhas)

lucro_otimo
tempo_dp
```

Exemplo simplificado:

```text
50 1000 1000
1 20 15 90
2 50 10 120
...

7542
0.023387
```

Onde:

- `m`: quantidade de itens
- `W_max`: capacidade máxima de peso
- `V_max`: capacidade máxima de volume
- `lucro_otimo`: melhor valor encontrado pela DP
- `tempo_dp`: tempo de execução da DP em segundos

## Instâncias geradas

O gerador cria 8 cenários fixos:

- 4 instâncias simétricas com `W = V`
- 4 instâncias assimétricas com `W != V`

Escalas usadas:

- 50 itens
- 250 itens
- 500 itens
- 1000 itens

## Decisão de arquitetura

Neste fluxo, a DP roda apenas no gerador.

Isso foi feito para evitar que o comparador:

- recalcule um ótimo que já existe no arquivo
- sofra com falhas de memória nas instâncias maiores
- distorça o baseline de tempo da DP ao tentar resolver instâncias grandes novamente

Por isso, o `Comparacao_Heuristicas`:

- não chama mais `resolverDP(...)`
- apenas lê `lucro_otimo` e `tempo_dp` do arquivo da instância

## Observações

- O comparador deve ser executado dentro da pasta `Comparacao_Heuristicas/`, porque ele usa o caminho relativo `../GeracaoInstancia/instancia%d.txt`.
- Se as instâncias forem antigas e não tiverem a linha `tempo_dp`, o comparador não terá o baseline correto de tempo. Nesse caso, regenere as instâncias.
- O speedup exibido compara os tempos atuais das heurísticas com o tempo da DP previamente salvo no arquivo.

## Limpeza

Para remover os executáveis:

### GeracaoInstancia

```powershell
mingw32-make clean
```

### Comparacao_Heuristicas

```powershell
mingw32-make clean
```
