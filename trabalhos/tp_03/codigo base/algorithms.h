#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX

// Executa o algoritmo de Dijkstra a partir de um nó de origem
double dijkstra(int **matriz, int num_nos, int origem);

// Executa o algoritmo de Duan a partir de um nó de origem
double duan(int **matriz, int num_nos, int origem);

// Executa o terceiro algoritmo a partir de um nó de origem
double outro(int **matriz, int num_nos, int origem);

#endif
