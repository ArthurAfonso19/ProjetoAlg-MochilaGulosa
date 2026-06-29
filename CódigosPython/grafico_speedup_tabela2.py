import os
import sys

import matplotlib.pyplot as plt
import numpy as np


INSTANCIAS = ["T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"]

# Valores de speedup extraidos da Tabela 2 do artigo.
GULOSO_SIMPLES = [2338, 162000, 555000, 1200000, 5807, 243000, 618000, 1100000]
GULOSO_PURO = [5846, 249000, 653000, 1000000, 10755, 312000, 739000, 1100000]
GULOSO_BL = [1271, 26000, 221000, 68000, 1962, 17000, 24000, 129000]
ALEATORIO_BL = [913, 48000, 46000, 228000, 1190, 77000, 69000, 192000]
GRASP = [30, 972, 2127, 7633, 53, 1382, 1867, 3524]


def main():
    x = np.arange(len(INSTANCIAS))
    largura = 0.16

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(15, 7.5))

    ax.bar(x - 2 * largura, GULOSO_SIMPLES, largura, label="Guloso Simples", color="#4C78A8")
    ax.bar(x - 1 * largura, GULOSO_PURO, largura, label="Guloso Puro", color="#9ECAE9")
    ax.bar(x, GULOSO_BL, largura, label="Guloso + BL", color="#F58518")
    ax.bar(x + 1 * largura, ALEATORIO_BL, largura, label="Aleatorio + BL", color="#E45756")
    ax.bar(x + 2 * largura, GRASP, largura, label="GRASP", color="#54A24B")

    ax.set_title("Tabela 2: Speedup em Relacao a DP por Instancia", fontsize=16, weight="bold")
    ax.set_xlabel("Instancias", fontsize=12)
    ax.set_ylabel("Speedup (vezes mais rapido que a DP, escala logaritmica)", fontsize=12)
    ax.set_xticks(x)
    ax.set_xticklabels(INSTANCIAS, fontsize=11)
    ax.set_yscale("log")

    ax.grid(axis="y", linestyle="--", alpha=0.45)
    ax.grid(axis="x", visible=False)

    ax.annotate(
        "Heuristicas gulosas atingem speedups na ordem de milhoes",
        xy=(x[3] - 2 * largura, GULOSO_SIMPLES[3]),
        xytext=(1.0, 200000),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.annotate(
        "GRASP e mais custoso, mas ainda supera a DP com ampla margem",
        xy=(x[7] + 2 * largura, GRASP[7]),
        xytext=(3.7, 180),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.legend(ncol=3, frameon=True, loc="upper left")
    fig.tight_layout()

    diretorio_saida = os.path.dirname(__file__)
    caminho_saida = os.path.join(diretorio_saida, "grafico_speedup_tabela2.png")
    fig.savefig(caminho_saida, dpi=300, bbox_inches="tight")

    if "--show" in sys.argv:
        plt.show()
    else:
        plt.close(fig)


if __name__ == "__main__":
    main()
