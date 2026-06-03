## Bloque 1
1. Aparecen demostraciones para de las estructuras (PQ, Heapify, Heapsort, Left_heap_merge, Huffman), comparación con la semana 5, rotaciones BST, Treap y el panorama de todo lo implementado, así como sus pruebas internas y externas
2. Se incluye la interfaz e implementaciones de PQ, vector_heapsort y Treap
3. En la identificación del contexto en el que se trabaja, ya que se observa que la semana 10 implentará las estructuras de la semana 6.
4. De la estructura de árboles.
5. Heap
6. Arbol de búsqueda binaria
7. Treap
8. Que los tests funcionan, mostrando que las implementaciones tienen un comportamiento mínimo correcto.
## Bloque 2
```cpp
inline constexpr bool pqHasLeftChild(std::size_t i, std::size_t n) noexcept {return pqInHeap(pqLeftChild(i), n);}inline constexpr bool pqHasRightChild(std::size_t i, std::size_t n) noexcept {return pqInHeap(pqRightChild(i), n);}inline constexpr bool pqIsLeaf(std::size_t i, std::size_t n) noexcept {return !(pqHasLeftChild(i, n) || pqHasRightChild(i, n));}inline constexpr bool pqIsInternal(std::size_t i, std::size_t n) noexcept {return i != 0 && (pqHasLeftChild(i, n) || pqHasRightChild(i, n));}
```
1. Para no colocar operaciones aritméticas que pueden dificultar la lectura del código.
2. Es una opción más moderna y recomendada a la hora de compilar en C++, permite firmas (tipos) y es más legible.
3. Siempre elegira intercambiar con dicho hijo
4. La condición del bucle while
5. El código de la implementación (sintáxis). El comportamiento es equivalente (semántica).