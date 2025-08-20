#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
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

    int min = 0, max = 100, intento;

    while (1) {
        // Realizar intento utilizando búsqueda binaria
        intento = (min + max) / 2;
        sprintf(mensaje.mtext, "%d", intento);
        mensaje.mtype = 1; // Intento del receptor

        // Enviar intento al productor
        ret = msgsnd(id_cola, &mensaje, sizeof(mensaje.mtext), 0);
        if (ret == -1) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        // Mostrar intento enviado
        printf("Intento enviado: %d\n", intento); 

        // Recibir respuesta del productor (mtype = 2)
        ret = msgrcv(id_cola, &mensaje, sizeof(mensaje.mtext), 2, 0);
        if (ret == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Verificar respuesta
        printf("Respuesta recibida: %s\n", mensaje.mtext); 

        // Ajustar límites según la respuesta
        if (strcmp(mensaje.mtext, "Acertaste") == 0) {
            printf("El número ha sido adivinado correctamente!\n"); // Mensaje de éxito
            break; // Salir si se adivinó el número
        } else if (strcmp(mensaje.mtext, "Mayor") == 0) {
            min = intento + 1; // Ajustar el límite inferior
        } else if (strcmp(mensaje.mtext, "Menor") == 0) {
            max = intento - 1; // Ajustar el límite superior
        }
    }

    // Eliminar la cola antes de terminar
    msgctl(id_cola, IPC_RMID, NULL);
    return 0;
}
