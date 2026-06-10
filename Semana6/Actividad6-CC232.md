## Actividad 6 - CC232

### Estudiante
- **Nombre:** Espinoza Sandoval, Adriel Alejandro
- **Código:** 20243502F
- **Fecha:** 9 de junio de 2026

### Bloque 1 - Diagnóstico inicial

1. Al revisar la configuración de CMake, se nota que compila demostraciones para las estructuras implementadas en la semana (Priority Queues, Heapify, HeapSort, Leftist Heaps, Huffman, Treaps) y ejecuta sus respectivas pruebas unitarias y públicas.
2. El archivo `Capitulo6.h` actúa como un índice o agrupador que incluye las interfaces e implementaciones clave, como las colas de prioridad (`PQ.h`), el ordenamiento `vector_heapSort.h` y estructuras híbridas como el `Treap.h`.
3. Ayuda a ubicar el contexto porque las estructuras diseñadas aquí servirán como base o componentes internos para los algoritmos que se abordarán en la semana 10.
4. Heredan directamente de la teoría de árboles binarios vista en la semana 5, reutilizando conceptos como la navegación entre nodos, el cálculo de alturas y los recorridos.
5. Para mantener la prioridad se usa la estructura Heap (montículo), principalmente su variante en arreglo completo (`PQ_ComplHeap`) o su versión enlazada (`PQ_LeftHeap`).
6. Para mantener la búsqueda ordenada se sigue confiando en el Árbol de Búsqueda Binaria (BST).
7. La estructura que combina las propiedades de orden lateral (BST) y de prioridad vertical (Heap) es el Treap.
8. Al correr las pruebas con `ctest`, se comprueba que el 100% de los tests pasan, lo que confirma que el código base proporcionado es funcional y estable.

### Bloque 2 - Utilidades de heap completo

```cpp
// MOD-A6-B2: Utilidades auxiliares constexpr para evaluar fronteras nodales en O(1)
inline constexpr bool pqHasLeftChild(std::size_t i, std::size_t n) noexcept {return pqInHeap(pqLeftChild(i), n);}
inline constexpr bool pqHasRightChild(std::size_t i, std::size_t n) noexcept {return pqInHeap(pqRightChild(i), n);}
inline constexpr bool pqIsLeaf(std::size_t i, std::size_t n) noexcept {return !(pqHasLeftChild(i, n) || pqHasRightChild(i, n));}
inline constexpr bool pqIsInternal(std::size_t i, std::size_t n) noexcept {return i != 0 && (pqHasLeftChild(i, n) || pqHasRightChild(i, n));}
```
1. Se refactoriza para evitar tener operaciones aritméticas crudas (`2*i + 1`, `(i-1)/2`) esparcidas por todo el código. Usar funciones descriptivas previene errores de índices fuera de rango y hace que la lógica sea mucho más fácil de leer.
2. Usar `constexpr` es la alternativa moderna a las macros de C. A diferencia de las macros, `constexpr` respeta el tipado fuerte, los espacios de nombres y se evalúa en tiempo de compilación sin penalizar el rendimiento.
3. Si un nodo interno solo tiene hijo izquierdo (está en el borde del último nivel), la lógica asume a este hijo como el candidato a evaluar, evitando que el programa intente intente leer un hijo derecho que no existe y cause un fallo de memoria.
4. Un nodo es hoja cuando su índice está dentro de los límites del arreglo válido, pero la fórmula matemática de su hijo izquierdo ya sobrepasa el tamaño de dicho arreglo.
5. El costo asintótico se mantiene en `O(\log n)`. Estos cambios solo mejoran la sintaxis y la seguridad del código, pero las operaciones internas siguen ejecutándose en tiempo constante `O(1)`.

### Bloque 3 - Conteo en percolateUp

```cpp
// MOD-A6-B3: Variante instrumentada para medir la escalada del nuevo elemento
template <class T, class Compare>
std::size_t complHeapPercolateUpCount(std::vector<T>& a, std::size_t i, Compare comp) {
    std::size_t swaps = 0;
    while (pqHasParent(i)) {
        const std::size_t p = pqParent(i);
        if (!comp(a[p], a[i]))
            break;
        std::swap(a[p], a[i]);
        ++swaps;
        i = p;
    }
    return swaps;
}
```
```cpp
template <typename T, typename Compare = std::less<T>>
bool isVecHeap(const std::vector<T> &a, Compare comp = {})
{
  for (std::size_t i = 0; i < a.size(); ++i)
  {
    const std::size_t l = ods::pqLeftChild(i);
    const std::size_t r = ods::pqRightChild(i);
    if (l < a.size() && comp(a[i], a[l]))
      return false;
    if (r < a.size() && comp(a[i], a[r]))
      return false;
  }
  return true;
}

int main()
{
  std::vector<int> base{40, 10, 70, 30, 90, 20, 80, 60};
  std::vector<int> heap;
  std::less<int> comp;

  for (const int v : base) {
    heap.push_back(v);
    std::size_t swaps = ods::complHeapPercolateUpCount(heap, heap.size() - 1, comp);

    std::cout << "Valor insertado: " << v << std::endl;
    std::cout << "Intercambios: " << swaps << std::endl;
    printVector(heap, "Heap interno");
    std::cout << "Priopiedad heap: " << std::boolalpha << isVecHeap(heap) << std::endl;
  }
  return 0;
}
```

```
Priopiedad heap: true
Valor insertado: 70
Intercambios: 1
Heap interno: [70, 10, 40]
Priopiedad heap: true
Valor insertado: 30
Intercambios: 1
Heap interno: [70, 30, 40, 10]
Priopiedad heap: true
Valor insertado: 90
Intercambios: 2
Heap interno: [90, 70, 40, 10, 30]
Priopiedad heap: true
Valor insertado: 20
Intercambios: 0
Heap interno: [90, 70, 40, 10, 30, 20]
Priopiedad heap: true
Valor insertado: 80
Intercambios: 1
Heap interno: [90, 70, 80, 10, 30, 20, 40]
Priopiedad heap: true
Valor insertado: 60
Intercambios: 1
Heap interno: [90, 70, 80, 60, 30, 20, 40, 10]
Priopiedad heap: true
```


1. Se registran 0 intercambios si el elemento insertado es el primero (raíz) o si la prioridad del valor insertado ya respeta la regla del montículo frente a su padre directamente.
2. La cantidad máxima de intercambios está limitada por la altura del árbol, es decir, `O(\log n)`. Esto ocurre si se inserta un nuevo valor extremo que debe subir desde la última hoja hasta convertirse en la nueva raíz.
3. Al agregar el elemento al final del vector (como una hoja), su única trayectoria posible para recuperar la propiedad del Heap es hacia arriba (hacia la raíz). El camino más largo posible siempre será igual a la altura del árbol.
4. No hace falta comparar con el nodo hermano porque el Heap solo impone una jerarquía vertical (padre contra hijos). No exige ningún tipo de orden lateral entre hermanos de un mismo nivel.
5. Queda garantizado que la invariante de Heap se cumple, ya que el valor problemático sube hasta encontrar a un padre con mayor prioridad o hasta llegar a la cima.

### Bloque 4 - Conteo en percolateDown

```cpp
// MOD-A6-B4: Variante instrumentada para retornar el número de intercambios (swaps)
template <typename T, typename Compare>
std::size_t complHeapPercolateDownCount(std::vector<T>& a, std::size_t n, std::size_t i, Compare comp) {
    std::size_t swaps = 0;
    while (pqHasLeftChild(i, n)) {
        std::size_t c = pqLeftChild(i);
        const std::size_t r = pqRightChild(i);
        if (pqInHeap(r, n) && comp(a[c], a[r]))
            c = r;
        if (!comp(a[i], a[c]))
            break;
        std::swap(a[i], a[c]);
        ++swaps;
        i = c;
    }
    return swaps;
}
```

```cpp
int main() {
    std::vector<int> heap{40, 10, 70, 30, 90, 20, 80, 60};
    std::less<int> comp;

    while(!heap.empty()) {
        int max = heap.front();
        heap.front() = heap.back();
        heap.pop_back();
        std::vector<int> vecBeforeRepair = heap;

        std::size_t swaps = 0;
        if (!heap.empty())
            ods::complHeapPercolateDownCount(heap, heap.size(), 0, comp);
        std::cout << "Máximo eliminado: " << max << std::endl;
        printVector(vecBeforeRepair, "Antes de reparar");
        std::cout << "Intercambios: " << swaps << std::endl;
        printVector(heap, "Después de reparar");

        std::cout << "Propiedad heap: " << std::boolalpha << isVecHeap(heap) << std::endl;
    }
    return 0;
}
```

```
Máximo eliminado: 60
Antes de reparar: [10, 40, 30, 20]
Intercambios: 2
Después de reparar: [40, 20, 30, 10]
Propiedad heap: true
Máximo eliminado: 40
Antes de reparar: [10, 20, 30]
Intercambios: 1
Después de reparar: [30, 20, 10]
Propiedad heap: true
Máximo eliminado: 30
Antes de reparar: [10, 20]
Intercambios: 1
Después de reparar: [20, 10]
Propiedad heap: true
Máximo eliminado: 20
Antes de reparar: [10]
Intercambios: 0
Después de reparar: [10]
Propiedad heap: true
Máximo eliminado: 10
Antes de reparar: []
Intercambios: 0
Después de reparar: []
Propiedad heap: true
```

1. Tomar el último elemento y ponerlo en la raíz mantiene la forma de "árbol completo" del arreglo. Evita dejar huecos (fragmentación) en la memoria y permite seguir usando aritmética simple para navegar.
2. Ese elemento que se movió a la raíz ya no tiene padres, por lo que no tiene sentido intentar subirlo. Su única posible infracción es que sea menos prioritario que sus nuevos hijos, por lo que debe bajar.
3. Para mantener la propiedad, se compara primero cuál de los dos hijos tiene mayor prioridad. El padre temporal se intercambia siempre con el hijo más fuerte/prioritario.
4. Si solo hay un hijo (el izquierdo), simplemente se compara con ese. La estructura completa asegura que si falta un hijo, siempre será el derecho.
5. La operación consiste en intercambios simples que descienden por una sola rama. Como la altura del árbol es logarítmica, el costo máximo está acotado a `O(\log n)`.

### Bloque 5 - Validación de propiedad heap

```cpp
// MOD-A6-B5: Función libre para auditar exhaustivamente la integridad top-down del invariante de Heap
template <class T, class Compare>
bool complHeapIsValid(const std::vector<T>& a, Compare comp) {
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::size_t l = pqLeftChild(i);
        const std::size_t r = pqRightChild(i);
        if ((pqInHeap(l, a.size()) && comp(a[i], a[l])) || (pqInHeap(r, a.size()) && comp(a[i], a[r])))
            return false;
    }
    return true;
}
```

```cpp
template <typename T, typename Compare = std::less<T>>
void test_heap(const std::vector<T>& a, Compare comp = Compare{}) {
    ods::PQ_ComplHeap<T, Compare> heap;
    for (const T v : a) {
        heap.insert(v);
    }
    assert(ods::complHeapIsValid(heap.data(), comp));
}

int main() {
    test_heap<int>({});
    test_heap<int>({4});
    test_heap<int>({3, 3, 3});
    
    test_heap<int>({3, 1, 7, 8, 4, 5});

    ods::PQ_ComplHeap<int> heapifiedHeap({40, 10, 70, 30, 90, 20, 80, 60});
    assert(ods::complHeapIsValid(heapifiedHeap.data(), std::less<int>{}));
    
    // TODO: Change asserts to prints
    while(!heapifiedHeap.empty()) {
        heapifiedHeap.delMax();
        assert(ods::complHeapIsValid(heapifiedHeap.data(), std::less<int>{}));
    }
    return 0;
}
```

1. La validación recorre linealmente el arreglo, verificando que para cada nodo "padre", sus valores tengan mayor o igual prioridad que los de sus hijos izquierdo y derecho (si existen), según el comparador usado.
2. Gracias a la propiedad transitiva, no es necesario comparar un nodo con sus nietos. Si el Padre vence al Hijo, y el Hijo vence al Nieto, lógicamente el Padre también vence al Nieto.
3. Validar de forma exhaustiva contra todos los descendientes elevaría la complejidad a `O(n^2)` sin aportar mayor seguridad, siendo un desperdicio de recursos.
4. Al hacer un único recorrido evaluando nodos vecinos mediante aritmética simple, el costo total es lineal `O(n)`.
5. Sirve como herramienta de auditoría para pruebas automatizadas (tests). No debe usarse en producción dentro de cada inserción o extracción, ya que degradaría el rendimiento de las operaciones que normalmente toman `O(\log n)` a `O(n)`.

### Bloque 6 - Inserciones sucesivas vs Floyd

```cpp
int main() {
    std::vector<int> data{17, 3, 11, 9, 6, 14, 1, 8, 5, 2};

    std::vector<int> a = data;
    printVector(a,  "antes de heapify");
    ods::complHeapHeapifyFloyd(a, std::less<int>{});
    printVector(a, "después de heapify");

    std::vector<int> b{};
    for (const int v : data) {
        ods::complHeapInsert(b, v, std::less<int>{});
    }
    printVector(b, "después de insert");

    std::cout << "Propiedad heap (heapify): " << std::boolalpha << ods::complHeapIsValid(a, std::less<int>{}) << std::endl;
    std::cout << "Propiedad heap (insert): " << std::boolalpha << ods::complHeapIsValid(b, std::less<int>{}) << std::endl;
    return 0;
}
```

```
antes de heapify: [17, 3, 11, 9, 6, 14, 1, 8, 5, 2]
después de heapify: [17, 9, 14, 8, 6, 11, 1, 3, 5, 2]
después de insert: [17, 9, 14, 8, 6, 11, 1, 3, 5, 2]
Propiedad heap (heapify): true
Propiedad heap (insert): true
```

1. Insertar repetidamente cuesta `O(n \log n)` porque cada elemento debe empujarse y eventualmente escalar. Por su parte, el método de Floyd consolida la estructura de abajo hacia arriba (`percolateDown`), logrando ordenar todo en un tiempo lineal `O(n)`.
2. El sobrecosto ocurre porque el método de inserción asume que el árbol va creciendo uno a uno y paga el precio de reacomodar elementos que terminarán moviéndose varias veces.
3. El enfoque Bottom-Up (Floyd) es más rápido porque la gran mayoría de los nodos de un árbol están en los niveles inferiores. Estos nodos tienen que viajar muy poca distancia hacia abajo (cero para las hojas), lo que hace que la suma total de operaciones promedie un comportamiento lineal.
4. El método arranca desde el índice `n/2 - 1`, que corresponde al último nodo interno del árbol, saltándose todas las hojas.
5. Una hoja, por definición, no tiene hijos. Un árbol de un solo nodo siempre es un Heap válido. Por tanto, no tiene sentido aplicarles `percolateDown` a las hojas.

### Bloque 7 - heapSort

```cpp
// MOD-A6-B7: Adaptación de heapSort para inyectar ordenamiento inverso sin fragmentar el arreglo
template <typename T, typename Compare = std::less<T>>
void heapSort(std::vector<T>& a, Compare comp, bool ascending) {
    if (!ascending)
        return heapSort(a, [&comp](const T& x, const T& y) {return comp(y, x);});

    if (a.size() < 2)
        return;
    complHeapHeapifyFloyd(a, comp);
    for (std::size_t n = a.size(); n > 1; --n) {
        std::swap(a[0], a[n - 1]);
        complHeapPercolateDown(a, n - 1, 0, comp);
    }
}
```

```cpp
int main() {
    std::vector<int> a{5, 1, 5, 3, 8, 2, 8, 0};
    std::vector<int> ascending = a, descending = a;

    ods::heapSort(ascending, std::less<int>{}, true);
    ods::heapSort(descending, std::less<int>{}, false);

    printVector(ascending, "Ascending");
    printVector(descending, "Descending");
    return 0;
}
```

```
Ascending: [0, 1, 2, 3, 5, 5, 8, 8]
Descending: [8, 8, 5, 5, 3, 2, 1, 0]
```

1. Optimiza el uso de memoria haciéndolo *in-situ*. Al extraer el máximo, lo coloca en la última posición disponible del arreglo y reduce el tamaño lógico del montículo, reciclando el mismo espacio sin requerir un vector adicional.
2. Al intercambiar la raíz (el máximo actual) con el final del arreglo: `std::swap(a[0], a[n - 1])`, aislando ese elemento recién colocado para que el siguiente `percolateDown` no lo toque.
3. Primero usa el método de Floyd `O(n)` para armar el heap inicial, y luego ejecuta $N$ extracciones, donde cada una toma `O(\log n)`. El costo dominante es `O(n \log n)`.
4. No es un algoritmo estable. Al realizar intercambios a larga distancia (de la raíz al final), pierde el orden relativo original en el que llegaron los elementos con valores duplicados.
5. Se usa `delMax` cuando se necesita procesar prioridades en tiempo real sobre la marcha (flujo de datos continuos). Se usa `heapSort` cuando ya se tiene toda la colección de datos desde el principio y solo se quiere ordenarla masivamente.

### Bloque 8 - Heap izquierdista

```cpp
// MOD-A6-B8: Estructura de auditoría interna para revisar el Null Path Length y el invariante Heap
struct Check {
    bool ok;
    int npl;
    std::size_t count;
};

Check check(Node* u) const {
    if (!u) return {true, 0, 0};

    const Check l = check(u->left);
    const Check r = check(u->right);

    const bool heapOk = (!u->left || !comp_(u->value, u->left->value)) &&
                        (!u->right || !comp_(u->value, u->right->value));
    const bool leftistOk = l.npl >= r.npl;
    const int expectedNpl = r.npl + 1;
    const bool nplOk = u->npl == expectedNpl;
    const std::size_t subtreeCount = 1 + l.count + r.count;

    return {l.ok && r.ok && heapOk && leftistOk && nplOk, expectedNpl, subtreeCount};
}

public:
// MOD-A6-B8: Interfaz pública para test unitarios de validación
bool isValidLeftHeap() const {
    Check result = check(root_);
    return result.ok && result.count == n_;
}
```

```cpp
template <typename Heap>
void printValidation(const Heap& heap, const char* label) {
    std::cout << label
              << " | size=" << heap.size()
              << " | valido=" << std::boolalpha
              << heap.isValidLeftHeap()
              << "\n";
}

int main() {
    ods::PQ_LeftHeap<int> a{7, 2, 9};
    ods::PQ_LeftHeap<int> b{1, 8, 3, 11};

    printVector(a.levelOrder(), "heap A antes del merge");
    printValidation(a, "validacion A");

    printVector(b.levelOrder(), "heap B antes del merge");
    printValidation(b, "validacion B");

    std::cout << "\nEjecutando A.merge(B)\n";
    a.merge(b);

    printVector(a.levelOrder(), "heap A despues del merge");
    printValidation(a, "validacion A despues del merge");

    std::cout << "B queda vacio: " << std::boolalpha << b.empty() << "\n";
    std::cout << "size de B: " << b.size() << "\n";
    printValidation(b, "validacion B despues del merge");

    std::cout << "\nEjecutando A.insert(10)\n";
    a.insert(10);

    printVector(a.levelOrder(), "A despues de insert(10)");
    printValidation(a, "validacion A despues de insert(10)");

    std::cout << "\nExtracciones con delMax: ";
    while (!a.empty()) {
        const int value = a.delMax();
        std::cout << value << ' ';

        if (!a.isValidLeftHeap()) {
            std::cout << "\nError: heap izquierdista invalido despues de delMax\n";
            return 1;
        }
    }

    std::cout << "\n";
    printValidation(a, "validacion A al final");
    return 0;
}
```

```
heap A antes del merge: [9, 7, 2]
validacion A | size=3 | valido=true
heap B antes del merge: [11, 8, 1, 3]
validacion B | size=4 | valido=true

Ejecutando A.merge(B)
heap A despues del merge: [11, 8, 9, 1, 3, 7, 2]
validacion A despues del merge | size=7 | valido=true
B queda vacio: true
size de B: 0
validacion B despues del merge | size=0 | valido=true

Ejecutando A.insert(10)
A despues de insert(10): [11, 8, 10, 1, 3, 9, 7, 2]
validacion A despues de insert(10) | size=8 | valido=true

Extracciones con delMax: 11 10 9 8 7 3 2 1 
validacion A al final | size=0 | valido=true
```

1. El Heap izquierdista está diseñado para que la operación fundamental sea mezclar o fusionar (`merge`).
2. Insertar un nuevo elemento es simplemente crear un mini-heap de un solo nodo y hacer un `merge` con el heap principal.
3. Eliminar el máximo consiste en retirar la raíz y hacer un `merge` entre sus hijos (el subárbol izquierdo y el derecho).
4. La propiedad obliga a que la longitud del camino nulo (NPL) del hijo izquierdo siempre sea mayor o igual a la del hijo derecho. Esto concentra el peso en la rama izquierda.
5. Al forzar que el árbol sea "pesado" a la izquierda, se garantiza que la rama derecha siempre sea muy corta (longitud logarítmica). Al hacer el `merge` recorriendo solo las ramas derechas, asegurando una fusión muy rápida en tiempo `O(\log n)`.

### Bloque 9 - Huffman

```cpp
struct HuffmanNode {
    char symbol{};
    int frequency{};
    // MOD-A6-B9: Criterio secundario determinista (desempate alfabético)
    char minSymbol{};
    std::shared_ptr<HuffmanNode> left{};
    std::shared_ptr<HuffmanNode> right{};

    HuffmanNode(char s, int f) : symbol(s), frequency(f), minSymbol(s) {}

    HuffmanNode(std::shared_ptr<HuffmanNode> a, std::shared_ptr<HuffmanNode> b)
        : symbol('\0'), frequency(a->frequency + b->frequency),
          left(std::move(a)), right(std::move(b)) {
        // MOD-A6-B9: Hereda el símbolo alfabéticamente menor al fusionar nodos
        minSymbol = std::min(left->minSymbol, right->minSymbol);
    }
    bool leaf() const noexcept { return !left && !right; }
};

struct HuffmanLowerFrequencyFirst {
    bool operator()(const std::shared_ptr<HuffmanNode>& a,
                    const std::shared_ptr<HuffmanNode>& b) const {
        // MOD-A6-B9: Aplicación del desempate determinista
        if (a->frequency != b->frequency) return a->frequency > b->frequency;
        return static_cast<unsigned char>(a->minSymbol) >
               static_cast<unsigned char>(b->minSymbol);
    }
};

inline void huffmanCollectCodes(const std::shared_ptr<HuffmanNode>& u,
                                const std::string& prefix,
                                std::unordered_map<char, std::string>& out) {
    if (!u) return;
    if (u->leaf()) {
        // MOD-A6-B9: Prevención de árbol unitario devolviendo cadena vacía (asignando "0")
        out[u->symbol] = prefix.empty() ? "0" : prefix;
        return;
    }
    huffmanCollectCodes(u->left, prefix + "0", out);
    huffmanCollectCodes(u->right, prefix + "1", out);
}
```

```cpp
namespace {

void printTrace(const std::vector<ods::HuffmanBuildStep>& steps) {
    std::cout << "Fusiones durante la construccion:\n";
    for (const auto& s : steps) {
        std::cout << "  (" << s.leftLabel << ':' << s.leftFrequency << ") + "
                  << "(" << s.rightLabel << ':' << s.rightFrequency << ") -> "
                  << s.mergedFrequency << '\n';
    }
}

void printCodeTable(const std::vector<ods::HuffmanSymbol>& alphabet,
                    const std::unordered_map<char, std::string>& codes) {
    std::cout << "\nTabla simbolo, frecuencia, codigo, longitud:\n";
    std::cout << "  simbolo  frecuencia  codigo  longitud\n";

    for (const auto& s : alphabet) {
        const std::string& code = codes.at(s.symbol);
        std::cout << "  " << std::setw(7) << s.symbol
                  << "  " << std::setw(10) << s.frequency
                  << "  " << std::setw(6) << code
                  << "  " << std::setw(8) << code.size()
                  << '\n';
    }
}

} // namespace

int main() {
    const std::vector<ods::HuffmanSymbol> alphabet{
        {'A', 5}, {'B', 5}, {'C', 10}, {'D', 10}, {'E', 20}
    };

    const auto steps = ods::huffmanBuildTrace(alphabet);
    printTrace(steps);

    const auto codes = ods::huffmanGenerateCodes(alphabet);
    const auto tree = ods::huffmanGenerateTree(alphabet);

    printCodeTable(alphabet, codes);

    std::cout << "\nPrefijo libre: " << std::boolalpha
              << ods::huffmanIsPrefixFree(codes) << '\n';

    std::cout << "Costo ponderado total: "
              << ods::huffmanWeightedPathLength(alphabet, codes) << '\n';

    const std::string text = "ABCDE";
    const std::string bits = ods::huffmanEncode(text, codes);
    const std::string decoded = ods::huffmanDecode(bits, tree);

    std::cout << "\nTexto original: " << text << '\n';
    std::cout << "Codificado: " << bits << '\n';
    std::cout << "Decodificado: " << decoded << '\n';

    assert(decoded == text);
    assert(ods::huffmanIsPrefixFree(codes));
    assert(ods::huffmanWeightedPathLength(alphabet, codes) == 110);

    // Prueba del caso límite (1 solo símbolo)
    const std::vector<ods::HuffmanSymbol> singleSymbol{{'X', 100}};
    const auto singleCodes = ods::huffmanGenerateCodes(singleSymbol);
    const auto singleTree = ods::huffmanGenerateTree(singleSymbol);

    std::cout << "\nCaso de un solo simbolo:\n";
    std::cout << "  X -> " << singleCodes.at('X') << '\n';

    const std::string singleText = "XXX";
    const std::string singleBits = ods::huffmanEncode(singleText, singleCodes);
    const std::string singleDecoded = ods::huffmanDecode(singleBits, singleTree);

    std::cout << "  texto original: " << singleText << '\n';
    std::cout << "  codificado: " << singleBits << '\n';
    std::cout << "  decodificado: " << singleDecoded << '\n';
    std::cout << "  prefijo libre: "
              << ods::huffmanIsPrefixFree(singleCodes) << '\n';

    assert(singleCodes.at('X') == "0");
    assert(singleDecoded == singleText);
    assert(ods::huffmanIsPrefixFree(singleCodes));

    return 0;
}
```

BORRAR ASSERTS
```
Fusiones durante la construccion:
  (A:5) + (B:5) -> 10
  (*:10) + (C:10) -> 20
  (D:10) + (*:20) -> 30
  (E:20) + (*:30) -> 50

Tabla simbolo, frecuencia, codigo, longitud:
  simbolo  frecuencia  codigo  longitud
        A           5    1100         4
        B           5    1101         4
        C          10     111         3
        D          10      10         2
        E          20       0         1

Prefijo libre: true
Costo ponderado total: 110

Texto original: ABCDE
Codificado: 11001101111100
Decodificado: ABCDE

Caso de un solo simbolo:
  X -> 0
  texto original: XXX
  codificado: 000
  decodificado: XXX
  prefijo libre: true
```

1. La cola de prioridad es ideal porque necesitamos obtener y remover constantemente los dos árboles (o caracteres) con menor frecuencia disponible, algo que el Heap hace eficientemente en `O(\log n)`.
2. Extrae los dos nodos con las frecuencias más bajas.
3. Se crea un nuevo nodo interno que agrupa a ambos, sumando sus frecuencias, y se vuelve a insertar en la cola de prioridad.
4. Si el alfabeto tiene un solo símbolo, el algoritmo nunca fusiona ramas, por lo que el árbol no tendría aristas. Por convención y para evitar fallos de lectura, se le asigna un bit estático como "0".
5. Un código "libre de prefijos" significa que la secuencia de bits de una letra nunca es el inicio de la secuencia de otra letra. Esto permite que el decodificador lea los bits de corrido y sepa exactamente cuándo termina una letra sin necesitar separadores especiales.
6. Cambiar la regla de empate puede invertir qué nodo va a la izquierda o a la derecha, generando una secuencia de bits distinta.
7. Sin importar la regla de desempate, siempre que se agrupen las frecuencias mínimas, el costo total ponderado del árbol será el mismo. La longitud óptima se mantiene, solo cambian los códigos asignados.

### Bloque 10 - Treap

```cpp
// MOD-A6-B10: Seguimiento de rotaciones ascendentes post-inserción
std::size_t bubbleUpCount(Node* u) {
    std::size_t rotations = 0;
    while(u->parent && u->parent->priority > u->priority) {
        if (u->isRightChild()) {
            rotateLeft(u->parent);
        } else {
            rotateRight(u->parent);
        }
        if (!u->parent) root_ = u;
        ++rotations;
    }
    return rotations;
}
// MOD-A6-B10: Seguimiento de rotaciones descendentes pre-corte
std::size_t trickleDownCount(Node* u) {
    std::size_t rotations = 0;
    while (u->left || u-> right) {
        if (!u->left)
            rotateLeft(u);
        else if (!u->right)
            rotateRight(u);
        else if (u->left->priority < u->right->priority)
            rotateRight(u);
        else
            rotateLeft(u);
        if (root_ == u) root_ = u->parent;
        ++rotations;
    }
    return rotations;
}

public:
// MOD-A6-B10: Envolturas públicas para obtener recuentos operacionales
std::size_t addWithPriorityCount(const T& x, std::uint64_t priority) {
    Node* u = new Node(x, priority);
    if (!addNode(u)) {
        delete u;
        return 0;
    }
    std::size_t rotations = bubbleUpCount(u);
    ++size_;
    return rotations;
}
std::size_t removeCount(const T& x) {
    Node* u = findEQ(x);
    if (!u) return 0;
    std::size_t rotations = trickleDownCount(u);
    splice(u);
    delete u;
    return rotations;
}

// MOD-A6-B10: Interfaz para confirmar la indemnidad topológica del árbol
bool hasValidParentLinks() const {
    return checkParents(root_, nullptr);
}
```

```cpp
namespace {

template <class T>
void printVector(const std::vector<T>& xs, const std::string& label) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < xs.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << xs[i];
    }
    std::cout << "]\n";
}

void printState(const ods::Treap<int>& t, const std::string& label) {
    std::cout << "\n" << label << "\n";

    printVector(t.inorderKeys(), "inorden");
    printVector(t.levelOrderKeys(), "niveles");

    if (t.root()) {
        std::cout << "raiz: " << t.root()->key
                  << " | prioridad: " << t.root()->priority << "\n";
    } else {
        std::cout << "raiz: null\n";
    }

    std::cout << "isBST: " << std::boolalpha << t.isBST() << "\n";
    std::cout << "isHeapByPriority: " << t.isHeapByPriority() << "\n";
    std::cout << "isTreap: " << t.isTreap() << "\n";
    std::cout << "parent_links: " << t.hasValidParentLinks() << "\n";
}

void printSearchResult(const std::string& name, const ods::Treap<int>::Node* node) {
    std::cout << name << ": ";
    if (node) {
        std::cout << node->key << " | prioridad=" << node->priority << "\n";
    } else {
        std::cout << "null\n";
    }
}

} // namespace

int main() {
    ods::Treap<int> t(232);

    const std::vector<std::pair<int, std::uint64_t>> fixed{
        {50, 50}, {30, 30}, {70, 70},
        {20, 20}, {40, 40}, {60, 60}, {80, 80}
    };

    for (const auto& [key, priority] : fixed) {
        t.addWithPriority(key, priority);

        std::cout << "\ninsertada clave=" << key
                  << " prioridad=" << priority << "\n";
        printState(t, "estado despues de insertar");
    }

    std::cout << "\nArbol final Parte A:\n";
    std::cout << t.asciiArt() << "\n";

    std::cout << "\nBloque 10 - Parte B: bubbleUpCount\n";
    ods::Treap<int> b(232);
    const std::vector<std::pair<int, std::uint64_t>> bubbleSeq{
        {100, 100}, {90, 90}, {80, 80}, {70, 70}, {60, 60}
    };

    for (const auto& [key, priority] : bubbleSeq) {
        const std::size_t rotations = b.addWithPriorityCount(key, priority);

        std::cout << "clave=" << key
                  << " prioridad=" << priority
                  << " rotaciones=" << rotations
                  << " raiz=" << b.root()->key << "\n";
        printVector(b.levelOrderKeys(), "niveles");
    }

    std::cout << "\nBloque 10 - Parte C: trickleDownCount y removeCount\n";
    ods::Treap<int> c(232);
    for (const auto& [key, priority] : fixed) {
        c.addWithPriority(key, priority);
    }

    for (int key : {50, 20, 70}) {
        const std::size_t rotations = c.removeCount(key);

        std::cout << "\neliminada clave=" << key
                  << " rotaciones=" << rotations << "\n";
        printState(c, "estado despues de eliminar");
    }

    std::cout << "\nBloque 10 - Parte D: busqueda ordenada\n";
    ods::Treap<int> d(232);
    for (const auto& [key, priority] : fixed) {
        d.addWithPriority(key, priority);
    }

    printSearchResult("findEQ(40)", d.findEQ(40));
    printSearchResult("findEQ(35)", d.findEQ(35));
    printSearchResult("lowerBound(35)", d.lowerBound(35));
    printSearchResult("lowerBound(40)", d.lowerBound(40));
    printSearchResult("upperBound(40)", d.upperBound(40));
    printSearchResult("upperBound(75)", d.upperBound(75));

    return 0;
}
```

```
insertada clave=50 prioridad=50

estado despues de insertar
inorden: [50]
niveles: [50]
raiz: 50 | prioridad: 50
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=30 prioridad=30

estado despues de insertar
inorden: [30, 50]
niveles: [30, 50]
raiz: 30 | prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=70 prioridad=70

estado despues de insertar
inorden: [30, 50, 70]
niveles: [30, 50, 70]
raiz: 30 | prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=20 prioridad=20

estado despues de insertar
inorden: [20, 30, 50, 70]
niveles: [20, 30, 50, 70]
raiz: 20 | prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=40 prioridad=40

estado despues de insertar
inorden: [20, 30, 40, 50, 70]
niveles: [20, 30, 40, 50, 70]
raiz: 20 | prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=60 prioridad=60

estado despues de insertar
inorden: [20, 30, 40, 50, 60, 70]
niveles: [20, 30, 40, 50, 60, 70]
raiz: 20 | prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=80 prioridad=80

estado despues de insertar
inorden: [20, 30, 40, 50, 60, 70, 80]
niveles: [20, 30, 40, 50, 60, 70, 80]
raiz: 20 | prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

Arbol final Parte A:
│                       ┌── 80|p=80
│                   ┌── 70|p=70
│               ┌── 60|p=60
│           ┌── 50|p=50
│       ┌── 40|p=40
│   ┌── 30|p=30
└── 20|p=20


Bloque 10 - Parte B: bubbleUpCount
clave=100 prioridad=100 rotaciones=0 raiz=100
niveles: [100]
clave=90 prioridad=90 rotaciones=1 raiz=90
niveles: [90, 100]
clave=80 prioridad=80 rotaciones=1 raiz=80
niveles: [80, 90, 100]
clave=70 prioridad=70 rotaciones=1 raiz=70
niveles: [70, 80, 90, 100]
clave=60 prioridad=60 rotaciones=1 raiz=60
niveles: [60, 70, 80, 90, 100]

Bloque 10 - Parte C: trickleDownCount y removeCount

eliminada clave=50 rotaciones=1

estado despues de eliminar
inorden: [20, 30, 40, 60, 70, 80]
niveles: [20, 30, 40, 60, 70, 80]
raiz: 20 | prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

eliminada clave=20 rotaciones=1

estado despues de eliminar
inorden: [30, 40, 60, 70, 80]
niveles: [30, 40, 60, 70, 80]
raiz: 30 | prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

eliminada clave=70 rotaciones=1

estado despues de eliminar
inorden: [30, 40, 60, 80]
niveles: [30, 40, 60, 80]
raiz: 30 | prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

Bloque 10 - Parte D: busqueda ordenada
findEQ(40): 40 | prioridad=40
findEQ(35): null
lowerBound(35): 40 | prioridad=40
lowerBound(40): 40 | prioridad=40
upperBound(40): 50 | prioridad=50
upperBound(75): 80 | prioridad=80
```

**Parte A y B:**
1. Las prioridades solo deciden quién va más arriba o abajo (vertical). El orden lateral siempre respeta las reglas clásicas del BST (menores a la izquierda, mayores a la derecha).
2. La raíz termina siendo el elemento al que la función aleatoria le asignó la mayor prioridad (o menor, dependiendo del comparador) de todo el conjunto.
3. Al insertarse inicialmente como hoja en un BST, si su prioridad aleatoria es más alta que la de su padre, debe aplicar un reacomodo hacia arriba (`bubbleUp`).
4. Para subir de nivel sin romper el orden lateral, usa rotaciones (`rotateLeft` o `rotateRight`), que ajustan la altura preservando el orden de búsqueda.
5. Reparan los punteros del hijo y del padre tras una inserción que violó el invariante vertical.

**Parte C y D:**
1. En un BST normal, se podría borrar un nodo interno buscando a su sucesor. En un Treap, hacer eso directamente rompería la coherencia de prioridades con los subárboles sobrantes.
2. Antes de borrar un nodo, se fuerza a descender (`trickleDown`) rotándolo con su hijo de mayor prioridad, hasta empujarlo a la posición de hoja, donde puede cortarse sin afectar a nadie.
3. Las funciones de rango (como `findEQ`, `lowerBound`) son exactamente iguales a las de un Árbol de Búsqueda Binaria normal. Simplemente navegan comparando claves sin importarles el valor de la prioridad.
4. El Treap evita la complejidad de implementar los engorrosos casos de rebalanceo de un árbol AVL o Red-Black. Delega el equilibrio a la probabilidad estadística, consiguiendo tiempos esperados `O(\log n)`.
5. Errores típicos incluyen olvidar reasignar los punteros del padre (`parent`) después de una rotación, causando árboles huérfanos, o realizar una rotación hacia el lado equivocado, destruyendo la lateralidad del BST. Usar semillas estáticas en los tests ayuda a atrapar estos errores con entornos predecibles.

### Bloque 11 - Comparación con Semana 5
[Tabla y selección de estructura]

### Bloque 12 - Pruebas
[Lista de pruebas, salida de ctest y explicación]

### Bloque 13 - Defensa escrita
[Respuesta final]