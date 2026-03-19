#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "algoritmos.h"

/***********************************
ESTE ARQUIVO NÃO DEVE SER MODIFICADO
***********************************/

/**
 * Lê o arquivo fornecido e extrai dinamicamente todas as linhas para strings independentes.
 * 
 * @param filepath Caminho absoluto ou relativo do arquivo.
 * @param count    Ponteiro para salvar a quantidade total de linhas (strings) lidas.
 * @return         Retorna o vetor dinâmico de strings lido (char**), ou NULL em caso de erro.
 */
char **ler_arquivo(const char *filepath, size_t *count) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    size_t lines_capacity = 10;
    size_t lines_count = 0;
    
    // Alocando o vetor de ponteiros para as linhas (array de strings) com calloc
    char **lines = calloc(lines_capacity, sizeof(char *));
    if (lines == NULL) {
        perror("Erro de alocação de memória");
        fclose(file);
        return NULL;
    }

    int ch;
    do {
        size_t line_capacity = 64;
        size_t line_length = 0;
        
        // Alocando o vetor de caracteres da linha (string em si) com calloc
        char *buffer = calloc(line_capacity, sizeof(char));
        if (buffer == NULL) {
            perror("Erro de alocação de memória");
            goto cleanup;
        }

        // Lê a linha caractere por caractere
        while ((ch = fgetc(file)) != EOF && ch != '\n') {
            buffer[line_length++] = (char) ch;

            // Se atingir a capacidade, expande o vetor de caracteres com calloc
            if (line_length >= line_capacity - 1) { // Deixa espaço para o '\0'
                size_t new_capacity = line_capacity * 2;
                char *new_buffer = calloc(new_capacity, sizeof(char));
                if (new_buffer == NULL) {
                    perror("Erro de alocação de memória");
                    free(buffer);
                    goto cleanup;
                }
                
                // Copia os dados
                for (size_t i = 0; i < line_length; i++) {
                    new_buffer[i] = buffer[i];
                }
                free(buffer); // Libera o bloco anterior
                buffer = new_buffer;
                line_capacity = new_capacity;
            }
        }

        // Se encontrou o fim do arquivo sem caracteres, descartamos a alocação e paramos
        if (ch == EOF && line_length == 0) {
            free(buffer);
            break;
        }

        // Adiciona fechamento da string
        buffer[line_length] = '\0';

        // Expande o vetor de linhas se necessário
        if (lines_count >= lines_capacity) {
            size_t new_capacity = lines_capacity * 2;
            char **new_lines = calloc(new_capacity, sizeof(char *));
            if (new_lines == NULL) {
                perror("Erro de alocação de memória");
                free(buffer);
                goto cleanup;
            }
            
            for (size_t i = 0; i < lines_count; i++) {
                new_lines[i] = lines[i];
            }
            free(lines); // Libera a estrutura de ponteiros anterior
            lines = new_lines;
            lines_capacity = new_capacity;
        }

        // Armazena a string lida
        lines[lines_count++] = buffer;

    } while (ch != EOF);

    fclose(file);

    // Retorna a contagem de linhas efetivamente lidas
    if (count != NULL) {
        *count = lines_count;
    }

    return lines;

cleanup:
    // Limpeza completa de recursos e encerramento seguro sob falha de alocação
    if (file) fclose(file);
    if (lines) {
        for (size_t i = 0; i < lines_count; i++) {
            free(lines[i]);
        }
        free(lines);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    // Recebe o caminho do arquivo como argumento da main
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <caminho_do_arquivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t count = 0;
    
    // Extraímos as strings do arquivo usando a função
    // Esta função NÃO PODE SER MODIFICADA DE MANEIRA ALGUMA
    char **strings = ler_arquivo(argv[1], &count);
    
    // Tratativa de erro na leitura ou no caminho passado
    if (strings == NULL) {
        return EXIT_FAILURE;
    }

    // Separa as strings lidas em s1 e s2 conforme descrição do trabalho
    char *s1 = strings[0];
    char *s2 = strings[1];

    char *s1_copy = s1;
    char *s2_copy = s2;

    struct timeval start, end;
    double tempo_dinamico, tempo_guloso;

    // Medição do tempo para Programacao Dinamica
    gettimeofday(&start, NULL);
    long int resultado_dinamico = programacao_dinamica(s1_copy, s2_copy);
    gettimeofday(&end, NULL);
    tempo_dinamico = (end.tv_sec - start.tv_sec) * 1e6;
    tempo_dinamico = (tempo_dinamico + (end.tv_usec - start.tv_usec)) * 1e-6;
    printf("Programação Dinâmica: \nResultado: %ld \nTempo: %f \n\n", resultado_dinamico, tempo_dinamico);

    // Medição do tempo para Guloso
    gettimeofday(&start, NULL);
    long int resultado_guloso = guloso(s1, s2);
    gettimeofday(&end, NULL);
    tempo_guloso = (end.tv_sec - start.tv_sec) * 1e6;
    tempo_guloso = (tempo_guloso + (end.tv_usec - start.tv_usec)) * 1e-6;
    printf("Guloso: \nResultado: %ld \nTempo: %f \n", resultado_guloso, tempo_guloso);

    // Liberação da memória das alocações dinâmicas após o uso em 'main'
    for (size_t i = 0; i < count; i++) {
        free(strings[i]);
    }
    free(strings);

    return EXIT_SUCCESS;
}
