import os
import sys

import matplotlib.pyplot as plt
import numpy as np


INSTANCIAS = ["T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"]

# Valores extraidos da Tabela 1 do artigo.
GULOSO_SIMPLES = [92.34, 89.31, 99.94, 98.06, 97.44, 91.84, 99.35, 99.90]
GULOSO_BL = [94.90, 94.82, 99.94, 98.98, 94.43, 97.48, 94.44, 98.25]
ALEATORIO_BL = [96.13, 79.81, 86.96, 65.82, 98.84, 78.82, 72.20, 47.75]
GRASP = [99.32, 99.32, 99.21, 99.52, 97.38, 98.76, 99.21, 99.23]


def rotular_barras(ax, barras):
    for barra in barras:
        altura = barra.get_height()
        ax.annotate(
            f"{altura:.1f}",
            xy=(barra.get_x() + barra.get_width() / 2, altura),
            xytext=(0, 3),
            textcoords="offset points",
            ha="center",
            va="bottom",
            fontsize=8,
        )


def main():
    x = np.arange(len(INSTANCIAS))
    largura = 0.20

    plt.style.use("seaborn-v0_8-whitegrid")
    fig, ax = plt.subplots(figsize=(14, 7))

    barras_gs = ax.bar(
        x - 1.5 * largura,
        GULOSO_SIMPLES,
        largura,
        label="Guloso Simples",
        color="#4C78A8",
    )
    barras_bl = ax.bar(
        x - 0.5 * largura,
        GULOSO_BL,
        largura,
        label="Guloso + BL",
        color="#F58518",
    )
    barras_aleat = ax.bar(
        x + 0.5 * largura,
        ALEATORIO_BL,
        largura,
        label="Aleatorio + BL",
        color="#E45756",
    )
    barras_grasp = ax.bar(
        x + 1.5 * largura,
        GRASP,
        largura,
        label="GRASP",
        color="#54A24B",
    )

    ax.set_title("Tabela 1: Qualidade da Solucao por Instancia", fontsize=16, weight="bold")
    ax.set_xlabel("Instancias", fontsize=12)
    ax.set_ylabel("Qualidade da Solucao (% do Otimo)", fontsize=12)
    ax.set_xticks(x)
    ax.set_xticklabels(INSTANCIAS, fontsize=11)
    ax.set_ylim(45, 101)

    ax.axhline(99, color="gray", linestyle="--", linewidth=1.2, label="Referencia 99%")
    ax.grid(axis="y", linestyle="--", alpha=0.45)
    ax.grid(axis="x", visible=False)

    rotular_barras(ax, barras_gs)
    rotular_barras(ax, barras_bl)
    rotular_barras(ax, barras_aleat)
    rotular_barras(ax, barras_grasp)

    ax.annotate(
        "Colapso do inicio aleatorio na T8",
        xy=(x[7] + 0.5 * largura, ALEATORIO_BL[7]),
        xytext=(5.2, 56.5),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.annotate(
        "GRASP permanece estavel acima de 99% em quase todas as instancias",
        xy=(x[3] + 1.5 * largura, GRASP[3]),
        xytext=(1.2, 95.8),
        textcoords="data",
        arrowprops={"arrowstyle": "->", "lw": 1.2},
        fontsize=10,
    )

    ax.legend(ncol=3, frameon=True, loc="lower left")

    fig.tight_layout()

    diretorio_saida = os.path.dirname(__file__)
    caminho_saida = os.path.join(diretorio_saida, "grafico_qualidade_tabela1.png")
    fig.savefig(caminho_saida, dpi=300, bbox_inches="tight")

    if "--show" in sys.argv:
        plt.show()
    else:
        plt.close(fig)


if __name__ == "__main__":
    main()
