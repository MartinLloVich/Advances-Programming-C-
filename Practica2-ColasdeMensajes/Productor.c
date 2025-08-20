#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define CLAVE_COLA 1

struct msgbuf {
    long mtype;       // Tipo de mensaje
    char mtext[15];   // Mensaje
};

// Función para crear la cola de mensajes
int crearcola(int key) {
    int msgqid = msgget(key, IPC_CREAT | 0666);
    if (msgqid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    return msgqid;
}

int main(void) {
    int id_cola, ret;
    struct msgbuf mensaje;

    // Crear cola de mensajes
    id_cola = crearcola(CLAVE_COLA);

    // Generar número aleatorio entre 0 y 100
    srand(time(NULL));
    int numero_secreto = rand() % 101;

    printf("Número secreto generado (para debug): %d\n", numero_secreto); // Solo para debug

    while (1) {
        // Recibir intento del receptor (mtype = 1)
        ret = msgrcv(id_cola, &mensaje, sizeof(mensaje.mtext), 1, 0);
        if (ret == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Convertir el intento a número
        int intento = atoi(mensaje.mtext);
        printf("Intento recibido: %d\n", intento); // Mostrar intento recibido

        // Preparar la respuesta
        mensaje.mtype = 2; // Respuesta del productor
        if (intento < numero_secreto) {
            strcpy(mensaje.mtext, "Mayor");
        } else if (intento > numero_secreto) {
            strcpy(mensaje.mtext, "Menor");
        } else {
            strcpy(mensaje.mtext, "Acertaste");
            // Enviar mensaje de acierto y salir del bucle
            msgsnd(id_cola, &mensaje, sizeof(mensaje.mtext), 0);
            printf("El receptor ha acertado el número!\n"); // Mensaje de acierto
            break;
        }

        // Enviar respuesta al receptor
        ret = msgsnd(id_cola, &mensaje, sizeof(mensaje.mtext), 0);
        if (ret == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    // Eliminar la cola antes de terminar
    msgctl(id_cola, IPC_RMID, NULL);
    return 0;
}
