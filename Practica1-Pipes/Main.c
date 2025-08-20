#include <stdio.h> //Libreria que proporciona las funciones printf y sprintf
#include <stdlib.h> //Libreria que proporciona las funciones atoi, srandom, random y exit
#include <sys/types.h> //Libreria que proporciona la funcion pid_t
#include <sys/wait.h> //Libreria que proporciona la funcion wait
#include <unistd.h> //Libreria que proporciona las funciones fork, pipe, y read/write
#include <string.h> //Libreria que proporciona la funcion strcmp

#define BUFFER_SIZE 100 //Defino el valor del tamaño de la info que se enviará entre padre e hijo

int main(void) {
    int fd1[2];  // Pipe de padre a hijo
    int fd2[2];  // Pipe de hijo a padre
    int status; //Variable para guardar el estado de terminación del proceso hijo
    char buffer[BUFFER_SIZE]; //Variable donde se gurda la info que va de padre a hijo, de tamaño BUFFER_SIZE (100)
    int min = 0, max = 256; //Valores mínimo y máximo que puede valer el número a adivinar (dado por el problema)

    // Creación de pipes
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
     printf("Pipe OK!\n"); //Las pipes se han creado sin problema

    // Creación del proceso hijo y padre
    switch (fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);

        case 0:  // Creacion del proceso hijo
            // Cierre de escritura de la pipe fd1 (padre a hijo)
            if (close(fd1[1]) == -1)
                perror("close");

            // Cierre de lectura de la pipe fd2 (hijo a padre)
            if (close(fd2[0]) == -1)
                perror("close");

            // Generación de número aleatorio
            int intento; //Variable para guardar el número que ha dicho el padre (correcto o no)
            int numero_adivinar; //Variable para guardar el numero secreto
            srandom(getpid()); //Se crea un número aleatorio a partir de la semilla del PID para que no se repita cada vez que se ejecuta el sistema
            numero_adivinar = (int)(256.0 * random() / RAND_MAX); // Se crea el numero secreto, de forma que el número sea menor a 256
            printf("Número a adivinar: %d\n", numero_adivinar); //Se dice por pantalla el número

            // Proceso hijo: recibe intentos del padre y responde si es mayor, menor o acertó
            while (1) {
                // Lectura de intento desde la pipe fd1
                int nread = read(fd1[0], buffer, sizeof(buffer));
                if (nread == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                buffer[nread] = '\0'; //Añade final de cadena

                intento = atoi(buffer);  // Convertir intento a entero
                if (intento == numero_adivinar) { //Comparamos si el numero es el correcto
                    write(fd2[1], "¡Acertaste!", 12);
                    break; //Si se adivina, salimos del bucle del pipe hijo
                } else if (intento < numero_adivinar) {
                    write(fd2[1], "Mayor", 6);
                } else {
                    write(fd2[1], "Menor", 6);
                }
            }

            // Cerrar pipes en el hijo
            close(fd1[0]);
            close(fd2[1]);
            exit(EXIT_SUCCESS);

        default:  // Proceso padre
            // Cierre del descriptor de lectura de la pipe fd1
            if (close(fd1[0]) == -1)
                perror("close");

            // Cierre del descriptor de escritura de la pipe fd2
            if (close(fd2[1]) == -1)
                perror("close");

            // Intentos del padre utilizando búsqueda binaria
            while (1) {
                int intento; //Variable para guardar el número que ha dicho el padre (correcto o no)
                intento = (min + max) / 2; //El padre en al busqueda binaria empieza por el medio del rango, y va cambiando segun la info dada por el hijo 
                sprintf(buffer, "%d", intento);  // Convertir intento a cadena
                write(fd1[1], buffer, sizeof(buffer));  // Enviar intento al hijo

                // Leer respuesta del hijo
                int nread = read(fd2[0], buffer, sizeof(buffer));
                if (nread == -1) {
                    perror("read");
                    exit(EXIT_FAILURE);
                }
                printf("Intento: %d - Hijo: %s\n", intento, buffer);

                // Ajustar el rango en función de la respuesta del hijo
                if (strcmp(buffer, "¡Acertaste!") == 0) {
                    break; //Si el numero adivinado es correcto salimos del bucle
                } else if (strcmp(buffer, "Mayor") == 0) {
                    min = intento + 1; //Si el numero es mayor, se suma 1 al limite inferior para subir el rango la busqueda (busqueda binaria)
                } else if (strcmp(buffer, "Menor") == 0) {
                    max = intento - 1; //Si el numero es menor, se resta 1 al limite superior para bajar el rango la busqueda (busqueda binaria)
                }
            }

            // Cerrar pipes en el padre
            close(fd1[1]);
            close(fd2[0]);

            // Esperar la finalización del hijo
            wait(&status);
            exit(EXIT_SUCCESS);
    }
}
