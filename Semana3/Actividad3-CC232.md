# Actividad 3 - CC232

### Integrantes
- Espinoza Sandoval, Adriel Alejandro

## Bloque 1

1. En vez de que los elementos se guarden de forma contigua físicamente en memoria, se guardan punteros que pueden dirigir a distintos puntos de ella.
2. El acceso por rango es por la contigüidad, mientras que por posición accede a cualquier posición arbitraria de la memoria física.
3. Porque al insertar o eliminar solamente se necesitan actualizar los enlaces de los elementos anterior y siguiente (complejidad $O(1)$), pero al acceder por índice se requiere hacer un bucle para acceder a los enlaces hasta llegar a esa posición (complejidad promedio $O(n)$)
4. Porque implementa de la misma forma que un stack push y pop (haciéndolo del inicio), y de un queue add y pop.
5. Porque para remover un elemento del final se requiere el enlace del anterior elemento, teniendo que acceder a los enlaces de todos los elementos anteriores.
6. El nodo aporta la cabeza y cola de la lista enlazada.
7. Porque dependiendo de si está cerca del inicio o final de la lista, procede por allí.
8. SEList usa ArrayDeque enlazadas.
9. Para adaptar sus funcionalidades para la implementación de SEList.
10. Es un adaptador de los métodos de `CleanList`. No reemplaza ya que es solamente una implementación una estructura de lista doblemente enlazada.

## Bloque 2

|Archivo|Salida y observable importante|Idea estructural|Argumento de costo, espacio o diseño|
|---|---|---|----|
|`Semana3/demos/demo_sllist.cpp`|`size = 3,peek = 5, pop = 5, remove = 10`|Lista enlazada simple||
|`Semana3/demos/demo_dllist.cpp`|`DLList: 10 20 30`|Lista enlazada doble|`add`: $O(1)$ `get`: $O(n)$|
|`Semana3/demos/demo_selist.cpp`|`0 10 20 30 40 50 60 70 80 90`|Lista de ArrayDeque enlazada|||`Semana3/demos/demo_deng_list.cpp`||Lista de Deng||
|`Semana3/demos/demo_morin_deng_bridge.cpp`||Lista enlazada de Deng||
|`Semana3/demos/demo_capitulo3_panorama.cpp`||Panorama de las distintas estructuras||
|`Semana3/demos/demo_min_structures.cpp`|`MinStack min=3 top=7, Min Queue min=2 front=4, MinDeque min=1 front=3 back=1`|Estructuras mínimas||
|`Semana3/demos/demo_xor_list.cpp`|`XorList: 5 10 20, front=5 back=20`|Lista XOR||
|`Semana3/demos/demo_linked_adapters.cpp`||Adaptadores de estructas con enlaces||
|`Semana3/demos/demo_contiguous_vs_linked.cpp`||Lista contigua vs enlazada||

## Bloque 3

1. Valida el comportamiento de `add`, `push`, `size`, `peek`, `pop`, `remove` y `size`.
2. Valida el comportamiento de `add`, `size`, `get`, `remove` y `size`
3. Valida el comportamiento de `add`, `size`, `get`, `remove` y `size`
4. `to_vector`, `secondLast`, `checkSize`, `rotate`, `isPalindrome`, `remove `, `front`
5. Los comportamientos de un `Stack`, `LinkedStack`, `LinkQueue` y `LinkDeque`.
6. La reutilización de la funciónes de ordenamiento y conversión a vector.
7. Se estresa su estabilidad por medio de la ocupación de la lista en memoria, haciendola crecer y decrecer.
8. Un comportamiento mínimo esperado de las funciones.
9. No demuestra comportamientos más internos, como invariantes, costos, casos bordes, etc.
10. Porque comportamientos internos requieren de análisis del algoritmo.

## Bloque 4

* **Roles de head, tail y n:** `head` apunta al inicio para acceso LIFO y recorridos; `tail` apunta al final para inserciones $O(1)$ en colas; `n` mantiene el conteo de elementos para evitar recorridos de longitud $O(n)$ al consultar el tamaño.
* **Cambios de punteros:** En `push`/`add(0,x)`, el nuevo nodo apunta al antiguo `head` y `head` se actualiza. En `pop`/`remove(0)`, `head` se mueve al siguiente nodo. En `add(n,x)`, el `next` del `tail` actual apunta al nuevo nodo y `tail` se mueve a este. `remove(n-1)` requiere buscar el penúltimo nodo para actualizar su `next` y mover el `tail`.
* **Funcionamiento de secondLast():** Recorre desde `head` comparando el `next` de cada nodo con `tail`. Es necesario porque en una lista simple no hay enlaces hacia atrás para conocer el predecesor de `tail` directamente.
* **Reverse() sin estructura auxiliar:** Invierte la dirección de los enlaces en un solo recorrido. Usa tres punteros temporales para no perder la referencia al resto de la lista mientras se cambia el puntero `next` del nodo actual hacia el nodo previo.
* **Función checkSize():** Recorre físicamente toda la lista contando nodos y valida que el resultado coincida con el valor de la variable `n`. Es una medida de integridad para detectar errores en la lógica de actualización del contador.
* **Acceso en DLList (getNode):** Al ser doblemente enlazada, si el índice está en la primera mitad se recorre desde el inicio; si está en la segunda, se recorre desde el final (usando `dummy->prev`), optimizando el tiempo a $O(1 + \min(i, n-i))$.
* **Uso del nodo centinela (dummy):** Elimina casos borde (lista vacía, insertar al inicio o al final) porque siempre existen nodos adyacentes reales. En `addBefore`, solo se requiere actualizar los punteros del nuevo nodo y de sus vecinos inmediatos.
* **Operación rotate(r):** Reubica el nodo `dummy` entre la nueva "cola" y la nueva "cabeza" de la lista. No mueve los datos, solo rompe y reconecta enlaces, resultando en una operación de costo constante respecto a punteros.
* **isPalindrome() en DLList:** Aprovecha los enlaces bidireccionales comparando simultáneamente elementos desde el inicio (`head`) y el final (`tail`) moviéndose hacia el centro.
* **Estructura Location en SEList:** Es un par `(u, j)` donde `u` es el nodo (bloque) y `j` es la posición dentro del `ArrayDeque` que contiene dicho bloque.
* **Spread() y Gather():** `spread()` reparte elementos cuando un bloque excede el tamaño `b`, mientras que `gather()` fusiona bloques cuando su ocupación es muy baja para mantener la eficiencia de espacio.
* **Trade-off del tamaño b:** Un `b` grande mejora la localidad de memoria y el acceso aleatorio (parecido a un arreglo), pero hace que las inserciones internas sean más lentas por los desplazamientos de datos.

## Bloque 5

* **Reutilización de SLList:** `LinkedStack` usa `add(0,x)` y `remove(0)`; `LinkedQueue` usa `add(n,x)` para insertar y `remove(0)` para extraer.
* **LinkedDeque y DLList:** Un `Deque` requiere eliminar del final en $O(1)$. Solo `DLList` permite esto gracias al puntero al predecesor; en `SLList`, eliminar el último nodo es $O(n)$.
* **MinStack:** Guarda en cada nodo el valor actual y el valor mínimo presente en la pila hasta ese punto, permitiendo que la consulta `min()` sea $O(1)$.
* **MinQueue:** Implementada con dos pilas (`in` y `out`). El mínimo global es el menor de los mínimos de ambas pilas.
* **Rebalanceo en MinDeque:** Asegura que ninguna de las dos pilas internas se quede vacía prematuramente, garantizando que el costo de las operaciones se mantenga constante de forma amortizada.
* **Implementar vs. Adaptar:** Implementar implica gestionar punteros y nodos manualmente; adaptar consiste en envolver una estructura ya probada para ofrecer una interfaz específica (ej. usar `DLList` para crear un `Deque`).
* **Complejidad:** Las operaciones de pila y cola en estas estructuras son constantes ($O(1)$). En el caso de `SEList` o estructuras con rebalanceo, se consideran costos amortizados.

## Bloque 6

* **Refuerzos en DengList:** Incluye algoritmos avanzados como ordenamiento estable, eliminación de duplicados (`uniquify`) y reversión de rangos.
* **Ventaja de la encapsulación:** Permite aplicar algoritmos complejos sobre estructuras de Morin sin alterar el diseño original de estas últimas.
* **to_deng y assign_from_deng:** Realizan la conversión de datos entre el formato de lista enlazada (Morin) y el formato compatible con los algoritmos de Deng, con un costo de $O(n)$.
* **stable_sort_with_deng:** Evita reimplementar el ordenamiento en cada tipo de lista; el puente delega el trabajo al algoritmo de Deng y devuelve la lista ya ordenada.
* **dedup_with_deng:** Filtra elementos repetidos aprovechando que, si la lista está ordenada, solo requiere una pasada lineal.
* **Costo de conversión:** Introduce una sobrecarga de $O(n)$ en tiempo y espacio. Es aceptable cuando la complejidad del algoritmo a ejecutar (como un ordenamiento $O(n \log n)$) justifica el paso de conversión.

## Bloque 7

* **ArrayDeque vs. LinkedDeque:** El primero ofrece mejor localidad y acceso $O(1)$, pero requiere `resize`. El segundo no requiere memoria contigua grande ni `resize`, pero gasta más en punteros.
* **Localidad de memoria:** Los arreglos aprovechan el caché del CPU al tener datos contiguos. Las listas enlazadas sufren de fallos de caché (*cache misses*) al saltar entre direcciones de memoria dispersas.
* **Ventaja de representaciones enlazadas:** Sobresalen en inserciones/eliminaciones frecuentes en los extremos o cuando el tamaño de la estructura cambia drásticamente de forma constante.
* **Lectura de Benchmarks:** No son verdades absolutas; muestran el rendimiento bajo condiciones específicas de hardware y carga de trabajo. Por ejemplo, muestran la clara superioridad del acceso aleatorio en arreglos.
* **XorList:** Utiliza aritmética de punteros (`prev XOR next`) para reducir a la mitad el gasto de memoria por enlaces en listas dobles, aunque complica la implementación y depuración.

## Bloque 8

Pasar de arreglos dinámicos a estructuras enlazadas implica cambiar la indexación directa por la navegación por referencias. Mientras que la representación contigua garantiza acceso $O(1)$ por rango, las estructuras enlazadas ofrecen flexibilidad para modificar la estructura en $O(1)$ localmente sin desplazamientos masivos de datos. Estructuras como `DLList` y `SEList` demuestran que es posible equilibrar el costo de acceso y el uso de memoria, mientras que los adaptadores permiten reutilizar estas bases para construir interfaces complejas (pilas, colas, deques) con garantías de rendimiento específicas.

## Autoevaluación

* **Fortalezas:** Dominio de la lógica de punteros en `SLList`/`DLList` y el concepto de adaptadores.
* **Puntos a reforzar:** Profundizar en el análisis amortizado de `SEList` durante procesos de `gather`.
* **Evidencia para sustentación:** Uso del costo $O(1 + \min(i, n-i))$ de `DLList` como argumento de eficiencia frente a listas simples.