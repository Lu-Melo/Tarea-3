# Sistema de Navegación en un Laberinto N x N

## Descripción

Este sistema permite a un agente (o robot) navegar en un laberintos donde algunas celdas están vacías y permiten el libre paso, mientras que otras contienen obstáculos (paredes) que bloquean el movimiento. Esta herramienta está diseñada para encontrar el camino más corto o más eficiente desde una posición inicial hasta una posición objetivo.

## Cómo compilar y ejecutar

El proyecto fue desarrollado en lenguaje C y puede ejecutarse usando [Replit](https://replit.com), sigue estos pasos:

### Requisitos previos:

- Tener una cuenta de Replit.

### Pasos para compilar y ejecutar:

1. **Descarga el** archivo `.zip` en una carpeta de tu elección.
2. Ingresa a [replit.com](https://replit.com) e inicia sesión.
3. Crea un nuevo Repl seleccionando **"Upload from your computer"** y sube el archivo `.zip`.
4. Asegúrate de que el archivo principal sea `tarea3.c` y que las carpetas `tdas/` estén incluidas en el proyecto.
5. En la terminal de Replit, compila el programa con:
4. **Compila el código**
    - Abre el archivo principal (`tarea3.c`).
    - En la terminal de replit, compila el programa con el siguiente comando:
        
        ```bash
        gcc tdas/*.c tarea3.c -Wno-unused-result -o tarea3
        ```
        
5. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```bash
        ./tarea3
        ```
        

## Funcionalidades

### Funcionando correctamente:

- Generación aleatoria del laberinto con dificultad configurable (porcentaje de obstáculos del 0 al 100).
- Visualización del estado inicial del laberinto con su distancia L1 hacia la meta.
- Glosario de los símbolos usados en el laberinto (opción 0 del menú).
- Búsqueda en Profundidad (DFS): encuentra un camino usando una pila para almacenar los estados que quedan por visitar, no garantiza la ruta más corta.
- Búsqueda en Anchura (BFS): encuentra el camino más corto en cantidad de pasos usando una cola para almacenar los estados que quedan por visitar. Este algoritmo evalúa el espacio en forma de "ondas" concéntricas y, por su naturaleza, garantiza encontrar el camino con la menor cantidad de pasos.
- Búsqueda A^*^: encuentra el camino más corto de forma eficiente usando un heap (cola de prioridad) para almacenar los estados no visitados. En cada iteración, selecciona el estado que minimiza la función de evaluación, sumando los pasos dados más la distancia L1 (Manhattan) hacia la meta. Este algoritmo también garantiza encontrar la ruta más corta, pero de manera mucho más eficiente que BFS.
>**Para cada Búsqueda:**
> - Visualización del camino encontrado dibujado sobre el laberinto con el carácter `#`.
> - Reporte del número de pasos y cantidad de iteraciones por cada algoritmo.


### Problemas conocidos:

- En este contexto no consideré necesario liberar la memoria dinámica de los nodos que no están visitados, pero si se agranda el laberinto si sería necesario haverlo para evitar problemas futuros de consumo de memoria.

### A mejorar:

- Implementar una interfaz de usuario más amigable.
- Permitir la re-generacion de laberintos sin tener que cerrar el programa.
- Implementar una comparacion más explícita de los distintos algoritmos de búsqueda.

## Ejemplo de uso

## Ejemplo de uso

**Paso 1: Ingresar la dificultad**

Al ejecutar el programa, se solicita el porcentaje de obstáculos del laberinto:

```C
Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): 90

Estado inicial del puzzle:
 A  .  .  .  .  ■  .  .  ■  ■ 
 .  .  .  .  .  .  .  .  .  . 
 .  .  .  .  .  .  .  .  .  . 
 .  .  .  .  .  ■  .  .  .  . 
 ■  .  .  .  .  .  .  .  .  . 
 ■  .  ■  .  .  .  .  .  .  . 
 ■  .  .  .  .  .  .  .  .  . 
 ■  ■  ■  .  .  .  ■  .  .  ■ 
 ■  ■  ■  .  .  .  ■  .  .  . 
 .  ■  ■  ■  .  .  .  .  ■  M 
Distancia L1: 18
===================================
     Escoge método de búsqueda     
===================================
0) Glosario de grafico
1) Búsqueda en Profundidad
2) Buscar en Anchura
3) Buscar A*
4) Salir
Ingrese su opción: 
```

El programa genera y muestra el laberinto inicial junto con la distancia L1 desde el inicio hasta la meta y un pequeño menú.

**Paso 2: Ver el glosario**

Si no conoces los símbolos del laberinto, puedes consultarlos:

```C
Ingrese su opción: 0
===================================
      Como leer el laberinto       
===================================
  'I'  = (letra I mayuscula) Inicio
  'M'  = (letra M mayuscula) Meta
  '#'  = (Simbolo de hashtag/numeral) Camino encontrado por el algoritmo de busqueda
 ' . ' = (Simbolo de punto) Espacios libres
 ' ■ ' = (Simbolo de bloque solido) Espacios con obstaculos
Presione una tecla para continuar...
```

**Paso 3: Ejecutar Búsqueda en Profundidad (DFS)**

El usuario ingresa la opción 1 para ejecutar la búsqueda en profundidad y se muestra el laberinto con el camino, cantidad de pasos e iteraciones del algoritmo.

```C
Ingrese su opción: 1
===================================
     Busqueda por Profundidad      
===================================
 I  #  #  #  #  ■  .  .  ■  ■ 
 .  .  .  .  #  #  #  #  #  # 
 #  #  #  #  #  #  #  #  #  # 
 #  #  #  #  #  ■  .  .  .  . 
 ■  .  .  .  #  #  #  #  #  # 
 ■  .  ■  #  #  #  #  #  #  # 
 ■  .  .  #  #  #  #  #  #  . 
 ■  ■  ■  .  .  .  ■  #  #  ■ 
 ■  ■  ■  .  .  .  ■  #  #  # 
 .  ■  ■  ■  .  .  .  .  ■  A 
Cantidad de pasos = 50
Cantidad de iteraciones = 52
Presione una tecla para continuar...
```
**Paso 4: Ejecutar Búsqueda en Anchura (BFS)**

El usuario ingresa la opción 2 para ejecutar la búsqueda en anchura y se muestra el laberinto con el camino, cantidad de pasos e iteraciones del algoritmo.

```C
Ingrese su opción: 2
===================================
       Busqueda por Anchura        
===================================
 I  .  .  .  .  ■  .  .  ■  ■ 
 #  .  .  .  .  .  .  .  .  . 
 #  .  .  .  .  .  .  .  .  . 
 #  #  .  .  .  ■  .  .  .  . 
 ■  #  .  .  .  .  .  .  .  . 
 ■  #  ■  .  .  .  .  .  .  . 
 ■  #  #  #  #  #  #  #  .  . 
 ■  ■  ■  .  .  .  ■  #  .  ■ 
 ■  ■  ■  .  .  .  ■  #  #  # 
 .  ■  ■  ■  .  .  .  .  ■  A 
Cantidad de pasos = 18
Cantidad de iteraciones = 78
Presione una tecla para continuar...
```
**Paso 5: Ejecutar Búsqueda A^*^**

El usuario ingresa la opción 1 para ejecutar la búsqueda en profundidad y se muestra el laberinto con el camino, cantidad de pasos e iteraciones del algoritmo.

```C
Ingrese su opción: 3
===================================
          Busqueda por A*          
===================================
 I  .  .  .  .  ■  .  .  ■  ■ 
 #  #  #  .  .  .  .  .  .  . 
 .  .  #  #  .  .  .  .  .  . 
 .  .  .  #  .  ■  .  .  .  . 
 ■  .  .  #  .  .  .  .  .  . 
 ■  .  ■  #  .  .  .  .  .  . 
 ■  .  .  #  #  #  #  #  .  . 
 ■  ■  ■  .  .  .  ■  #  .  ■ 
 ■  ■  ■  .  .  .  ■  #  #  # 
 .  ■  ■  ■  .  .  .  .  ■  A 
Cantidad de pasos = 18
Cantidad de iteraciones = 76
Presione una tecla para continuar...
```

**Paso 6: Salir**
El usuario ingresa la opción 4.

```
Ingrese su opción: 4
```

El programa termina.
