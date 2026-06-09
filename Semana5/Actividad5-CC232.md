## Actividad 5 - CC232

### Estudiante

- Nombre: Espinoza Sandoval, Adriel Alejandro

### Bloque 1 - Núcleo conceptual

1. Un árbol binario enlazado requiere punteros explícitos en memoria (`left`, `right`, `parent`) para construir la jerarquía y conectar nodos. Un árbol implícito proyecta la estructura lógica sobre un arreglo secuencial puro, donde las relaciones de padre-hijo se calculan con operaciones aritméticas sobre los índices.
2. Guarda el valor (`data`), la ubicación de sus vecinos estructurales inmediatos en la memoria (`parent`, `left`, `right`) y su `height` (la máxima distancia en aristas desde el nodo hasta su hoja más profunda).
3. Al tener una referencia directa hacia arriba, el algoritmo puede retroceder hacia la raíz de forma dinámica sin necesidad de mantener un `Stack` auxiliar durante la búsqueda, permitiendo operaciones locales como buscar el sucesor (`succ()`) o actualizar alturas mediante `updateHeightAbove()`.
4. Sirve como la "envoltura" administradora. Protege el puntero absoluto a la raíz (`root_`), rastrea el recuento global de nodos en $O(1)$ (`size_`) y expone las políticas de mutación segura como `attach`, `secede` o inserciones.
5. Introduce el comportamiento estándar de C++ a la estructura abstracta. Agrega iteradores (`begin`, `end`), cálculos geométricos (altura, profundidad), traversals exportables a secuencias `std::vector` e impresiones gráficas (`asciiArt`).
6. Exige ordenamiento transversal total: para todo nodo, todos los elementos de su subárbol izquierdo son menores y todos los de su derecho son mayores, regidos por un comparador estricto (`comp_`).
7. Exige un orden vertical parcial (Min-Heap): el valor de cada nodo padre siempre es menor o igual al valor de cualquiera de sus dos hijos.
8. Un BST garantiza orden absoluto lateral, permitiendo hallar el 3er menor elemento rápidamente. El Heap solo garantiza prioridad estricta en el nivel vertical (el menor está en la raíz); no establece ninguna regla entre hermanos laterales.
9. Porque Inorden dicta la visita: Izquierda $\to$ Centro $\to$ Derecha. Al superponer esto a la invariante del BST (Menores $\to$ Padre $\to$ Mayores), el resultado es lógicamente una secuencia ascendente.
10. Porque un arreglo Heap carece de información de orden completo entre ramas (el hijo derecho de la raíz puede ser menor que el nieto izquierdo). La única forma de ordenarlo es extraer su mínimo cíclicamente (`remove`), lo cual muta y consume la estructura.

### Bloque 2 - Navegación, altura, profundidad y tamaño

1. `hasLeft` y `hasRight` verifican existencia de hijos. `isRoot` verifica si `parent` es nulo. `isLeaf` valida que no tenga descendencia en absoluto. `isLeftChild` verifica si el puntero `left` de su padre apunta exactamente a sí mismo, y análogo para `isRightChild`.
2. Ocurre si el nodo tiene subárbol derecho (`right != nullptr`). El sucesor es el descendiente más profundo y a la izquierda de dicho hijo derecho (`right->leftmost()`).
3. Ocurre si no tiene subárbol derecho. Sube por `parent` mientras sea un "hijo derecho" (ya visitado). El primer padre del que es "hijo izquierdo" será el sucesor en inorden.
4. Si hay subárbol izquierdo, es el nodo más a la derecha de este (`left->rightmost()`). Si no, se sube mientras se sea un "hijo izquierdo"; el primer padre del cual es "hijo derecho" es el predecesor.
5. 
```
      (5)
     /   \
   (3)   (8)
   / \   / \
 (2) (4)(7)(9)
```
  - `succ(4)` es `5` (sube). `pred(4)` es `3`.
  - `succ(3)` es `4` (baja-derecha).
  - `pred(8)` es `7`.
6. Calcula el número de arcos hacia la raíz estructural del árbol. Se implementa con un bucle trivial que incrementa un contador por cada salto exitoso sobre `node->parent`.
7. Calcula la distancia a la hoja más profunda bajo `u`. Requiere bajar recursivamente porque el "camino más largo" es desconocido, tomando localmente el máximo entre la altura de los hijos izquierdo y derecho sumándole 1.
8. Suma el recuento total acumulado en `size()` de su hijo izquierdo y derecho, más 1 por sí mismo, bajando en postorden por todo el árbol.
9. `height(T)` es la rama más larga absoluta existente. El lado izquierdo de la ecuación dibuja una ruta desde la raíz hasta la hoja cruzando por un nodo `u` arbitrario. Como ese cruce puede no pertenecer a la rama "absolutamente más larga" del árbol `T`, su suma de aristas será lógicamente menor o igual al máximo global.
10. La suma iguala a la altura del árbol sólo si el nodo `u` está posicionado exactamente a lo largo de la rama más profunda global de todo el árbol.

### Bloque 3 - Recorridos y trazado guiado

|Recorrido|Versión revisada|Estructura auxiliar|Secuencia (Árbol genérico Raíz A, Hijos B,C)|Argumento de correctitud y costo|
|------|------|------|------|------|
|Preorden recursivo|`travPre`|Call Stack implícito|A, B, C|$O(n)$. Visita raíz primero, delega hijos. Visita cada borde 2 veces máx.|
|Preorden iterativo|`travPreIterative2`|`std::stack<Node*>`|A, B, C|$O(n)$. Apila R y luego L, así L es tope. Emula call stack a la perfección.|
|Inorden recursivo|`travInRecursive`|Call Stack implícito|B, A, C|$O(n)$. Desciende L, evalúa Centro, desciende R. Coste lineal en tamaño.|
|Inorden iterativo #1|`travInIterative1`|`std::stack<Node*>`|B, A, C|$O(n)$. Pega todo a la izq en pila, extrae e intenta ir a la derecha.|
|Inorden iterativo #2|`travInIterative2`|Variables de estado (`prev`)|B, A, C|$O(n)$. Sin pila adicional, simula con 3 punteros y `parent`. Espacio $O(1)$.|
|Inorden iterativo #3|`travInIterative3`|Iterador `succ()`|B, A, C|$O(n)$. Salta usando `leftmost()` y `succ()`. Muy elegante, espacio constante.|
|Postorden recursivo|`travPost`|Call Stack implícito|B, C, A|$O(n)$. Deja a la raíz al final absoluto garantizando hijos muertos.|
|Postorden iterativo|`travPostIterative`|Dos `std::stack<Node*>`|B, C, A|$O(n)$. `s1` inyecta a `s2` que invierte Preorden-Modificado a Postorden puro.|
|Por niveles|`travLevel`|`std::queue<Node*>`|A, B, C|$O(n)$. Consume nodos por profundidad FIFO. Encola hijos progresivamente.|

1. Implica procesar un nodo en el momento exacto en que se le descubre al descender, antes de tocar cualquier rama inferior.
2. Implica procesarlo "en el medio", tras regresar completamente de vaciar su subárbol izquierdo, pero antes de comenzar con el derecho.
3. Implica procesarlo cuando se garantizó que toda su posible descendencia (izq y der) ya fue evaluada; la raíz siempre es el último paso.
4. Es un escaneo de amplitud (BFS). Evalúa todo un "estrato" de nodos en una altura `k` dada antes de bajar a la altura `k+1`.
5. Porque la función se invoca y regresa exactamente una vez por cada nodo válido del árbol entero, realizando trabajo de $O(1)$ constante por nodo.
6. Porque los bucles empujan o extraen elementos desde los Stacks o se navega por aristas un número fijo de veces, respetando el límite asintótico lineal del conteo de nodos totales.
7. El árbol equilibrado tiene poca profundidad. La profundidad del Call Stack se trunca en $O(\log n)$ marcos de llamada.
8. El árbol actúa como lista enlazada (altura extrema). Inunda el Call Stack hasta requerir $O(n)$ marcos de memoria.
9. La explícita aloja datos crudos controlables sobre el Heap, evitando fallas críticas (Stack Overflow) en grafos inmensos; la implícita ensucia la memoria limitada del sistema (Call Stack) en C++.
10. En árboles completos, la fila base final (hojas) contiene casi la mitad de los elementos ($\sim \frac{N}{2}$), requiriendo una cola gigantesca. Un árbol degenerado (tipo LinkedList) siempre tiene ancho $1$, dejando a la cola consumiendo memoria constante mínima.

### Bloque 4 - Demos

|Archivo|Salida u observable importante|Idea estructural|Argumento de costo, espacio o diseño|
|------|------|------|------|
|`demo_binary_tree.cpp`|Secuencias devueltas por traversals (vectores) y gráficos ASCII.|Iteradores vs Recursividad sobre `BinaryTree`.|Asegura que traversales complejos generen datos íntegros idénticos con costos dispares espaciales.|
|`demo_bst.cpp`|El arreglo producto de `inorder()` siempre es monótono (clasificado).|Búsqueda binaria espacial en `BinarySearchTree`.|Proporciona tiempos de inserción rápidos $O(\log n)$ a la vez que preserva el ordenamiento natural.|
|`demo_heap.cpp`|`top()` constantemente muestra el valor mínimo remanente.|Arreglo implícito de prioridad completa.|El uso de `trickleDown` optimiza la recolocación en arreglo en $O(\log n)$ sin instanciar memoria suelta.|
|`demo_capitulo5_panorama.cpp`|Prueba encadenada de los tres ADTs contrastando su creación.|Composición de jerarquías sobre `BinaryTree`.|Evidencia cómo la misma raíz enlazada abstrae tanto el acceso a posiciones como búsqueda binaria.|

1. La extracción vectorial `std::vector<T>` permite interceptar y comparar los arrays paso por paso sin mutar el original.
2. Su integración comprobable sobre los iteradores (`iterator`) que atraviesan los extremos validando que devuelven estrictamente un clon del arreglo `inorder()`.
3. Es un depurador visual inmenso. Pone en perspectiva visual si las ramas están cargadas más a la izquierda o derecha en balances rotacionales.
4. Al exportarlo o imprimirlo, no presenta caídas ni altibajos numéricos; siempre va de valores chicos a gigantescos.
5. `lowerBound` devuelve siempre la franja contigua mayor si el elemento no fue hallado; `findEQ` expone un rechazo puro (`nullptr`) ante faltas.
6. Sin importar en qué orden estocástico se meta a la basura (`add()`), llamar repetidamente a `remove()` devolverá arreglos completamente ordenados numéricamente.
7. Agregar elemento por elemento aplica `bubbleUp` top-down desordenando ligeramente las raíces; `heapify` hace `trickleDown` bottom-up masivo creando una disposición de arreglo interna formalmente distinta.
8. Se destaca cómo el árbol base provee estructura universal, un BST se aplica en búsquedas acopladas ordenadas, y un Heap renuncia al rastreo para priorizar y destruir el mínimo.

### Bloque 5 - Pruebas e invariantes

1. Valida las adiciones lógicas, hallazgos (`findEQ`, `lowerBound`), que no se desarticule y su remoción interna (`remove`) preservando la cohesión.
2. Una aserción que captura si `add(val)` sobre un nodo que ya integra `val` devuelve explícitamente `false` y `size_` no se altera.
3. Se afirma `assert(Iterative == Recursive)`, asegurando de que ninguna versión deje nodos fuera y operen la misma ruta.
4. Se espera que arroje el puntero físico intacto en donde reside "8", de lo contrario, `nullptr`.
5. `lowerBound(9)` encuentra el nodo `v >= 9`. `upperBound(8)` encuentra el nodo `v > 8` estricto (no igual).
6. Que la regla universal de que cada nodo `X` cumpla iterativamente la cláusula "Izquierdos $<$ Padre $<$ Derechos" sin corromperse en ningún sub-fragmento.
7. Se constata una reducción escalar pura de `size()` y que `isBST()` sigue arrojando `true`.
8. Efectúa un escaneo de seguridad verificando bidireccionalmente que cada hijo enlazado mantenga su `parent` apuntado correctamente hacia quien invocó la llamada, evitando árboles infinitos o huérfanos.
9. Constata `add()`, `remove()`, la consulta del top sin destrucción (`top()`), y sus constructores directos con la rutina masiva `heapify()`.
10. Prueba el algoritmo *HeapSort*: al destruir el Heap elemento por elemento, devuelve la matriz totalmente ordenada.
11. Prueban la transferencia de nodos de un ente a otro asegurando que `size_` de ambos lados se balanceen y los `parent` se refactoricen sin fugar de memoria.
12. Someten al árbol al estrés de nodos nulos, validan el encadenamiento ascendente en los rebotes del límite del Heap, y miden alturas/profundidades dinámicas forzadas.
13. Demuestra que la interfaz básica responde sin caídas (seg faults) a la sintaxis del usuario normal.
14. No garantiza límites asintóticos estrictos (complejidad), ni carencia rotunda de fugas de memoria en escenarios adversos degenerados.
15. Los resultados observables son finitos; los invariantes y la complejidad demuestran la tolerancia universal de la máquina ante variables arbitrariamente grandes.

### Bloque 6 - Lectura cercana de código

1. Si un nodo $V$ es el `left` (o `right`) de $U$, entonces lógicamente el puntero $V \to parent$ debe ser idénticamente $U$.
2. Sobrescribir un puntero crudo en C++ de un nodo "vivo" sin avisar desata *memory leaks*.
3. Ejecuta un proceso recursivo natural descendente de sumar la métrica `size()` izquierda y derecha, consolidando el valor más la unidad local `1U`.
4. Bucles `while` ciegos que se deslizan eternamente en una sola dirección hasta topar con barrera `nullptr`.
5. Si el nodo tiene hijo derecho, entra allí y ejecuta su `leftmost()`. Si carece de hijo derecho, repite un ascenso recursivo (`parent`) hasta toparse con una bifurcación donde sea un hijo "izquierdo" de un eslabón superior; ese eslabón es su siguiente real.
6. Simetría total. Desciende `left` y halla su `rightmost()`. Si no, asciende el `parent` hasta chocar con el primer vínculo que dictamine que fuiste un "hijo derecho".
7. Fungen como los metadatos base en tiempo $O(1)$. Permiten ingresar rápidamente al origen.
8. Inspecciona alturas (usando `stature`) imponiéndose $+1$ al máximo.
9. Tras insertar, la propagación de una profundidad mayor altera las ramas superiores. Este bucle trepa `parent` corrigiendo todo hasta la cima.
10. Quita a un subárbol encapsulado su condición de ente libre. Inyecta a su `root_` en el puntero del nuevo padre y anula al ADT donante.
11. `remove` saca el subárbol llamando un delete a todo de la RAM. `secede` saca los punteros, pero retorna lo útil en un nuevo contenedor.
12. Porque su propósito es el "trasplante" de sub-segmentos estructurales de código.
13. Porque su propósito es el borrado absoluto definitivo.
14. Rastreando a la baja, asegura recursivamente que el puntero devuelto por un `child->parent` encaje con el bloque actual; localiza huérfanos estructurales.
15. Funciones de envoltura que redireccionan la lógica estática (`leftmost`, `succ`) bajo parámetros de objeto para ceder a las clases iteradores (`begin/end`).
16. Porque avanzar sin parar usando el "sucesor" de un nodo es equivalente por definición al procesamiento Inorden ($L \to N \to R$).
17. Facilita a los programadores confirmar de un vistazo ocular (sustentación) las inclinaciones de altura sin parsear código C++ tedioso.

### Bloque 7 - BST

1. Toda llave $k$ contenida en el subárbol izquierdo de un nodo $X$ es evaluable como estricto menor bajo comparador iterado. Toda llave contenida en el subárbol derecho es mayor.
2. El patrón de visita se acopla uno a uno con la invariante: Primero la rama estrictamente menor ($L$), luego el elemento neutro actual, finaliza absorbiendo la rama mayor ($R$).
3. `find` ubica estático, `findEQ` expone una asertividad de igualdad, `lowerBound` encuentra un objetivo $\ge$ al deseado y `upperBound` un objetivo estricto $>$.
4. Si se busca el 13 y el más cercano es 15; `findEQ` responde con "Fracaso" y `nullptr`, mientras `lowerBound` trae el 15 superior de contención.
5.
```
          (7)
        /     \
     (3)       (10)
    /   \      /  \
  (1)   (5)  (8)  (12)
        / \
      (4) (6)
```
6. **Recorridos:**
   - **Inorden:** 1, 3, 4, 5, 6, 7, 8, 10, 12
   - **Preorden:** 7, 3, 1, 5, 4, 6, 10, 8, 12
   - **Postorden:** 1, 4, 6, 5, 3, 8, 12, 10, 7
   - **Niveles:** 7, 3, 10, 1, 5, 8, 12, 4, 6
7. - `lowerBound(9)`: Salta desde `7` (der) $\to$ `10` (izq, guarda candidato 10) $\to$ `8` (der) $\to$ null. Retorna `10`.
   - `upperBound(8)`: Salta desde `7` (der) $\to$ `10` (izq, guarda candidato 10) $\to$ `8` (ignora como candidato, der) $\to$ null. Retorna `10`.
8. `Hoja` se borra nulo. `1 Hijo` se puentéa (splice). `2 Hijos` se requiere invocar `succ()`, encriptar datos y podar un elemento estructural de rango menor.
9. Unifica la operación topológica saltándose el eslabón actual para conectar a su nieto unívoco con el abuelo sin arrastrar ramas erróneas.
10. El número de nodos (`size_`), las jerarquías de magnitud, el equilibrio de enlazado `parent/left` y la aserción de `isBST()` no disminuyen ni un nivel de validez.
11. Porque internamente la sustitución usa "sucesor" de Inorden, que su clon tome el puesto central sin trastornar la escala preexistente.
12. Baja el nodo inicial apuntándolo como hijo izquierdo y asciende su pre-existente rama derecha al estrado.
13. Análogo espejo. Promueve su eslabón izquierdo al trono principal e inserta el nodo desterrado abajo a la derecha.
14. Para un `rotateLeft(U)`, la condición $U < \text{sub}(R)$ es intacta. La rama izquierda del hijo se acopla a la rama derecha del renegado $U$, garantizando ser mayores que $U$ pero menores que la nueva raíz.
15. Segmenta arreglos perfectos inyectando la "mitad" estricta (`median`) recursivamente. Aboca a un árbol perfectamente truncado con búsqueda garantizada en logaritmos.
16. En balanceado es $O(\log n)$. En degenerado (como listas) $O(n)$.

### Bloque 8 - Heap

1. Es un árbol perfectamente denso "Completo". No depara baches lógicos, pudiendo correlacionar saltos de altura con aritmética de base 2 (`left=2i+1`) en un array puro en caché.
2. En base a raíz indexada `0`, el estrato crece exponencialmente en potencias 2, alineando al hijo L a $2i+1$ y al hijo R al sucesor $+1$. Restar algebraicamente invierte la rama.
3. Prioridad asertiva donde $Padre \le Hijos$. No impone control transversal lateral.
4. Porque al validar recursivamente $Padre \le Hijos$, la raíz final está garantizada como menor que todos.
5. Tras anexarlo al fondo, indaga iterativamente contra su padre superior. Si el valor es minúsculo, transfiere por `swap` hasta anclarse.
6. Ubica su estrato comprobando cuál de sus dos hijos es *el más pequeño*. Si hay disyuntiva, desciende ocupando el más pequeño reacomodando.
7. Si no se moviera el último antes de `trickle`, ya no se cumple la compactación del nivel dejando un "hoyo". Subir el último elemento de la matriz y percolar a la baja asegura la integridad estricta del Array Completo.
8. Escaneo contiguo que evalúa a cada unidad que su `left` y `right` indexado no rompan con el umbral condicional `comp`.
9. Individual es pesado Top-Down por inserción solitaria. `heapify` acapara el array entero Bottom-Up reparando eficientemente solo las hojas infractoras conjuntas.
10. Hay que empujar $N$ veces, y cada elemento burbujea $O(\log N)$ barreras estructurales máximas en el peor caso.
11. Sumatoria teórica. Las cúpulas ($O(1)$ desplazamientos en nodos finales) ocupan la mayoría global poblacional del árbol.
12. Como extrae de la raíz (1) y hace swap continuo con el estrato inferior priorizado, salen cronológicamente 1, 2, 3, 5, 7, 8, 10 de menor a mayor en masa.
13. Heap brilla en colas de prioridad donde se borra al mínimo veloz $O(\log n)$ y $O(1)$ top. BST es indispensable para diccionarios orgánicos que piden saltos lógicos absolutos iterables como bases de datos masivas.

### Bloque 9 - Cierre comparativo

* Se pasa de estructuras lineales de acceso singular a formaciones orgánicas ramificadas. Una representación enlazada con nodos y punteros (`left`, `right`) rompe la unidimensionalidad creando dimensiones topológicas infinitas de altura dinámica sin los sobrecostos re-allocativos de arreglos básicos.
* A la vez, se demuestra la representación implícita en arreglo (`BinaryHeap`) donde una simple aritmética matemática (`2i+1`) logra simular topologías bidimensionales suprimiendo la necesidad de punteros pesados.
* Se diferencia fuertemente que las listas se agrupan por orden cronológico (llegada), la propiedad estructural limita únicamente la densidad espacial formal de un árbol y las propiedades de orden/prioridad reescriben los paradigmas internos sobre cómo los elementos asumen su magnitud (`BST` lateral contra `Heap` vertical).
* Los recorridos (Inorden, Preorden, Niveles) actúan como una "sonda proyectora", transformando dimensionalidad pura de grafo a un vector legible serial que la CPU pueda procesar e indexar.
* Validar alturas y enlaces `parent` adentró al control estricto de balances locales que impiden colapsos catastróficos u orfandades sin usar memoria implícita desbordante en Call Stacks.
* El BST y su Búsqueda Ordenada otorgan a un arreglo dinámico una maestría logarítmica extrema permitiendo encajar e iterar ordenadamente valores mediante su recorrido Inorden.
* El Heap se defiende de todo con su jerarquía de prioridad vertical; anula los gastos con métodos asintóticos ultra eficientes de goteo (`bubbleUp`/`trickleDown`) forjando colapsos topológicos integrales `heapify()` con $O(N)$.
* Finalmente, para defender la correctitud no basta invocar funciones, se exhibe su fortaleza y robustez al exponer a la máquina validando aserciones puras formales como `isBST()`, `checkParentLinks()`, `isHeapArray()`, y el cruce observacional exhaustivo con `asciiArt`.

### Autoevaluación breve

- **Qué puedo defender con seguridad:** La integración de un `BinaryHeap` de base indexada 0 y el razonamiento elemental de percolación `trickleDown`.
- **Qué todavía confundo:** Diferenciar el cálculo exacto en casos adversos subiendo `succ()` cuando los enlaces parentales entran en conflicto estructural múltiple en la misma rama.
- **Qué evidencia usaría en una sustentación:** Replicar aserciones iterativas de tests exportando vectores limpios con traversales contrastados, evidenciando el resultado idéntico lineal puro del `std::vector` ante las iteraciones complejas `Inorder3` y `iterateBySuccessor`.
- **Qué parte del código me parece más importante para revisar otra vez:** Las rotaciones profundas espaciales locales `rotateLeft`/`Right` del BST y los puentes topológicos de desprendimiento `secede` en `BinTree`.