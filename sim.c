#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    int pagina_virtual;
    int ocupado;
    int uso;
} Marco;

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

    Marco *marcos = malloc(n_marcos * sizeof(Marco));
    if (marcos == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para los marcos.\n");
        return 1;
    }
    else {
        for (int i = 0; i < n_marcos; i++) {
            marcos[i].pagina_virtual = -1; // -1 indica que el marco está libre
            marcos[i].ocupado = 0;
            marcos[i].uso = 0;
        }
    }

    char buffer[100];
    int temp = tam_marco;
    int b = 0;
    int reloj = 0;
    int page_faults = 0;
    int iterations = 0;

    while (temp >>= 1) {
        b++;
    }
    unsigned MASK = (1U << b) - 1;

    unsigned int dv;
    unsigned int offset;
    unsigned int nvp;
    unsigned int DF;
    int marco_hit;
    int marco_libre;
    int marco_asignado;

    printf("Leyendo archivo de traza...\n");

    while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        printf("Procesando línea: %s\n", buffer);
        iterations++;
        buffer[strcspn(buffer, "\n")] = 0; 
        
        dv = (unsigned int)strtol(buffer, NULL, 0);
        offset = dv & MASK;
        nvp = dv >> b;

        marco_hit = -1;
        for(int i = 0; i < n_marcos; i++) {
            if(marcos[i].ocupado && marcos[i].pagina_virtual == nvp) {
                marco_hit = i;
                break;
            }
        }

        if (marco_hit != -1) {
            marcos[marco_hit].uso = 1;
            DF = (marco_hit << b) | offset;
            if (verbose) {
                printf("HIT: nvp %u en marco %d, DF=0x%X\n\n", nvp, marco_hit, DF);
            }
        } else {
            page_faults++;
            marco_libre = -1;

            for(int i = 0; i < n_marcos; i++) {
                if(marcos[i].ocupado == 0) {
                    marco_libre = i;
                    break;
                }
            }

            if (marco_libre != -1) {
                marco_asignado = marco_libre;
            } else {
                while(1) {
                    if(marcos[reloj].uso == 0) {
                        marco_asignado = reloj;
                        break;
                    } else {
                        marcos[reloj].uso = 0;
                        reloj = (reloj + 1) % n_marcos;
                    }
                }
            }

            marcos[marco_asignado].pagina_virtual = nvp;
            marcos[marco_asignado].ocupado = 1;
            marcos[marco_asignado].uso = 1;
            reloj = (marco_asignado + 1) % n_marcos;
            DF = (marco_asignado << b) | offset;

            if (verbose) {
                printf("FALLO: nvp %u asignada a marco %d, DF=0x%X\n\n", nvp, marco_asignado, DF);
            }
        }

        if (verbose) {
            printf("  Dirección Virtual (DV): 0x%X\n", dv);
            printf("  Dirección Física (DF): 0x%X\n", DF);
            printf("  nvp (Página virtual): %u\n", nvp);
            printf("  offset: %u\n\n", offset);
            printf("-----------------------------\n\n");
        }
    }

    printf("Total de fallos de página: %d\n", page_faults);

    if (iterations > 0) {
        printf("Total de referencias de página: %d\n", iterations);
        printf("Tasa de fallos de página: %.2f%%\n\n", ((double) page_faults / iterations) * 100);
    } else {
        printf("No se procesaron referencias de página.\n\n");
    }

    printf("Simulación finalizada correctamente.\n");
    fclose(archivo);
    free(marcos);
    return 0;
}