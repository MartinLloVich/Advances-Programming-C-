//Se incluyen las librerias
#include <stdio.h>
#include <stdlib.h>



// Se define la estructura para el arbol
typedef struct nodo {
    int info;
    struct nodo *left, *right;
} tnode, *ptree;



// Se declaran las funciones
ptree vec2tree(ptree t, int *v, int n); //De vector a arbol
void tree2vect(ptree t, int *v, int *pos); //De arbol a vector
ptree insorder(ptree t, int x); //Se ordena el arbol
void destruir(ptree *t); //Se detruye el arbol
void sort(int *v, int n); //Funcion sort



//En el main pediremos al usuario el tamaño del vector y que indique los numeros
int main() {
    int n;

    // Se solicita el tamaño del vector al usuario
    printf("Ingrese el tamaño del vector: ");
    scanf("%d", &n);

    //Se asocia el espacio de memoria
    int *v = (int *)malloc(n * sizeof(int));
    if (!v) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Se solicitan los elementos del vector
    printf("Ingrese %d elementos del vector:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Elemento %d: ", i + 1);
        scanf("%d", &v[i]);
    }

    // Se imprime el vector original
    printf("Vector original:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    // Se ordena el vector usando sort
    sort(v, n);

    // Se imprime el vector ordenado
    printf("Vector ordenado:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");

    // Libera la memoria del vector
    free(v);
    return 0;
}



// Implementación de tree2vect (de las transparencias)
void tree2vect(ptree t, int *v, int *pos) {
    if (!t) return;

    //rellena con el sub-arbol izquierdo
    if (t->left) tree2vect(t->left, v, pos);
    v[*pos] = t->info;                       
    (*pos)++;

    //rellena con el sub-arbol derecho
    if (t->right) tree2vect(t->right, v, pos);
}



// Implementacion de vec2tree (de las transparencias)
ptree vec2tree(ptree t, int *v, int n) {
    for (int i = 0; i < n; i++) {
        t = insorder(t, v[i]);
    }
    return t;
}



// Implementación de insorder (de las transparencias)
ptree insorder(ptree t, int x) {
    ptree tmp;
    if (!t) {
        tmp = (tnode *)malloc(sizeof(tnode));
        tmp->info = x;
        tmp->left = NULL;
        tmp->right = NULL;
        return tmp;
    }
    if (x > (t->info)) {
        t->right = insorder(t->right, x);
        return t;
    } else {
        t->left = insorder(t->left, x);
        return t;
    }
}



// Implementación de destruir
void destruir(ptree *t) {
    if (!(*t)) return;

    // Subarbol izquierdo
    destruir(&(*t)->left); 

    // Subárbol derecho
    destruir(&(*t)->right);

    // Nodo actual
    free(*t);             
    *t = NULL;
}



// Implementación de sort (de las transparencias)
void sort(int *v, int n) {
    ptree t = NULL;
    int pos = 0;

    t = vec2tree(t, v, n);     
    tree2vect(t, v, &pos); 
    
    destruir(&t);              
}
