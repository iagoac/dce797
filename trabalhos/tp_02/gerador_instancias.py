import random
import sys

def generate_dna_sequences(size1, size2, filename="dna_input.txt"):
    bases = ['A', 'C', 'G', 'T']
    s1 = ''.join(random.choice(bases) for _ in range(size1))
    s2 = ''.join(random.choice(bases) for _ in range(size2))
    
    with open(filename, 'w') as f:
        f.write(f"{s1}\n{s2}")
    print(f"Instância gerada com sucesso em {filename} (Tamanhos: {size1}, {size2})")

if __name__ == "__main__":
    # Exemplo de uso: python gen.py 100 100
    if len(sys.argv) > 2:
        generate_dna_sequences(int(sys.argv[1]), int(sys.argv[1]), sys.argv[2])
    else:
        print("Uso: python gen.py <tamanho> <arquivo_saída>")