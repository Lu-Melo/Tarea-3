#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include "tdas/stack.h"
#include "tdas/queue.h"
#include <string.h>
#include <time.h>

//hoila

// Definición de la estructura para el estado del puzzle
typedef struct {
    int maze[N][N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) {
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

void mostrarGlosario(){ // funcion para mostrar el glosario del laberinto quehasta yo me perdí muchas veces
    puts("===================================");
    puts("      Como leer el laberinto       ");
    puts("===================================");

    puts("  'I'  = (letra I mayuscula) Inicio");
    puts("  'M'  = (letra M mayuscula) Meta");
    puts("  '#'  = (Simbolo de hashtag/numeral) Camino encontrado por el algoritmo de busqueda");
    puts(" ' . ' = (Simbolo de punto) Espacios libres");
    puts(" ' ■ ' = (Simbolo de bloque solido) Espacios con obstaculos");
}

// Función para imprimir el estado actual
void imprimirEstado(const State *state) {
    
    int x_aux = 0; 
    int y_aux = 0;
    int caminito[N][N] = {0};
    int *action = list_first(state->actions);
    
    while(action != NULL) {
        if(*action == 1){ // ARRIBA: entonces se le resta 1 a la fila
            x_aux--;
        } else if(*action == 2) { // ABAJO: entonces se le suma 1 a la fila
            x_aux++;
        } else if(*action == 3) { // IZQUIERDA: entonces se le resta 1 a la columna
            y_aux--;
        }
        else if(*action == 4) { // DERECHA: entonces se le suma 1 a la columna
            y_aux++;
        }
        caminito[x_aux][y_aux] = -1;
        action = list_next(state->actions);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (state->x==i && state->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (caminito[i][j] == -1) printf(" # ");
            else if (state->maze[i][j] == 0) printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            
            else printf(" ■ ");
        }
        printf("\n");
    }
    if(state->steps != 0) {
        printf("Cantidad de pasos = %i\n", state->steps);
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
     // Copiar el laberinto generado al estado
    generate_maze(estado.maze,  dificultad);
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

bool is_final_state(State *state){
    if(state->x == N - 1 && state->y == N - 1) return true; // esto revisa si ya llegó a las coordenadas de la meta
    return false;
}

State *transition(State *state, int action){ // funcion revisa si el estado puede ir en esa direccion y devuelve ese estado avanzando en esa direccion
    // arriba=1, abajo=2, izquierda=3, derecha=4
    /* guia visual borrar dsp:
    [11][12][13]      
    [21][22][23]
    [31][32][33]
    */
    // N = 10 por si acaso esta en el extra.h borrar dsp
    int x_aux = state->x; // crea una copia de las coords a las que se va a mover
    int y_aux = state->y;
    
    if(action == 1){ // ARRIBA: entonces se le resta 1 a la fila
        x_aux--;
    } else if(action == 2) { // ABAJO: entonces se le suma 1 a la fila
        x_aux++;
    } else if(action == 3) { // IZQUIERDA: entonces se le resta 1 a la columna
        y_aux--;
    }
    else if(action == 4) { // DERECHA: entonces se le suma 1 a la columna
        y_aux++;
    }

    if(x_aux < 0 || x_aux >= N || y_aux < 0 || y_aux >= N) { // si se pasa del laberinto en la nueva posicion no se puede y retorna
        return NULL;
    }
    if(state->maze[x_aux][y_aux] == 1) { // si hay un obstaculo en la nueva posicion tampoco se puede y retorna
        return NULL;
    }
    State *estado_aux = (State*) malloc(sizeof(State)); // Si se pudo entonces, crea state auxiliar para guardar los valores del estado a avanzar
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            estado_aux->maze[i][j] = state->maze[i][j];
        }
    }   
    estado_aux->x = x_aux; // se copian las coordenadas validas al nuevo estado
    estado_aux->y = y_aux;
    estado_aux->steps = (state->steps + 1); // se le suma 1 a los pasos
    estado_aux->actions = list_create(); // se crea una nueva lista de acciones para el nuevo estado

    int *action_primero = list_first(state->actions); // recorre la antigua lista y le agrega las acciones anteriores a la nueva lista del estado nuevo
    while(action_primero != NULL) {
        int *action_aux = (int*) malloc(sizeof(int));
        *action_aux = *action_primero;
        list_pushBack(estado_aux->actions, action_aux);
        action_primero = list_next(state->actions);
    }
    int *action_ahora = (int*) malloc(sizeof(int)); // crea la copia como puntero para tirarla a la lista de nodos adjacentes y que no se borre cuando se acabe la funcion (caché a la mala que pasaba)
    *action_ahora = action;
    list_pushBack(estado_aux->actions, action_ahora); // le agrega la nueva accion a la lista de acciones
    return estado_aux; // retorna el nuevo estadeins
}

List *get_adj_nodes(State *state) { // funciolini que te da los nodos adjacentes al estado
    List * vecinitos = list_create();
    for(int i = 1; i < 5; i++) { // revisa 4 veces pq solo hay cuatro direcciones arriba, abajo, izquierda, derecha
        State *adj_aux = transition(state, i); // obtiene si se puede mover o si es NULL y no se puede
        if(adj_aux != NULL) {
            list_pushBack(vecinitos, adj_aux); 
        }
    }
    return vecinitos; // retorna el vecino
}

bool is_visited(int maze_aux[N][N], State* state){
    if(maze_aux[state->x][state->y] == 1) { // en un laberinto auxiliar, en las coordenadas del estado, revisa si hay un 1 (si se visitó)
        return true; 
    } else {
        return false;
    }
}

void visit(int maze_aux[N][N], State* state){
    maze_aux[state->x][state->y] = 1; // en un laberinto auxiliar, en las coordenadas del estado, le pone un 1 (para saber si se visitó)
}

void deep_first_search(State* state, int *iteraciones) { // busqueda por profundidad
    //PILAAA
    Stack *Duracell = stack_create(Duracell); // aca crea una pila y le pone el estado
    stack_push(Duracell, state);

    int maze_aux[N][N] = {0}; // crea un laberinto "vacío", con puros 0 como valores con el mismo tamaño que el original
    
    while(stack_top(Duracell) != NULL) { // itera mientras la pila tenga cositas
        State *aux = stack_pop(Duracell); // revisa el elemento de arriba de la pila
        if(is_visited(maze_aux, aux)) continue; // si se visitó ese estado entonces se sigue con otro estado
        visit(maze_aux, aux); // si no se visitó se visita
        (*iteraciones)++; // aumenta en uno el contador de iteraciones
    
        if(is_final_state(aux)) { // si el estado está en la meta se termino el camineins
            puts("===================================");
            puts("     Busqueda por Profundidad      ");
            puts("===================================");
            imprimirEstado(aux); // se imprime el estado y toda la paranoia
            return;
        }
        List *vecinitos = get_adj_nodes(aux); // esto crea la lista de vecinos del estado
        State *adj_aux = list_first(vecinitos);
        while(adj_aux != NULL) { // le agrega todos los vecinos a la pila para revisar despues esos vecinos
            stack_push(Duracell, adj_aux);
            adj_aux = list_next(vecinitos);
        }
    }
    printf("No se encontro ningun camino disponible\n"); // si nunca se llegó a la meta no funcionó la busqueda
    return;
}

void breadth_first_search(State* state, int* iteraciones){ // aca hace lo mismo que arriba pero con una cola en vez de una pila, es lo mismo mismo
    //COLAA
    Queue *Cola = queue_create(Cola);
    queue_insert(Cola, state);

    int maze_aux[N][N] = {0};

    while(queue_front(Cola) != NULL) {
        State *aux = queue_remove(Cola);
        if(is_visited(maze_aux, aux)) continue;
        visit(maze_aux, aux);
        (*iteraciones)++;

        if(is_final_state(aux)) {
            puts("===================================");
            puts("       Busqueda por Anchura        ");
            puts("===================================");
            imprimirEstado(aux);
            return;
        }
        List *vecinitos = get_adj_nodes(aux);
        State *adj_aux = list_first(vecinitos);
        while(adj_aux != NULL) {
            queue_insert(Cola, adj_aux);
            adj_aux = list_next(vecinitos);
        }
    }
    printf("No se encontro ningun camino disponible\n");
    return;
}

void busqueda_Astar(State* state, int* iteraciones){ //aca hace lo mismo que las de arriba pero con un heap
    //HEAPPP
    Heap *heap = heap_create(heap); // se crea el heap
    heap_push(heap, state, ( state->steps + distancia_L1(state) ) * -1); // se le pone un elemento con prioridad de la distancia y los pasos que lleva, en negativo porque queremos la menor distancia total

    int maze_aux[N][N] = {0};

    while(heap_top(heap) != NULL) {
        State *aux = heap_top(heap); // se revisa el elemento de arriba del heap
        heap_pop(heap); // y se quita para despues seguir revisando los otros
        if(is_visited(maze_aux, aux)) continue; // si ya se visito se sigue con el siguiente elemento del heap
        visit(maze_aux, aux); // si no, se visita
        (*iteraciones)++; // se suma una iteracion

        if(is_final_state(aux)) { // si el estado esta en las coords de la meta retorna porque si se pudo encontrar el caminito
            puts("===================================");
            puts("          Busqueda por A*          ");
            puts("===================================");
            imprimirEstado(aux); // imprime el estado y toda la movida esa tio
            return;
        }
        List *vecinitos = get_adj_nodes(aux); // aca crea una lista de vecinos adjacentes al estado
        State *adj_aux = list_first(vecinitos);
        while(adj_aux != NULL) {
            heap_push(heap, adj_aux, ( adj_aux->steps + distancia_L1(adj_aux) ) * -1); // pone los vecinos con prioridad de quien tiene menos distancia de la meta y ocupe menos pasos para llegar, para que revise los que tienen menos distancia total primero
            adj_aux = list_next(vecinitos); // sigue recorriendo la lista de vecinitos
        }
    }
    printf("No se encontro ningun camino disponible\n"); // si nunca llegó a la meta no se pudo encontrar solucion pipipi
    
    return;
}

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));

    int maze[N][N];
    int dificultad;

    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);

    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);

    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);

    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));

    char opcion;
    do {
        // aca muestra el menu con las opciones varias
        puts("===================================");
        puts("     Escoge método de búsqueda     ");
        puts("===================================");

        puts("0) Glosario de grafico");
        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '0':
                mostrarGlosario();
                break;
            case '1':
                int iteracionesDFS = 0; 
                deep_first_search(&estado_inicial, &iteracionesDFS);
                printf("Cantidad de iteraciones = %i\n", iteracionesDFS); // estos mensajes muestran cuantas iteraciones se demoro cada uno, lo tire afuera de la funcion para que no se pierdan cuando termine cada busqueda
                break;
            case '2':
                int iteracionesBFS = 0;
                breadth_first_search(&estado_inicial, &iteracionesBFS);
                printf("Cantidad de iteraciones = %i\n", iteracionesBFS);
                break;
            case '3':
                int iteracionesAstar = 0;
                busqueda_Astar(&estado_inicial, &iteracionesAstar);
                printf("Cantidad de iteraciones = %i\n", iteracionesAstar);
                break;
        }

        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }

  } while (opcion != '4');

  return 0;
}
