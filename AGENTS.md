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
```

**Optimization flags differ by intent**: `GeracaoInstancia` compiles with `-O3` (DP must finish fast), while `Comparacao_Heuristicas` compiles with `-O0` (timing comparisons must be fair and unspecialized).

## Known Issues

- **Hardcoded stale path** in `Comparacao_Heuristicas/main.c:31` — reads instances from `C:/temp/Tarefa2_Final_Arthur/GeracaoInstancia/` instead of the local `GeracaoInstancia/` directory. Must update this path or switch to relative paths before running `simulador.exe`.
- **`Item.score` type mismatch** between programs: `double` in `GeracaoInstancia/item.h` vs `float` in `Comparacao_Heuristicas/item.h`. The two programs are independent binaries, but copy-paste edits should stay consistent.
- `.vscode/launch.json` also contains the stale `Tarefa2_Final_Arthur` path.

## Architecture

- `GeracaoInstancia/` — Generates 8 benchmark instances (`instancia1.txt`–`instancia8.txt`) with random items and solves each with DP to write the optimal profit. Entry: `main.c` → `gerador.c` + `dp.c`.
- `Comparacao_Heuristicas/` — Reads those instance files and benchmarks four methods: greedy-simple, greedy-optimized, greedy+local-search, random+local-search. Uses `QueryPerformanceCounter` (Windows API) for high-precision timing. Entry: `main.c` → `algoritmos.c` + `utils.c`.
- `CódigosPython/test.py` — Reads instance files with pandas for inspection. Uses the `.venv` Python 3.10 venv (numpy + pandas installed).

## Instance File Format

```
m W_max V_max
id peso volume lucro
...  (m lines)
lucro_otimo
```

First line = header, next m lines = items, last line = DP optimal profit.