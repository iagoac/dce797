#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algorithms.h"

// Estrutura para representar o grafo usando uma matriz de adjacência
typedef struct {
    int num_nos;
    int num_arestas;
    int **matriz;
} Grafo;

// Função para ler o grafo de um arquivo para uma matriz de adjacência
Grafo* ler_grafo(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    if (!grafo) {
        perror("Falha na alocação de memória");
        fclose(arquivo);
        return NULL;
    }

    // Lê o número de nós e arestas
    if (fscanf(arquivo, "%d %d", &grafo->num_nos, &grafo->num_arestas) != 2) {
        fprintf(stderr, "Erro ao ler o número de nós e arestas\n");
        free(grafo);
        fclose(arquivo);
        return NULL;
    }

    // Aloca memória para a matriz de adjacência (num_nos x num_nos)
    grafo->matriz = (int **)malloc(grafo->num_nos * sizeof(int *));
    if (!grafo->matriz) {
        perror("Falha na alocação de memória para as linhas da matriz");
        free(grafo);
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < grafo->num_nos; i++) {
        // calloc inicializa a memória com 0 (que usamos para representar a ausência de aresta)
        grafo->matriz[i] = (int *)calloc(grafo->num_nos, sizeof(int));
        if (!grafo->matriz[i]) {
            perror("Falha na alocação de memória para as colunas da matriz");
            // Libera as linhas alocadas anteriormente para evitar vazamento de memória
            for (int j = 0; j < i; j++) {
                free(grafo->matriz[j]);
            }
            free(grafo->matriz);
            free(grafo);
            fclose(arquivo);
            return NULL;
        }
    }

    // Lê cada aresta e preenche a matriz
    for (int i = 0; i < grafo->num_arestas; i++) {
        int u, v, custo;
        if (fscanf(arquivo, "%d %d %d", &u, &v, &custo) != 3) {
            fprintf(stderr, "Erro ao ler a aresta %d\n", i + 1);
            // Libera a memória antes de retornar
            for (int j = 0; j < grafo->num_nos; j++) {
                free(grafo->matriz[j]);
            }
            free(grafo->matriz);
            free(grafo);
            fclose(arquivo);
            return NULL;
        }
        
        // Assumindo que os nós são indexados a partir de 0 e o grafo é não direcionado
        grafo->matriz[u][v] = custo;
        grafo->matriz[v][u] = custo;
    }

    fclose(arquivo);
    return grafo;
}

// Função para liberar a memória do grafo
void liberar_grafo(Grafo *grafo) {
    if (grafo) {
        if (grafo->matriz) {
            for (int i = 0; i < grafo->num_nos; i++) {
                free(grafo->matriz[i]);
            }
            free(grafo->matriz);
        }
        free(grafo);
    }
}

int main(int argc, char *argv[]) {
    // Usa o nome de arquivo fornecido ou o padrão ../exemplo.dat
    const char *nome_arquivo = (argc > 1) ? argv[1] : "instancia_exemplo.dat";

    Grafo *grafo = ler_grafo(nome_arquivo);
    
    if (!grafo) {
        return EXIT_FAILURE;
    }

    double cost;
    clock_t inicio, fim;
    double tempo_gasto;

    inicio = clock();
    cost = dijkstra(grafo->matriz, grafo->num_nos, 0);
    fim = clock();
    
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Dijkstra: %f %f\n", cost,tempo_gasto);

    inicio = clock();
    cost = duan(grafo->matriz, grafo->num_nos, 0);
    fim = clock();
    
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Duan: %f %f\n", cost,tempo_gasto);

    inicio = clock();
    cost = outro(grafo->matriz, grafo->num_nos, 0);
    fim = clock();
    
    tempo_gasto = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Outro: %f %f\n", cost,tempo_gasto);

    // Limpeza
    liberar_grafo(grafo);
    
    return EXIT_SUCCESS;
}
