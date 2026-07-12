
## Actividad 1- CC232

### Integrantes
- Fabio Alberto Pantoja Limache
- Adriel Alejandro Espinoza Sandoval

#### Bloque 1 - Núcleo conceptual de Semana1

1. - **Problema:** Situación a resolver
   - **Algoritmo:** Estragia lógica para resolver el problema independientes del lenguaje.
   - **ADT:** Interfaz/ contrato de operaciones abstractas. 
   - **Implementación:** Forma de ejecución concreta del ADT 
2. El observable que conecta la correctitud y el costo es el conteo de comparaciones e intercambios presentados en la estructura BubbleStats, ya que al ejecutar el algoritmo, se obtine tanto la verificacion de que el resultado es correcto como una medida precisa del costo en terminos de operaciones realizadas.
3. Porque en el caso de pow, multiplicar con cuadrados (n^2, (n^2)^2, etc.) es más eficiente que multiplicar 'exponente' veces.
4. La funcion "fib(n)" es correcta por que cumple en dar el resultado esperado, pero es una mala eleccion practica porque para valores altos abusa de manera exponencial las llamadas recursivas.
5. Esto se debe a como se va trabajar la entrada, en este caso no importa el valor que representa, si no el como se representa en bits. 
6. Porque distintas implementaciones funcionan de una manera equivalente, aunque dependiendo de la aplicación puede variar su eficacia.

#### Bloque 2 - Demos y trazado guiado

| Archivo | Salida u Observable Importante | Idea Algorítmica | Argumento de Costo |
| :--- | :--- | :--- | :--- |
| `demo_bubblesort.cpp` | Arreglo ordenado y estadísticas de `swaps` y `comparisons`. | **Burbuja Optimizado:** Uso de `lastSwapIndex` para actualizar el límite `end` y evitar revisar el sufijo ya ordenado. | **Peor:** $O(n^2)$ (Arreglo invertido). <br>**Mejor:** $O(n)$ (Ya ordenado). |
| `demo_power.cpp` | Resultados de $a^n$ y $2^n$ calculados por 6 métodos diferentes. | **Exponenciación Binaria:** Divide el exponente a la mitad (`n >> 1`) y eleva la base al cuadrado (`p *= p`) en cada paso. | **Fuerza Bruta:** $O(n)$ (Lineal). <br>**Optimizado:** $O(\log n)$ (Logarítmico). |
| `demo_fibonacci.cpp` | El 10mo término (55) y la secuencia generada por la clase `Fib`. | **Iteración vs Recursión:** Contrasta el bucle simple contra la recursión de árbol y la recursión lineal con paso por referencia. | **Iterativo/Lineal:** $O(n)$. <br>**Recursivo Simple:** $O(2^n)$ (Exponencial). |
| `demo_countones.cpp` | Cantidad de bits en '1' de un entero. | **Manipulación de Bits:** Contraste entre el algoritmo de Kernighan (`n & n-1`) y el conteo paralelo con máscaras. | **Kernighan:** $O(k)$ (según bits activos). <br>**Paralelo:** $O(1)$ (5 pasos constantes). |

1. Las salidas de `BubbleStats`: `.comparisons`, `.swaps`, `.lastSwapIndex`.
2. La comparacion es el mismo resultado numerico para las implementaciones pero `power(base,exp)` presenta una mejora algoritmica respecto al costo y eficiencia.
3. El de FibI y la segunda de Fib
4. El segundo, ya que la función agrupa en bloques, marcando la diferencia entre bits (valor numérico) y su tamaño.


#### Bloque 3 - Pruebas públicas y preparación de sustentación

1. test_public_week1 prueba las funciones de suma, potencia y Fibonacci. test_public_week1_extra prueba bubblesort, máximo, revertir una lista y resolver la torre de Hanoi.
2. Demuestra que tenga un comportamiento mínimo esperado, como devolver algunas salidas correctamente.
3. No demuestra otros comportamientos más internos del algoritmo, como invariantes, costos, casos bordes, etc.
4. **(6)** bubble_sort resuelve un ordenamiento de una lista, toma como entrada una lista con datos relacionados con un orden (como números y el orden 'mayor' (<)) y devuelve una lista ordenada tal que cada par de elementos cumpla ese orden.
5. - En proceso
   - En proceso
   - Logrado

#### Bloque 4 - Puente corto con Proyecto0

1. El vector es leído por sum_readonly, sumando todos los valores que los contiene. El vector original cambia con append_in_place, ya que le agrega un valor, modificandolo. Mientras que append_copy agrega a una copia del vector.
2. Que reserve reserva un espacio de memoria para n enteros y poder llenarlo. Cuando no se reserva, el vector podría necesitar realocarse cuando se añade un valor nuevo.
3. Porque mientras push_back solo añade un valor al final, insert require mover todos los elementos siguientes antes de colocar el nuevo valor, siendo insert(begin()) más costoso porque es el que mueve más elementos.
4. Nos encontramos con 3 formas de sumar elementos de un vector, los cuales se diferencian en como acceden a los elementos del vector. Observamos que los accesos secuenciales y contiguos son mas eficientes

#### Bloque 5 - Extensión breve con Ejercicios0

1. 
2. Que usar un algoritmo apropiado para el problema puede hacer que el costo sea menor y más eficiente. No siempre usar y 'combinar' los mismos algoritmos es lo mejor.
3. Puede evidenciar la correctitud y eficiencia de los algoritmos.
4. Menciona limitaciones con `ASan`, `UBSan`, `TSan`, `gcov` y `gprof`
5. Porque no se demuestra su correctitud. Para ello, se requiere de otros análisis como invariantes.

#### Bloque 6 - Cierre comparativo

Al pasar de defender la correctitud teórica a comparar implementaciones mediante evidencia experimental, el enfoque cambia de garantizar la validez abstracta a evaluar el comportamiento físico y real del código frente a los recursos del sistema. En este contexto, se comprueba que la solucion implementada satisface el problema, ademas probar que el algoritmo termina y es logicamente valido. No obstante no basta con que el problema se resuelva con correctitud, las implementaciones con evidencia experimental nos ayudan a tener una nocion del comportamiento asintotico, esto no solo depende de la logica, pudimos comprobar que el como gestiona la memoria la estructura de datos elegida ( asignacionde espacio, la contiguidad y la localidad de cache).