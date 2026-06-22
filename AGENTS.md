# AGENTS.md

## Project

Comparison of algorithms for the 0/1 bidimensional knapsack problem (weight + volume constraints). Written in C, with a Python analysis helper.

## Build

Both C programs use `make` with GCC on Windows. `clean` targets use `del` (Windows-only).

```sh
# Generate instances + DP optimal solutions
cd GeracaoInstancia && make
# Output: gerador_mochila.exe

# Run heuristic comparisons
cd Comparacao_Heuristicas && make
# Output: simulador.exe

# Run comparisons over the official benchmark instances
cd Comparacao_Oficial && make
# Output: simulador_oficial.exe
# NOTE: must be executed from inside Comparacao_Oficial/ (uses relative path
# ../Instancias de Teste/Teste%d.txt). On Windows run: simulador_oficial.exe
```

**Optimization flags differ by intent**: `GeracaoInstancia` compiles with `-O3` (DP must finish fast), while `Comparacao_Heuristicas` and `Comparacao_Oficial` compile with `-O0` (timing comparisons must be fair and unspecialized). If `make` is not on PATH, build with: `gcc -Wall -O0 main.c algoritmos.c utils.c -o simulador_oficial.exe`.

## Known Issues

- **Hardcoded stale path** in `Comparacao_Heuristicas/main.c:31` — reads instances from `C:/temp/Tarefa2_Final_Arthur/GeracaoInstancia/` instead of the local `GeracaoInstancia/` directory. Must update this path or switch to relative paths before running `simulador.exe`.
- **`Item.score` type mismatch** between programs: `double` in `GeracaoInstancia/item.h` vs `float` in `Comparacao_Heuristicas/item.h`. The two programs are independent binaries, but copy-paste edits should stay consistent.
- `.vscode/launch.json` also contains the stale `Tarefa2_Final_Arthur` path.

## Architecture

- `GeracaoInstancia/` — Generates 8 benchmark instances (`instancia1.txt`–`instancia8.txt`) with random items and solves each with DP to write the optimal profit. Entry: `main.c` → `gerador.c` + `dp.c`.
- `Comparacao_Heuristicas/` — Reads those instance files and benchmarks four methods: greedy-simple, greedy-optimized, greedy+local-search, random+local-search. Uses `QueryPerformanceCounter` (Windows API) for high-precision timing. Entry: `main.c` → `algoritmos.c` + `utils.c`.
- `Comparacao_Oficial/` — Same four heuristics, but over the official benchmark instances in `Instancias de Teste/`. There is no precomputed optimum, so DP is run live as the baseline. Each method operates on its own `memcpy` of the pristine instance to avoid cross-contamination from `qsort`/`selecionado` state. Entry: `main.c` → `algoritmos.c` + `utils.c`.
- `Instancias de Teste/` — Official benchmark instances (`Teste1.txt`–`Teste8.txt`) in a different format (see below).
- `CódigosPython/test.py` — Reads instance files with pandas for inspection. Uses the `.venv` Python 3.10 venv (numpy + pandas installed).

## Instance File Format

Two distinct formats exist in this repo:

### `GeracaoInstancia/instancia*.txt` and `Comparacao_Heuristicas/instancia*.txt`

```
m W_max V_max
id peso volume lucro
...  (m lines)
lucro_otimo
```

First line = header, next m lines = items, last line = DP optimal profit.

### `Instancias de Teste/Teste*.txt` (official benchmark, used by `Comparacao_Oficial`)

```
dim m
<lucro_1> ... <lucro_m>          (token stream, m values, line breaks ignored)
W_max V_max
<p_1> <v_1> ... <p_m> <v_m>      (token stream, m (peso, volume) pairs)
```

First line = dimensionality (`2` = weight+volume) + item count. Next m tokens = profits. Then capacities. Then m weight/volume pairs. No precomputed optimum; DP is run live.