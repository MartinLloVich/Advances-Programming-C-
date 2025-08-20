#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include "common.h"

#define BUFFER_SIZE 5  // Tamaño del buffer

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_data;

int consumir_item(shared_data *data) {
    int item = data->buffer[data->out];
    data->out = (data->out + 1) % BUFFER_SIZE;
    printf("Consumidor ha consumido: %d\n", item);
    return item;
}

int main() {
    int semid, shmid;
    struct sembuf operacion;
    shared_data *data;

    // Crear/obtener semáforos
    semid = semget(CLAVE, 3, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Crear/obtener memoria compartida
    shmid = shmget(CLAVE, sizeof(shared_data), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Adjuntar la memoria compartida
    data = (shared_data *)shmat(shmid, NULL, 0);
    if (data == (void *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // Esperar si no hay ítems en el buffer
        operacion.sem_num = FULL;
        operacion.sem_op = -1;
        operacion.sem_flg = 0;
        semop(semid, &operacion, 1);  // Decrementa FULL

        // Obtener exclusión mutua
        operacion.sem_num = MUTEX;
        operacion.sem_op = -1;
        semop(semid, &operacion, 1);  // Decrementa MUTEX

        // Consumir el ítem
        int item = consumir_item(data);

        // Liberar exclusión mutua
        operacion.sem_num = MUTEX;
        operacion.sem_op = 1;
        semop(semid, &operacion, 1);  // Incrementa MUTEX

        // Señalar que ahora hay un espacio libre en el buffer
        operacion.sem_num = EMPTY;
        operacion.sem_op = 1;
        semop(semid, &operacion, 1);  // Incrementa EMPTY

        sleep(2);  // Pausa entre consumiciones
    }

    return 0;
}

