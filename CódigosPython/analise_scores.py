import sys
import os
sys.path.append(os.path.dirname(__file__))

from test import ler_instancia
import matplotlib.pyplot as plt

diretorio = "C:/temp/ProjetoAlg-MochilaGulosa/GeracaoInstancia"

instancias = []
for i in range(1, 9):
    caminho = os.path.join(diretorio, f"instancia{i}.txt")
    inst = ler_instancia(caminho)
    if inst:
        instancias.append(inst)

# --- Análise 1: Distribuição dos Scores ---

fig, axs = plt.subplots(4, 2, figsize=(14, 18))
fig.suptitle("Distribuição dos Scores: Simples vs Otimizado", fontsize=16)

for idx, inst in enumerate(instancias):
    row, col = divmod(idx, 2)
    ax = axs[row][col]

    df = inst.itens
    ax.hist(df['score_simples'], bins=30, alpha=0.6, label='Simples', color='steelblue')
    ax.hist(df['score_otimizado'], bins=30, alpha=0.6, label='Otimizado', color='coral')

    ax.set_title(f"Instância {inst.id_instancia} (m={inst.qtd_itens}, W={inst.peso_max}, V={inst.volume_max})")
    ax.set_xlabel("Score")
    ax.set_ylabel("Frequência")
    ax.legend()

plt.subplots_adjust(hspace=0.6, top=0.92)
plt.savefig("C:/temp/ProjetoAlg-MochilaGulosa/CódigosPython/distribuicao_scores.png", dpi=150)
plt.show()

# --- Estatísticas descritivas por instância ---

print(f"{'Inst':>4} | {'Score':>12} | {'Média':>8} | {'Mediana':>8} | {'DesvPad':>8} | {'Min':>6} | {'Max':>6} | {'CV%':>6}")
print("-" * 80)

for inst in instancias:
    df = inst.itens
    for score_nome in ['score_simples', 'score_otimizado']:
        s = df[score_nome]
        cv = (s.std() / s.mean()) * 100 if s.mean() != 0 else 0
        label = "Simples" if score_nome == 'score_simples' else "Otimizado"
        print(f"{inst.id_instancia:>4} | {label:>12} | {s.mean():>8.3f} | {s.median():>8.3f} | {s.std():>8.3f} | {s.min():>6.2f} | {s.max():>6.2f} | {cv:>6.1f}")
    print()