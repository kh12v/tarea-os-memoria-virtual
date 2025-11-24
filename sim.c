#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        fprintf(stderr, "Error: Número incorrecto de argumentos.\n");
        printf("Uso: ./sim <Nmarcos> <tamañomarco> [--verbose] <traza.txt>\n");
        printf("Ejemplos:\n");
        printf("  ./sim 16 256 traza.txt\n");
        printf("  ./sim 4 1024 --verbose traza.txt\n");
        return 1;
    }

    int n_marcos = atoi(argv[1]);
    int tam_marco = atoi(argv[2]);
    
    if (n_marcos <= 0 || tam_marco <= 0) {
        fprintf(stderr, "Error: Nmarcos y tamañomarco deben ser enteros positivos.\n");
        return 1;
    }

    int verbose = 0;
    char *nombre_archivo = NULL;

    if (argc == 5) {
        if (strcmp(argv[3], "--verbose") == 0) {
            verbose = 1;
            nombre_archivo = argv[4];
        } else if (strcmp(argv[4], "--verbose") == 0) {
            verbose = 1;
            nombre_archivo = argv[3];
        } else {
            fprintf(stderr, "Error: Argumento opcional desconocido '%s'.\n", argv[3]);
            printf("Uso: ./sim <Nmarcos> <tamañomarco> [--verbose] <traza.txt>\n");
            printf("Ejemplos:\n");
            printf("  ./sim 16 256 traza.txt\n");
            printf("  ./sim 4 1024 --verbose traza.txt\n");
            return 1;
        }
    } else {
        verbose = 0;
        nombre_archivo = argv[3];
    }

    if (verbose) {
        printf("--- Configuración Inicial ---\n");
        printf("Marcos: %d\n", n_marcos);
        printf("Tamaño de Marco: %d\n", tam_marco);
        printf("Modo Verbose: ACTIVADO\n");
        printf("Archivo de traza: %s\n", nombre_archivo);
        printf("-----------------------------\n");
    }

    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        fprintf(stderr, "Error: No se pudo abrir el archivo '%s'.\n", nombre_archivo);
        return 1;
    }

    printf("Leyendo archivo de traza...\n");
    
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        if (verbose) {
            buffer[strcspn(buffer, "\n")] = 0; 
            printf("Procesando línea: %s\n", buffer);
        }
    }

    printf("Simulación finalizada correctamente.\n");

    fclose(archivo);
    return 0;
}