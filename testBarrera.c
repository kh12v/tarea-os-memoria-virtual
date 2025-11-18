#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// --- Recursos Compartidos ---
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;

// La "condición" que comprobamos
int toFinish = 3;
// ----------------------------

void* work(void* arg) {
    printf("Working...\n");
    sleep(2);
    
    // Adquirir el mutex para modificar los datos
    pthread_mutex_lock(&mutex);

    // --- Sección Crítica ---
    toFinish--;
    printf("Works to be done: %d.\n", toFinish);

    while (toFinish > 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    
    // Despertar a un hilo que esté esperando en 'cond'
    pthread_cond_signal(&cond);
    
    // Liberar el mutex
    pthread_mutex_unlock(&mutex);

    printf("Work done\n");
    
    return NULL;
}

int main() {
    pthread_t tid0, tid1, tid2;

    // Crear hilos
    pthread_create(&tid0, NULL, work, NULL);
    sleep(1);
    pthread_create(&tid1, NULL, work, NULL);
    sleep(1);
    pthread_create(&tid2, NULL, work, NULL);

    // Esperar a que terminen
    pthread_join(tid0, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("All the works have been done.\n");
    return 0;
}