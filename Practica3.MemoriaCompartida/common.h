// common.h
#ifndef COMMON_H
#define COMMON_H

#define CLAVE_SHM 1234 // Clave para shmget

typedef struct {
    int turno;           // Controla el turno de acceso (1 para proceso 1, 2 para proceso 2)
    int intento;         // Número propuesto por el Proceso 1
    char respuesta[20];  // Respuesta del Proceso 2 ("mayor", "menor" o "acertado")
} datos;

#endif

