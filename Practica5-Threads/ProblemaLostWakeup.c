#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mut;
pthread_cond_t cond1, cond2;
int wakeup1 = 0;  // Control variable for function1
int wakeup2 = 0;  // Control variable for function2

void pausa() {
    int pausa;
    pausa = 500 + (int)(1000 * (random() / (float)RAND_MAX));  // entre 1000 y 3000 microsegundos
    usleep(pausa * 1000);  // entre 1000 y 3000 millisegundos (1 a 3 segundos)
}

void* function1(void *arg) {
    pthread_t my_id = pthread_self();
    
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mut);

        // Wait until wakeup1 is set by function2
        while (!wakeup1) {
            pthread_cond_wait(&cond1, &mut);
        }
        wakeup1 = 0;  // Reset wakeup1 after consuming the signal
        pthread_mutex_unlock(&mut);

        printf("\nSoy el thread 1 (%ld) DENTRO en mi seccion critica...\n", my_id);
        pausa();

        pthread_mutex_lock(&mut);
        wakeup2 = 1;  // Signal to function2 that it can proceed
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mut);

        printf("\nSoy el thread 1 (%ld) FUERA en mi seccion critica...\n", my_id);
        pausa();
    }
    return NULL;
}

void* function2(void *arg) {
    pthread_t my_id = pthread_self();

    pthread_mutex_lock(&mut);
    wakeup1 = 1;  // Allow function1 to proceed initially
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mut);

    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mut);

        // Wait until wakeup2 is set by function1
        while (!wakeup2) {
            pthread_cond_wait(&cond2, &mut);
        }
        wakeup2 = 0;  // Reset wakeup2 after consuming the signal
        pthread_mutex_unlock(&mut);

        printf("\nSoy el thread 2 (%ld) DENTRO en mi seccion critica...\n", my_id);
        pausa();

        pthread_mutex_lock(&mut);
        wakeup1 = 1;  // Signal to function1 that it can proceed
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mut);

        printf("\nSoy el thread 2 (%ld) FUERA en mi seccion critica...\n", my_id);
        pausa();
    }
    return NULL;
}

int main(void) {
    pthread_t t1_id, t2_id;
    int err;

    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

    err = pthread_create(&t1_id, NULL, &function1, NULL);
    if (err != 0)
        printf("\nCan't create thread 1: [%s]", strerror(err));
    else
        printf("\nThread 1 created successfully\n");

    err = pthread_create(&t2_id, NULL, &function2, NULL);
    if (err != 0)
        printf("\nCan't create thread 2: [%s]", strerror(err));
    else
        printf("\nThread 2 created successfully\n");

    printf("\nSoy el proceso principal, espero a que los threads terminen...\n");

    pthread_join(t1_id, NULL);
    printf("\nT1 ha terminado\n");

    pthread_join(t2_id, NULL);
    printf("\nT2 ha terminado\n");

    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);

    return 0;
}

