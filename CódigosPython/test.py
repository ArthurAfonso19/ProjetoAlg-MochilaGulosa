import pandas as pd
import os 

class Instancia: 
    #Classe para armazenar os dados e os itens de uma instancia
    def __init__(self, id_instancia, m, w_max, v_max, lucro_otimo, df_itens):
        self.id_instancia = id_instancia
        self.qtd_itens = m 
        self.peso_max = w_max
        self.volume_max = v_max 
        self.lucro_otimo = lucro_otimo
        self.itens = df_itens   

def ler_instancia(caminho_arquivo):
    if not os.path.exists(caminho_arquivo):
        print(f"Erro: Arquivo {caminho_arquivo} não encontrado")
        return None
    
    with open(caminho_arquivo, 'r') as f:
        cabecalho = f.readline().strip().split()
        m, w_max, v_max = map(int, cabecalho)

        dados_itens = []
        for _ in range(m):
            linha = f.readline().strip().split()
            if linha:
                dados_itens.append(list(map(int, linha)))
        
        conteudo_restante = f.read().strip()
        lucro_otimo = int(conteudo_restante) if conteudo_restante else 0 

    colunas = ['id', 'peso', 'volume', 'lucro']
    df_itens = pd.DataFrame(dados_itens, columns=colunas)

    id_id = "".join(filter(str.isdigit, os.path.basename(caminho_arquivo)))

    return Instancia(id_id, m, w_max, v_max, lucro_otimo, df_itens)


lista_instancias = []
diretorio = "C:/temp/Tarefa2_Final_Arthur/GeracaoInstancia"

for i in range(1, 9):
    nome_arq = f"instancia{i}.txt"
    caminho = os.path.join(diretorio, nome_arq)

    instan = ler_instancia(caminho)
    if instan:
        lista_instancias.append(instan) # Adicionando na lista correta
        print(f"Instância {i} lida com sucesso (Itens: {instan.qtd_itens}, Ótimo: {instan.lucro_otimo})")

# Imprimir os resultados
for instan in lista_instancias:
    print(f"\n=== Instância {instan.id_instancia} ===")
    print(f"Capacidade (W): {instan.peso_max} | Volume (V): {instan.volume_max} | Ótimo: {instan.lucro_otimo}")
    print("-" * 40)
    print(instan.itens.head()) # .head() retorna as 5 primeiras linhas