import os
import sys

import matplotlib.pyplot as plt
import numpy as np


INSTANCIAS = ["T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"]

# Valores extraidos da Tabela 2 do artigo.
# DP originalmente em segundos; os demais em milissegundos.
DP_MS = [23.387, 7498.684, 55061.60, 260268.5, 29.039, 5882.228, 31197.02, 113294.2]
GULOSO_SIMPLES_MS = [0.010, 0.046, 0.099, 0.215, 0.005, 0.024, 0.050, 0.103]
GULOSO_PURO_MS = [0.004, 0.030, 0.084, 0.245, 0.003, 0.019, 0.042, 0.099]
GULOSO_BL_MS = [0.018, 0.288, 0.249, 3.776, 0.015, 0.334, 1.290, 0.874]
ALEATORIO_BL_MS = [0.026, 0.154, 1.180, 1.139, 0.024, 0.076, 0.452, 0.587]
GRASP_MS = [0.767, 7.711, 25.883, 34.094, 0.540, 4.255, 16.706, 32.142]


def main():
    x = np.arange(len(INSTANCIAS))
    largura = 0.13

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(15, 7.5))

    ax.bar(x - 2.5 * largura, DP_MS, largura, label="DP", color="#4C78A8")
    ax.bar(x - 1.5 * largura, GULOSO_SIMPLES_MS, largura, label="Guloso Simples", color="#9ECAE9")
    ax.bar(x - 0.5 * largura, GULOSO_PURO_MS, largura, label="Guloso Puro", color="#F58518")
    ax.bar(x + 0.5 * largura, GULOSO_BL_MS, largura, label="Guloso + BL", color="#FFBF79")
    ax.bar(x + 1.5 * largura, ALEATORIO_BL_MS, largura, label="Aleatorio + BL", color="#E45756")
    ax.bar(x + 2.5 * largura, GRASP_MS, largura, label="GRASP", color="#54A24B")

    ax.set_title("Tabela 2: Comparativo do Tempo de Solucao por Instancia", fontsize=16, weight="bold")
    ax.set_xlabel("Instancias", fontsize=12)
    ax.set_ylabel("Tempo de execucao (ms, escala logaritmica)", fontsize=12)
    ax.set_xticks(x)
    ax.set_xticklabels(INSTANCIAS, fontsize=11)
    ax.set_yscale("log")

    ax.grid(axis="y", linestyle="--", alpha=0.45)
    ax.grid(axis="x", visible=False)

    ax.annotate(
        "A DP cresce ordens de grandeza acima das heuristicas",
        xy=(x[3] - 2.5 * largura, DP_MS[3]),
        xytext=(1.4, 40000),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.annotate(
        "Mesmo o GRASP permanece muito abaixo da DP",
        xy=(x[7] + 2.5 * largura, GRASP_MS[7]),
        xytext=(4.2, 150),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.legend(ncol=3, frameon=True, loc="upper left")
    fig.tight_layout()

    diretorio_saida = os.path.dirname(__file__)
    caminho_saida = os.path.join(diretorio_saida, "grafico_tempo_tabela2.png")
    fig.savefig(caminho_saida, dpi=300, bbox_inches="tight")

    if "--show" in sys.argv:
        plt.show()
    else:
        plt.close(fig)


if __name__ == "__main__":
    main()
