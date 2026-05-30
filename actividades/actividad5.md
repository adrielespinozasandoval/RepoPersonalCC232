## Bloque 1

1. Un árbol binario enlazado usa punteros para enlazar nodos con sus hijos, mientras que el otro usa arreglo con una cierta estructura que codifica el árbol.
2. Un BinNode guarda el valor que se almacena en dicho nodo, así como punteros a su padre, hijos. La altura indica lo "profundo" que está dicho nodo desde la raíz.
3. Porque dichas operaciones pueden requerir salir de un subárbol. Mientras que es posible hacerlo sin parent (ejemplo con un Stack), tener parent simplifica este costo de tiempo y espacio amortizado.
4. BinTree es la representación del árbol y operaciones asociadas a ella como altura, manejo de subárboles, iteración a lo largo del árbol, etc. Mientras que BinNode se encarga de un nodo individual, sus descendientes y sus más próximos vecinos (que son requeridos para las visitas).
5. Agrega infraestructura de C++ estándar como funciones de lista, iteradores, representación como String.
6. Las funciones de búsqueda binaria.
7. El manejo del arreglo como árbol (usando el orden).
8. En ambos, se mantiene un comparador, 
9. Por el orden del BST que mantiene .
10. Por su representación como arreglo necesita un procesamiento avanzado para tratarlo como árbol, a diferencia del uso de árbol binario enlazado. 

## Bloque 2

1. hasLeft indica si tiene hijo izquierdo, hasRight si tiene derecho, isRoot si no tiene padre, isLeaf si no tiene hijo, isLeftChild si es hijo izquierdo de un padre e isRightChild si es hijo derecho.
2. Cuando haya terminado con el subárbol del hijo izquierdo
3. Cuando haya terminado con un subárbol izquierdo del árbol completo
4. Similar como succ, cuando haya terminado con un subárbol derecho, pasa al hijo izquierdo y busca el nodo más a la derecha.
5. [Dibujo]
6. Depth calcula la distancia hasta el nodo más profundo (más distante de la raíz).
7. Height calcula la distancia hasta la raíz
8. Calcula la cantidad de nodos de un subárbol.
9. [Gráficamente se puede demostrar la igualdad directa]
10. [Mismo de 9]

## Bloque 3

|Recorrido|Versión revisada|Estructura auxiliar usada|Secuencia producida en el árbol de prueba|Argumento de correctitud y costo|
|----|----|----|----|----|
|Preorden recursivo|Stack implícito||$O(n)$|
|Preorden iterativo|Stack|||
|Inorden recursivo|Stack implícito||$O(n)$|
|Inorden iterativo #1|Stack||$O(n)$|
|Inorden iterativo #2|||$O(n)$|
|Inorden iterativo #3|||$O(n)$|
|Postorden recursivo|Stack implícito||$O(n)$|
|Postorden iterativo|Stack||$O(n)$|
|Recorrido por niveles|||$O(n)$|

1.
2.
3.
4. Pasar por la misma profundidad
5. Por que se pasa por cada subárbol una sola vez.
6. Por que cada nodo se pasa a lo sumo dos veces.
7. $O(log n)$
8. $O(n)$
9. Que uno usa un Stack explícito en código, mientras que el otro Stack está implícito en las llamadas recursivas de la función
10. Porque no se garantiza que dichos niveles .