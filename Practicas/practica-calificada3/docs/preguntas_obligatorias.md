## Preguntas comunes

1. El problema trata de encontrar la suma de los $k$ menores elementos de la subsecuencia $A_i,...,A_{(i+m-1)}$ para cada $i = 1,...,n-m+1$.
2. Se recibe una secuencia $A$ de $n$ elementos, y de salida $n-m+1$ valores que son la suma de los $k$ menores correspondiente.
3. La ineficiencia tanto temporal como espacial para valores grandes de `n` y `m`ya que induce redundencia de procesamiento.
4. La solución más directa de pensar es copiar ($O(m)$), ordenar parcialmente ($O(m \log k)$) la secuencia y sumar ($O(k)$), lo cual hace cada deslizamiento tener complejidad $O(m \log k)$ ($\cdot (n-m+1) = O(n \cdot m \log k)$)
5. Se eligió un Treap, ya que principalmente cumple la propiedad BST, útil para este tipo de problemas que requieren ordenamiento, los datos llegan en *streaming* y las prioridades aleatorias convergen a un árbol balanceado.
6. Esta estructura ya ha sido implementada en `cc232`, pero ha sido reimplementada y refinada para un mejor control de tipos, una mayor generalidad (que cumplan un cierto ADT) y especialización para este problema.
7. Todas las operaciones tienen una complejidad $O(h)$, siendo $h$ la altura de árbol (en promedio $\log  m$)
8. Las invariantes de BST y Heap. En cada nodo el hijo izquierdo tiene un valor menor al del padre, y el derecho mayor. En las prioridades, ambos son menores al del padre.
9. Las funciones de rotación `rotateLeft` y `rotateRight` que usan las funciones `percolateUp` y `percolateDown` que a su vez usan cuando se insertan o remueven elementos y mantienen la invarianza.
10. En el caso que dos o más elementos sean iguales en árboles BST una solución superficial lo detectaría como iguales, pudiendo hacerlo fallar.
11. `LeastElements` implementa la estructura `TreapComparator` en el cual las posiciones de los elementos en el contenedor a comparar provocan el desempate.
12. Dado que a lo sumo dos nodos de `window_` y `least_` se eliminan e insertan el costo por deslizamiento es $O(\log m)$.
13. Dado que hay $n-m+1$ deslizamientos, la complejidad total temporal es $O(n \log m)$
14. Dado que se cada ventana tiene $m$ elementos, la complejidad espacial es $O(m)$
15. 
16. Se podría usar dos Heaps, pero requería *lazy deletion* (despreciar los elementos que no están en la ventana) lo cual al largo plazo puede generar un desperdicio de memoria, o en otra solución de jugar con el `Compare` dejar el determinismo y por lo tanto perder la invarianza del heap.
17. Se escribió más casos de prueba con conjuntos de datos aleatorios como con patrones que estresen el algoritmo.
18. 
19. 
20. Porque se implementó desde cero todas las estructuras utilizadas en la librería, lo cual requiere entender el algoritmo.

## Preguntas específicas

1. El Treap `least_`
2. Cuando el valor mínimo de `window_` es menor al máximo de `least_` (frontera)
3. La suma se actializa cuando algún elemento de `least_` es intercambiado.