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
inline constexpr bool pqIsLeaf(std::size_t i, std::size_t n) noexcept {return !(pqHasLeftChild(i, n)||pqHasRightChild(i, n));}
inline constexpr bool pqIsInternal(std::size_t i, std::size_t n) noexcept {return i != 0 && (pqHasLeftChild(i, n)||pqHasRightChild(i, n));}
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
        if ((pqInHeap(l, a.size()) && comp(a[i], a[l]))||(pqInHeap(r, a.size()) && comp(a[i], a[r])))
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

    const bool heapOk = (!u->left||!comp_(u->value, u->left->value)) &&
                        (!u->right||!comp_(u->value, u->right->value));
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
              << "|size=" << heap.size()
              << "|valido=" << std::boolalpha
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
validacion A|size=3|valido=true
heap B antes del merge: [11, 8, 1, 3]
validacion B|size=4|valido=true

Ejecutando A.merge(B)
heap A despues del merge: [11, 8, 9, 1, 3, 7, 2]
validacion A despues del merge|size=7|valido=true
B queda vacio: true
size de B: 0
validacion B despues del merge|size=0|valido=true

Ejecutando A.insert(10)
A despues de insert(10): [11, 8, 10, 1, 3, 9, 7, 2]
validacion A despues de insert(10)|size=8|valido=true

Extracciones con delMax: 11 10 9 8 7 3 2 1 
validacion A al final|size=0|valido=true
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
    while (u->left||u-> right) {
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
                  << "|prioridad: " << t.root()->priority << "\n";
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
        std::cout << node->key << "|prioridad=" << node->priority << "\n";
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
raiz: 50|prioridad: 50
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=30 prioridad=30

estado despues de insertar
inorden: [30, 50]
niveles: [30, 50]
raiz: 30|prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=70 prioridad=70

estado despues de insertar
inorden: [30, 50, 70]
niveles: [30, 50, 70]
raiz: 30|prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=20 prioridad=20

estado despues de insertar
inorden: [20, 30, 50, 70]
niveles: [20, 30, 50, 70]
raiz: 20|prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=40 prioridad=40

estado despues de insertar
inorden: [20, 30, 40, 50, 70]
niveles: [20, 30, 40, 50, 70]
raiz: 20|prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=60 prioridad=60

estado despues de insertar
inorden: [20, 30, 40, 50, 60, 70]
niveles: [20, 30, 40, 50, 60, 70]
raiz: 20|prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

insertada clave=80 prioridad=80

estado despues de insertar
inorden: [20, 30, 40, 50, 60, 70, 80]
niveles: [20, 30, 40, 50, 60, 70, 80]
raiz: 20|prioridad: 20
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
raiz: 20|prioridad: 20
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

eliminada clave=20 rotaciones=1

estado despues de eliminar
inorden: [30, 40, 60, 70, 80]
niveles: [30, 40, 60, 70, 80]
raiz: 30|prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

eliminada clave=70 rotaciones=1

estado despues de eliminar
inorden: [30, 40, 60, 80]
niveles: [30, 40, 60, 80]
raiz: 30|prioridad: 30
isBST: true
isHeapByPriority: true
isTreap: true
parent_links: true

Bloque 10 - Parte D: busqueda ordenada
findEQ(40): 40|prioridad=40
findEQ(35): null
lowerBound(35): 40|prioridad=40
lowerBound(40): 40|prioridad=40
upperBound(40): 50|prioridad=50
upperBound(75): 80|prioridad=80
```

**Parte A**
1. Las prioridades dictaminan estrictamente la jerarquía vertical (quién es padre o hijo), mientras que las claves (`key`) controlan exclusivamente la disposición lateral (menores a la izquierda, mayores a la derecha). El Treap respeta ambas invariantes en simultáneo.
2. La raíz termina siendo, de forma innegociable, el elemento con la prioridad matemática más dominante de todo el conjunto de datos insertado.
3. Al insertarse un nuevo nodo como hoja (siguiendo las normas del BST), si su valor de prioridad asignado es más dominante que el de su padre, rompe la invariante vertical del montículo y debe ser empujado hacia arriba (`bubbleUp`).
4. El nodo escala niveles a través de rotaciones locales (`rotateLeft` o `rotateRight`). Estas operaciones intercambian las paternidades ajustando la altura sin quebrar en ningún momento el orden transversal del árbol de búsqueda.
5. Principalmente los punteros de los descendientes (`left`, `right`) y del ancestro superior (`parent`), asegurando que la malla de memoria del grafo se suture correctamente tras el giro direccional.

**Parte B**
1. Inyectar valores con prioridad extrema dominante obliga a un reacomodo ascendente forzoso. La clave asume la supremacía topológica abriéndose paso hacia la raíz rotando en contrasentido con sus predecesores directos.
2. La escalada se frena en seco cuando el nodo emergente topa con un padre cuya prioridad sí legitima su dominancia jerárquica frente a él, o en su defecto, cuando alcanza la cúspide (raíz absoluta).
3. El costo temporal esperado es `O(\log n)` gracias a la dispersión probabilística. Sin embargo, en un escenario matemáticamente adverso o degenerado, el costo límite será `O(h)`.
4. El pivoteo transfiere subárboles enteros (el hijo interno cambia de tutela), pero conserva impecable la frontera paramétrica: los valores menores se mantienen aislados a la izquierda y los mayores a la derecha.
5. Inyectar prioridades aleatorias estocásticas rompe los patrones de ingreso ordenados que causarían degeneraciones topológicas (árboles lineales). Permite un balance orgánico con eficiencia logarítmica sin el aplastante costo operacional de mantener estructuras rígidamente balanceadas como los AVL.

**Parte C**
1. Suprimir abruptamente un bloque interno con dos descendientes activos desarticularía la convergencia de prioridades en los subárboles remanentes. Es obligatorio forzar el hundimiento del nodo (`trickleDown`) hacia la periferia (como hoja o nodo de un solo hijo) antes de amputarlo con seguridad.
2. Se evalúa a ambos descendientes del nodo a descartar y se elige aquel que posea la prioridad más alta. Este hijo es el único capacitado para tomar la corona local sin quedar subyugado erróneamente por el otro hermano.
3. Ejerce obligatoriamente un giro hacia la derecha (`rotateRight`), ascendiendo al hijo izquierdo.
4. Ejerce obligatoriamente un giro hacia la izquierda (`rotateLeft`), relevando la posición y ascendiendo al hijo derecho.
5. Múltiples factores críticos: el mantenimiento de la invariante inorden (claves), la rectificación de la invariante de Heap (prioridades), la integridad de todos los enlaces `parent` en la estela de rotación, el reanclaje del puntero global `root_` si cambió la cima, y el descuento aritmético de la cota `size_`.

**Parte D**
1. Las operaciones de búsqueda horizontal barren la topología consultando exclusivamente las magnitudes de las llaves (`key`). Son ajenas e independientes de las alteraciones morfológicas impuestas por el vector vertical `priority`.
2. Aquellas rutinas que exigen navegabilidad determinista sobre la magnitud de datos, tales como `findEQ`, `lowerBound`, `upperBound` y la recolección transversal secuencial (`inorderKeys()`).
3. El núcleo de superioridad vertical entre progenitores y vástagos, la cual se audita de forma exhaustiva mediante la rutina asertiva `isHeapByPriority()`.
4. El Treap demanda sobrecostos inherentes a su naturaleza de árbol enlazado: punteros, asignación de memoria dinámica no contigua y ciclos de rotación de punteros. Para una purga masiva donde solo importa destilar máximos, el mapeo indexado sobre arreglos de `PQ_ComplHeap` aniquila estos tiempos por su aprovechamiento absoluto de la memoria caché.
5. Resulta la arquitectura de elección cuando la operativa exige un híbrido implacable: rangos de consulta estructurados y flujos continuos de inserción/eliminación sin la abrumadora complejidad algorítmica y los estrictos cuellos de botella rotacionales de un Red-Black Tree.
6. Errores catastróficos abarcan obviar la reasignación simétrica de los apuntadores `parent` tras rotar (desatando fugas de memoria o *segmentation faults* directos) o pivotear en el sentido inverso, lo cual destruye la invariante inorden. Fomentar pruebas sobre semillas de pseudo-aleatoriedad estáticas instaura ecosistemas reproducibles que permiten atrapar estas fisuras lógicas sin depender de la suerte.

### Bloque 11 - Comparación con Semana 5
```cpp
int main() {
    cout << "=========================================================\n";
    cout << " Bloque 11 - Comparativa: Semana 5 vs Semana 6\n";
    cout << "=========================================================\n\n";

    // Conjunto de datos base para las pruebas
    vector<int> datos = {50, 20, 70, 10, 30, 60, 80, 5, 15, 90};
    
    cout << "Datos de prueba inyectados: ";
    printVector(datos, "Original");
    cout << "\n";

    /* ------------------------------------------------------------------
     * 1. BinaryHeap (Semana 5) - Montículo tradicional
     * ------------------------------------------------------------------ */
    cout << "--- 1. BinaryHeap (Semana 5) ---\n";
    ods::BinaryHeap<int> bh;
    for (int x : datos) bh.add(x);
    
    cout << "Propiedad: Prioridad Pura (Monticulo base)\n";
    cout << "Extrayendo minimos O(log n): [";
    while(bh.size() > 0) {
        cout << bh.remove() << (bh.size() > 0 ? ", " : "");
    }
    cout << "]\nIneficiencia: Carente de metodos de busqueda lateral (find).\n\n";

    /* ------------------------------------------------------------------
     * 2. PQ_ComplHeap (Semana 6) - Montículo implícito en arreglo
     * ------------------------------------------------------------------ */
    cout << "--- 2. PQ_ComplHeap (Semana 6) ---\n";
    ods::PQ_ComplHeap<int, std::greater<int>> pq; // Usamos greater para simular min-heap
    for (int x : datos) pq.insert(x);
    
    cout << "Propiedad: Monticulo Implicito Contiguo (Alta localidad en Cache)\n";
    // Utilizamos el validador instrumentado que hiciste en el Bloque 5
    cout << "Validacion Interna de Invariante: " << boolalpha << ods::complHeapIsValid(pq.data(), std::greater<int>{}) << "\n";
    cout << "Extrayendo prioridades (delMax): [";
    while(!pq.empty()) {
        cout << pq.delMax() << (pq.empty() ? "" : ", ");
    }
    cout << "]\n\n";

    /* ------------------------------------------------------------------
     * 3. BinarySearchTree (Semana 5) - Árbol de búsqueda puro
     * ------------------------------------------------------------------ */
    cout << "--- 3. BinarySearchTree (Semana 5) ---\n";
    ods::BinarySearchTree<int> bst;
    for (int x : datos) bst.add(x);
    
    cout << "Propiedad: Lateralidad estricta (Busqueda de rangos)\n";
    cout << "Busqueda findEQ(30): " << (bst.findEQ(30) ? "Exito" : "Fallo") << "\n";
    cout << "Busqueda lowerBound(65): " << bst.lowerBound(65) << "\n";
    cout << "Ineficiencia: Se degrada a O(n) si se insertan datos ya ordenados.\n\n";

    /* ------------------------------------------------------------------
     * 4. Treap (Semana 6) - Simbiosis BST + Heap
     * ------------------------------------------------------------------ */
    cout << "--- 4. Treap (Semana 6) ---\n";
    ods::Treap<int> treap(232); // Inicializado con semilla estatica para testing
    for (int x : datos) treap.add(x); // Genera prioridades internas aleatorias
    
    cout << "Propiedad: Simbiosis BST (Lateral) + Heap por Prioridad (Vertical)\n";
    printVector(treap.inorderKeys(), "Recorrido Inorden (Claves)");
    
    // Invocando las validaciones estrictas del Bloque 10
    cout << "Auditoria de Invariantes:\n";
    cout << "  -> isBST (Orden lateral): " << boolalpha << treap.isBST() << "\n";
    cout << "  -> isHeapByPriority (Prioridad Vertical): " << treap.isHeapByPriority() << "\n";
    cout << "  -> hasValidParentLinks (Ausencia de orfandad): " << treap.hasValidParentLinks() << "\n";
    
    cout << "Consultas hibridas:\n";
    cout << "  -> findEQ(30): " << (treap.findEQ(30) ? "Encontrado" : "No encontrado") << "\n";
    
    // Probamos el remove instrumentado
    size_t rotations = treap.removeCount(70);
    cout << "  -> trickleDownCount al eliminar(70): " << rotations << " rotaciones ejecutadas.\n";
    cout << "  -> isBST post-eliminacion: " << treap.isBST() << "\n";

    cout << "\n=========================================================\n";
    cout << " Conclusion demostrada: El Treap retiene la navegacion de \n";
    cout << " busqueda del BST, blindandose de la degeneracion O(n) al \n";
    cout << " acatar las prioridades estocasticas de la estructura Heap.\n";
    cout << "=========================================================\n";

    return 0;
}
```

```
=========================================================
 Bloque 11 - Comparativa: Semana 5 vs Semana 6
=========================================================

Datos de prueba inyectados: Original: [50, 20, 70, 10, 30, 60, 80, 5, 15, 90]

--- 1. BinaryHeap (Semana 5) ---
Propiedad: Prioridad Pura (Monticulo base)
Extrayendo minimos O(log n): [5, 10, 15, 20, 30, 50, 60, 70, 80, 90]
Ineficiencia: Carente de metodos de busqueda lateral (find).

--- 2. PQ_ComplHeap (Semana 6) ---
Propiedad: Monticulo Implicito Contiguo (Alta localidad en Cache)
Validacion Interna de Invariante: true
Extrayendo prioridades (delMax): [5, 10, 15, 20, 30, 50, 60, 70, 80, 90]

--- 3. BinarySearchTree (Semana 5) ---
Propiedad: Lateralidad estricta (Busqueda de rangos)
Busqueda findEQ(30): Exito
Busqueda lowerBound(65): 0x6086508d2830
Ineficiencia: Se degrada a O(n) si se insertan datos ya ordenados.

--- 4. Treap (Semana 6) ---
Propiedad: Simbiosis BST (Lateral) + Heap por Prioridad (Vertical)
Recorrido Inorden (Claves): [5, 10, 15, 20, 30, 50, 60, 70, 80, 90]
Auditoria de Invariantes:
  -> isBST (Orden lateral): true
  -> isHeapByPriority (Prioridad Vertical): true
  -> hasValidParentLinks (Ausencia de orfandad): true
Consultas hibridas:
  -> findEQ(30): Encontrado
  -> trickleDownCount al eliminar(70): 1 rotaciones ejecutadas.
  -> isBST post-eliminacion: true

=========================================================
 Conclusion demostrada: El Treap retiene la navegacion de 
 busqueda del BST, blindandose de la degeneracion O(n) al 
 acatar las prioridades estocasticas de la estructura Heap.
=========================================================
```

|Estructura|Operación principal|Propiedad mantenida|Operación eficiente|Operación que no conviene|Evidencia producida por la demostración|
|-------|-------|-------|-------|-------|-------|
|**`BinaryHeap` (Semana 5)**|Extracción del máximo/mínimo|Invariante vertical pura (Padre $\ge$ Hijos)|`remove()` / `add()` en `O(\log n)`|Búsqueda lateral o inorden `O(n)`|Trazas de `remove` ordenadas, fallos al intentar ubicar rangos.|
|**`PQ_ComplHeap` (Semana 6)**|Gestión de cola de prioridad sobre vector|Montículo implícito contiguo|`delMax()` y localidad en caché `O(1)`|Iteraciones secuenciales crecientes|Extracciones fluidas y validación estricta de jerarquías sobre índices.|
|**`BinarySearchTree` (Sem 5)**|Búsqueda ordenada y de rangos|Lateralidad estricta (L $<$ P $<$ R)|`lowerBound` / `upperBound` en `O(h)`|Extracción masiva continua de extremos|Consultas `findEQ` y `lowerBound` exitosas y directas.|
|**`Treap` (Semana 6)**|Simbiosis BST y Heap|Orden lateral (BST) + Prioridad (Heap)|Rebalanceo orgánico `O(\log n)`|Operaciones sobre caché exigente (punteros dispersos)|Mantenimiento simultáneo de `isBST` y `isHeapByPriority` tras inserciones.|

1. El heap de prioridad restringe los datos de forma vertical (un padre siempre es mayor/menor que sus hijos) sin correlación entre hermanos. El árbol de búsqueda restringe los datos de forma lateral (todo lo izquierdo es menor, todo lo derecho es mayor), permitiendo ubicar valores exactos descartando mitades lógicas.
2.  Porque el BST garantiza un orden absoluto global de izquierda a derecha. En un heap, un valor grande puede alojarse arbitrariamente tanto en la profundidad de la rama izquierda como en la derecha, haciendo imposible un recorrido unidireccional creciente.
3. Agrega eficiencia a nivel de hardware. Al proyectar el montículo sobre un arreglo implícito contiguo (`std::vector`) y usar funciones precompiladas (`constexpr`) para la aritmética de fronteras, maximiza la localidad en caché y elimina la latencia de punteros dinámicos.
4. Combina la navegabilidad lateral estricta de un BST (para las claves o *keys*) con la dominancia vertical de un Heap (asignada mediante una prioridad aleatoria), utilizando rotaciones para satisfacer ambas reglas en paralelo.
5.  Utilizaría `PQ_ComplHeap`, dado que su arquitectura sobre vectores contiguos domina los tiempos de ejecución para purgas continuas de la cima.
6.  Utilizaría un `BinarySearchTree` (o su variante balanceada), ya que su naturaleza lateral es la única capaz de descartar subárboles para ubicar fronteras numéricas exactas.
7.  Utilizaría el `Treap`. Evita la complejidad de codificar rebalanceadores estrictos (AVL o Red-Black) y delega el equilibrio a distribuciones estadísticas de prioridad, conservando asintóticas eficientes `O(\log n)`.

La elección arquitectónica depende del flujo utilitario: para purgas masivas y estáticas de extremos, el arreglo implícito de `PQ_ComplHeap` rige incontestable; para auditorías analíticas ordenadas y escaneos de rangos discretos con inserciones concurrentes, el modelo híbrido probabilístico del `Treap` se impone al evitar degeneraciones lineales.

### Bloque 12 - Pruebas

```cpp
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

#include "Capitulo5.h"
#include "Capitulo6.h"

int main() {
  // PQ_ComplHeap: secuencia completa de extracciones.
  ods::PQ_ComplHeap<int> pq;
  for (int x : {8, 3, 10, 1, 6, 14, 4, 7, 13, 14}) {
    pq.insert(x);
    assert(pq.isHeap());
  }
  std::vector<int> out;
  while (!pq.empty()) {
    out.push_back(pq.delMax());
    assert(pq.empty()||pq.isHeap());
  }
  assert((out == std::vector<int>{14, 14, 13, 10, 8, 7, 6, 4, 3, 1}));

  // Leftist heap: merge e invariantes.
  ods::PQ_LeftHeap<int> a{20, 7, 18, 3};
  ods::PQ_LeftHeap<int> b{19, 8, 4, 1, 17};
  a.merge(b);
  assert(a.isLeftistHeap());
  assert(b.empty());
  std::vector<int> leftOut;
  while (!a.empty()) {
    leftOut.push_back(a.delMax());
    assert(a.empty()||a.isLeftistHeap());
  }
  assert((leftOut == std::vector<int>{20, 19, 18, 17, 8, 7, 4, 3, 1}));

  // Huffman con heap completo y leftist heap deben tener mismo costo ponderado.
  const std::vector<ods::HuffmanSymbol> s{{'a', 45}, {'b', 13}, {'c', 12},
                                          {'d', 16}, {'e', 9},  {'f', 5}};
  const auto codes1 = ods::huffmanGenerateCodes(s);
  const auto codes2 = ods::huffmanGenerateCodesLeftHeap(s);
  assert(ods::huffmanIsPrefixFree(codes1));
  assert(ods::huffmanIsPrefixFree(codes2));
  assert(ods::huffmanWeightedPathLength(s, codes1) == 224);
  assert(ods::huffmanWeightedPathLength(s, codes2) == 224);

  // Rotaciones BST: preservan inorder aun cuando cambie la forma.
  ods::BinarySearchTree<int> bst;
  for (int x : {8, 3, 10, 1, 6, 14, 4, 7, 13}) {
    bst.add(x);
  }
  auto sorted = bst.inorder();
  bst.rotateLeft(bst.root());
  assert(bst.isBST());
  assert(bst.inorder() == sorted);
  bst.rotateRight(bst.root());
  assert(bst.isBST());
  assert(bst.inorder() == sorted);

  // Treap: BST por clave + heap por prioridad.
  ods::Treap<int> treap(555);
  treap.addWithPriority(8, 80);
  treap.addWithPriority(3, 40);
  treap.addWithPriority(10, 90);
  treap.addWithPriority(1, 20);
  treap.addWithPriority(6, 70);
  treap.addWithPriority(14, 120);
  treap.addWithPriority(4, 65);
  treap.addWithPriority(7, 68);
  assert(treap.isTreap());
  assert((treap.inorderKeys() == std::vector<int>{1, 3, 4, 6, 7, 8, 10, 14}));
  assert(treap.lowerBound(5)->key == 6);
  assert(treap.upperBound(6)->key == 7);
  assert(treap.remove(3));
  assert(treap.remove(8));
  assert(treap.isTreap());
  assert((treap.inorderKeys() == std::vector<int>{1, 4, 6, 7, 10, 14}));
}
```
```cpp
#include <vector>
#include <cassert>
#include <string>

#include "Capitulo6.h"

using namespace std;
using namespace ods;

void test_complheap_invariants() {
    PQ_ComplHeap<int> heap;
    vector<int> inputs = {45, 20, 80, 10, 90, 30, 60, 50, 70};

    // PQ_ComplHeap conserva la propiedad heap después de cada inserción.
    for (int v : inputs) {
        heap.insert(v);
        assert(heap.isHeap()); 
    }

    // PQ_ComplHeap conserva la propiedad heap después de cada eliminación.
    while (!heap.empty()) {
        heap.delMax();
        if (!heap.empty()) {
            assert(heap.isHeap());
        }
    }
}

void test_heapify_floyd_invariants() {
    // heapifyFloyd produce un heap válido.
    vector<int> raw_data = {15, 3, 18, 9, 21, 6, 12, 4, 8, 2};
    complHeapHeapifyFloyd(raw_data, std::less<int>{});
    
    assert(complHeapIsValid(raw_data, std::less<int>{}));
}

void test_leftheap_invariants() {
    PQ_LeftHeap<int> h1;
    PQ_LeftHeap<int> h2;

    // PQ_LeftHeap conserva su propiedad después de insert.
    h1.insert(50);
    assert(h1.isValidLeftHeap());
    h1.insert(30);
    assert(h1.isValidLeftHeap());
    h1.insert(70);
    assert(h1.isValidLeftHeap());

    h2.insert(40);
    h2.insert(20);
    h2.insert(60);
    assert(h2.isValidLeftHeap());

    // PQ_LeftHeap conserva su propiedad después de merge.
    h1.merge(h2);
    assert(h1.isValidLeftHeap());

    // PQ_LeftHeap conserva su propiedad después de delMax.
    while (!h1.empty()) {
        h1.delMax();
        if (!h1.empty()) {
            assert(h1.isValidLeftHeap());
        }
    }
}

void test_huffman_invariants() {
    vector<HuffmanSymbol> alphabet = {
        {'A', 45}, {'B', 13}, {'C', 12}, {'D', 16}, {'E', 9}, {'F', 5}
    };
    
    auto codes = huffmanGenerateCodes(alphabet);

    // Huffman produce códigos libres de prefijos.
    assert(huffmanIsPrefixFree(codes));

    // Huffman maneja correctamente el caso de un solo símbolo.
    vector<HuffmanSymbol> single_symbol = {{'Z', 100}};
    auto single_codes = huffmanGenerateCodes(single_symbol);
    
    assert(single_codes.size() == 1);
    assert(single_codes['Z'] == "0"); 
    assert(huffmanIsPrefixFree(single_codes));
}

void test_treap_invariants() {
    Treap<int> treap(12345); 
    vector<int> keys = {55, 22, 88, 11, 33, 66, 99, 44, 77};

    for (int k : keys) {
        treap.add(k);
        // PRUEBA: Treap conserva propiedad BST después de insertar.
        assert(treap.isBST());
        // PRUEBA: Treap conserva propiedad de heap por prioridad después de insertar.
        assert(treap.isHeapByPriority());
        assert(treap.hasValidParentLinks());
    }

    for (int k : keys) {
        treap.remove(k);
        // Treap conserva ambas propiedades después de eliminar.
        if (!treap.empty()) {
            assert(treap.isBST());
            assert(treap.isHeapByPriority());
            assert(treap.hasValidParentLinks());
        }
    }
}

int main() {
    test_complheap_invariants();
    test_heapify_floyd_invariants();
    test_leftheap_invariants();
    test_huffman_invariants();
    test_treap_invariants();
    return 0;
}
```

**Resultado ctest:**
```
Test project .../build
    Start 1: semana6_public
1/2 Test #1: semana6_public ............   Passed    0.01 sec
    Start 2: semana6_internal
2/2 Test #2: semana6_internal ..........   Passed    0.02 sec

100% tests passed, 0 tests failed out of 2
```

* **Alteración de tamaño (`getMax` y `delMax`):** Atrapa inconsistencias en el estado interno del arreglo. Evita que consultas de solo lectura muten la estructura y que eliminaciones reales olviden actualizar el contador lógico, lo cual dejaría "basura" accesible en memoria.
* **Ordenamiento inestable (`heapSort`):** Previene bugs lógicos donde los valores duplicados provocan ciclos infinitos o sobrescrituras destructivas al intercambiar posiciones a gran distancia.
* **Mantenimiento de invariantes (`PQ_ComplHeap`):** Detecta fallos en la lógica de `percolateUp` y `percolateDown`. Atrapa casos donde el nodo se intercambia con el hijo equivocado o donde la iteración se detiene antes de restaurar por completo la jerarquía.
* **Ensamblaje top-down (`heapifyFloyd`):** Asegura que el barrido inverso `(n/2 - 1)` no omita ningún nodo interno crítico, previniendo que los subárboles superiores queden desalineados respecto a las hojas.
* **Inclinación izquierda (`PQ_LeftHeap`):** Detecta errores matemáticos al actualizar o comparar el *Null Path Length* (NPL). Un fallo aquí haría que el árbol gane peso en la rama derecha, destruyendo la garantía de tiempo logarítmico en las fusiones.
* **Construcción de diccionarios (`Huffman`):** Atrapa *segmentation faults* generados por alfabetos de un solo símbolo y previene bucles muertos si la estructura intenta procesar caracteres con frecuencia nula.
* **Seguridad de decodificación (*Prefix-Free* en `Huffman`):** Detecta asignaciones recursivas defectuosas (por ejemplo, asignar el mismo bit a ambas ramas), lo que generaría códigos ambiguos imposibles de decodificar sin usar separadores.
* **Simbiosis estructural (`Treap`):** Atrapa el clásico error de aplicar una rotación en sentido inverso, lo que restauraría la prioridad pero destruiría el orden lateral (BST). También detecta amputaciones prematuras que olvidan realizar el `trickleDown` hacia las hojas antes de liberar la memoria del nodo.

## Bloque 13 - Defensa escrita

Modificar directamente el código fuente de estas estructuras demuestra que programar topologías a bajo nivel otorga una comprensión que la simple revisión teórica no puede ofrecer. A nivel de arquitectura de software, trabajar bajo la interfaz genérica `PQ` me comprobó la eficacia del polimorfismo: el contrato operativo que garantiza insertar y extraer según una prioridad se mantiene inviolable frente al usuario, encapsulando por completo la complejidad de la topología interna elegida.

Al operar sobre la representación implícita del heap binario completo, resulta evidente la brutal ventaja de rendimiento que se obtiene al prescindir de punteros dinámicos. Usar un vector continuo y aritmética simple (`2i+1`, `(i-1)/2`) saca el máximo provecho de la localidad de la memoria caché. En esta estructura, comprobé que `percolateUp` es una maniobra ligera, ya que el nodo escala naturalmente verificando a un único ancestro directo. Por el contrario, codificar `percolateDown` expone la fragilidad crítica de la estructura: elegir al hijo equivocado para el hundimiento corrompería todo el subárbol subyacente. Frente a estas operaciones unitarias, implementar el `heapify` de Floyd demuestra ser una genialidad algorítmica; omitir la inmensa base de hojas y procesar los nodos internamente hacia arriba consolida la estructura entera en un eficiente tiempo `O(n)`. Respaldado por este concepto, adaptar `heapSort` muestra la elegancia del reciclaje de recursos, logrando ordenar *in-situ* al desterrar las cimas extraídas hacia los límites lógicos del arreglo, con cero fragmentación de memoria adicional.

Por otro lado, implementar la operación `merge` en el heap izquierdista obliga a replantear el concepto de equilibrio. Forzar deliberadamente una asimetría para cargar el peso en la rama izquierda (vía NPL) no es una falla, sino la clave de diseño que permite fusionar ramales derechos de manera garantizada en tiempo `O(\log n)`. Esta fusión veloz demuestra su valor en entornos industriales como la codificación de Huffman, donde delegar el emparejamiento codicioso de frecuencias a una cola de prioridad no solo agiliza la compresión, sino que asegura matemáticamente un árbol de diccionarios estrictamente libre de prefijos.

Finalmente, integrar un **`Treap`** me dejó claro que las rotaciones no están limitadas a costosos rebalanceos deterministas. Aquí, las rotaciones funcionan como el mecanismo perfecto para propulsar un nodo y satisfacer su prioridad vertical probabilística, manteniendo intacta la lateralidad requerida para ejecutar búsquedas ordenadas ágiles. Al realizar la comparación con `BinaryHeap` y el `BinarySearchTree`se concluye que no hay una estructura universalmente superior, sino decisiones basadas en el flujo operativo: el arreglo contiguo domina la purga rápida de cimas, mientras que las estructuras enlazadas son imprescindibles si se requieren consultas de rango (*lower/upper bounds*). Todo esto deja claro que diseñar pruebas unitarias, auditar invariantes lógicas y blindar casos borde es una disciplina ingenieril ineludible. Validar rigurosamente cada estado es la única garantía de que estas estructuras soportarán la carga de un entorno real sin desplomarse por fugas de memoria o degradación asintótica.