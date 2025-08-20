#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // Para sleep
#include <string.h>  // Para strerror

// Variables globales para el mutex, condition variable y señal
pthread_mutex_t mut;
pthread_cond_t cond;
int thread2_avanza = 0;  // Señal para indicar el avance del thread 2

void* function1(void *arg)
{
    int i = 0;

    printf("\n Hello, soy el thread 1 y me voy a dormir...\n");

    // Bloquea el mutex antes de esperar
    pthread_mutex_lock(&mut);

    // Espera hasta que thread2_avanza sea 3 (cuando el thread 2 ha completado 3 iteraciones)
    while (thread2_avanza < 3)  // Verifica la condición, espera si no se cumple
    {
        pthread_cond_wait(&cond, &mut);
    }

    // Al recibir la señal, desbloquea el mutex
    pthread_mutex_unlock(&mut);

    // Continúa después de ser despertado por el thread 2
    for (i = 0; i < 3; i++)
    {
        printf("\n Hello, soy el thread 1\n");
        sleep(1);
    }

    printf("\n T1 says: bye bye !\n");
    return NULL;
}

void* function2(void *arg)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        printf("\n Hello, soy el thread 2\n");
        sleep(1);

        // Después de 3 iteraciones, envía la señal y actualiza el contador
        if (i == 2)  // Aquí se cambia a i == 2 para asegurar que el thread 1 se despierte tras la tercera iteración
        {
            // Bloquea el mutex antes de enviar la señal
            pthread_mutex_lock(&mut);

            // Cambia el valor de thread2_avanza a 3
            thread2_avanza = 3;

            // Desbloquea el mutex después de enviar la señal
            pthread_cond_signal(&cond);

            pthread_mutex_unlock(&mut);
        }
    }

    // Desbloquea el mutex antes de terminar
    pthread_mutex_lock(&mut);
    pthread_cond_signal(&cond);  // Asegúrate de que se despierte el thread 1 si aún está esperando
    pthread_mutex_unlock(&mut);

    printf("\n T2 says: Hasta luego lucas !\n");
    return NULL;
}

int main(void)
{
    pthread_t t1_id, t2_id;

    // Inicializar el mutex y la condition variable
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mut, NULL);

    // Crear el thread 1
    int err = pthread_create(&t1_id, NULL, &function1, NULL);
    if (err != 0)
        printf("\nCan't create thread 1: [%d] - %s", err, strerror(err)); // Usa el código de error
    else
        printf("\n Thread 1 created successfully\n");

    // Crear el thread 2
    err = pthread_create(&t2_id, NULL, &function2, NULL);
    if (err != 0)
        printf("\nCan't create thread 2: [%d] - %s", err, strerror(err)); // Usa el código de error
    else
        printf("\n Thread 2 created successfully\n");

    // Esperar a que ambos threads terminen
    pthread_join(t1_id, NULL);
    pthread_join(t2_id, NULL);

    // Destruir el mutex y la condition variable
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mut);

    return 0;
}
