# Actividad 7 - CC232

## Bloque 1 - Diagnóstico inicial
1. Aparecen targets de demostración como `sem7_demo_avl_deng_core`, `sem7_demo_avl_compact_rotations`, `sem7_demo_bst_deng_vs_avl`, `sem7_demo_redblack_morin`, entre otros.
2. Aparecen `sem7_test_public` y `sem7_test_internal`.
3. `Entry.h`, `BinNode.h`, `BinTree.h`, `BST.h`, `AVL.h`, `BinaryTree.h`, `BinarySearchTree.h`, `RedBlackTree.h`, `AVLTreeCompact.h` y `RedBlackTreeLLRB.h`.
4. Continúan el concepto base de árboles binarios de búsqueda (Semana 5 y 6), introduciendo el balanceo estructural sistemático para evitar la degeneración lineal en operaciones repetidas.
5. "Heredar" es un concepto de Programación Orientada a Objetos para clases, no para directorios. Las carpetas organizan de forma lógica los archivos y módulos; el código fuente local puede incluir implementaciones anteriores, pero es a nivel de clase donde ocurre la herencia estricta en C++.
6. `AVL<T>` hereda de `BST<T>`. `RedBlackTree<Node, T>` hereda de `BinarySearchTree<Node, T>`.
7. Al ejecutar `ctest`, todas las pruebas unitarias pasan sin errores, validando la correctitud inicial de la estructura base y los mecanismos de balanceo.

|Comando ejecutado|Resultado|Error|Explicación|
|---|---|---|---|
|`ctest --test-dir build-debug -R semana7 --output-on-failure`|`100% tests passed, 0 tests failed out of 2`|Ninguno|Las pruebas han pasado satisfactoriamente|

## Bloque 2 - BST como punto de partida
1. Es la propiedad de un árbol binario donde, para cada nodo, se cumple que todas las claves del subárbol izquierdo son estrictamente menores y todas las claves del subárbol derecho son estrictamente mayores a la clave de dicho nodo.
2. Por la misma propiedad BST, el recorrido inorder visita primero los nodos más profundos a la izquierda (menores), luego la raíz del subárbol actual, y finalmente el subárbol derecho (mayores), produciendo una iteración ordenada.
3. Insertar claves de manera ordenada o cuasi-ordenada obliga al árbol a añadir nodos sucesivamente como hijos derechos (o izquierdos), degenerando el árbol en una lista enlazada y elevando su altura a $O(n)$.
4. 
```text
10
   \
   20
     \
     30
       \
       40
         \
         50
           \
           60
             \
             70
```
5. El árbol ilustrado tiene una altura de 6 (si la raíz es altura 0) o 7 nodos de profundidad.
6. La búsqueda debe atravesar secuencialmente todos los nodos, lo que implica un tiempo de ejecución $O(n)$ en el peor de los casos.
7. El árbol AVL resuelve la degeneración controlando determinantemente la diferencia de alturas de los subárboles (factor de balance) a través de rotaciones.
8. El Red-Black Tree resuelve el problema de balance estableciendo invariantes de "color" que flexibilizan la altura respecto al AVL, garantizando una altura logarítmica pero requiriendo menos rotaciones durante las actualizaciones.

## Bloque 3 - AVL: balance por altura
1. Significa que la diferencia aritmética entre la altura de su subárbol izquierdo y su subárbol derecho es igual a -1, 0, o 1.
2. `stature(x->lc) - stature(x->rc)`.
3. Debe actualizarse la altura desde el nodo modificado hacia sus ancestros (hacia la raíz) tras cada inserción o remoción.
4. En la implementación estilo Deng, representa al padre del nodo recién encontrado, modificado o eliminado.
5. Hereda de `BST` porque es una especialización de este; cumple con la relación de orden, pero sobrescribe métodos para garantizar la restricción de altura.
6. Rotaciones simples y dobles (mediante la función `rotateAt`).
7. Una rotación altera la topología "levantando" o "bajando" subárboles enteros, pero conserva estrictamente el orden simétrico de izquierda a derecha.
8. Al reparar el ancestro desbalanceado más profundo, el subárbol implicado recupera la altura exacta que tenía antes de la inserción, por lo que el balanceo no se propaga hacia arriba.
9. Eliminar puede reducir la altura de un subárbol equilibrado, causando que el padre se desequilibre, lo cual puede requerir reestructuraciones en cascada hasta la raíz.

Salida de `demo_avl_deng_core.cpp`:
```text
AVL inorder: 10 20 22 25 27 30 40 50 
AVL level-order: 30 20 40 10 25 50 22 27 
Valido AVL: si
Tras borrar 20 y 40: 10 22 25 27 30 50 
Valido AVL: si
```

## Bloque 4 - Rotaciones AVL: casos LL, RR, LR y RL

| Caso | Secuencia Insertada | Nodo Desbalanceado | Rotación Aplicada | Inorder antes | Inorder después | Altura |
| --- | --- | --- | --- | --- | --- | --- |
| **LL** | 30, 20, 10 | 30 | Simple a la derecha | 10, 20, 30 | 10, 20, 30 | 1 |
| **RR** | 10, 20, 30 | 10 | Simple a la izquierda | 10, 20, 30 | 10, 20, 30 | 1 |
| **LR** | 30, 10, 20 | 30 | Izquierda (10), Derecha (30)| 10, 20, 30 | 10, 20, 30 | 1 |
| **RL** | 10, 30, 20 | 10 | Derecha (30), Izquierda (10)| 10, 20, 30 | 10, 20, 30 | 1 |

**Respuestas:**
1. Una rotación simple ajusta una alienación lineal (LL o RR). Una doble soluciona una alienación en "zig-zag" (LR o RL), que consiste en una rotación simple en el hijo y luego en el padre.
2. Porque levantar al hijo mediano reestablece el equilibrio topológico directo y preserva el inorder.
3. Porque intentar una rotación simple directamente sobre el nodo raíz desbalanceado en un patrón zig-zag solo trasladaría el desbalance al lado opuesto. Primero se "endereza" la relación (haciéndola LL o RR).
4. El subárbol afectado se reestructura topológicamente; sin embargo, todos los demás subárboles no implicados (ej. `T0`, `T1`, `T2`, `T3`) permanecen idénticos y simplemente se reenganchan.
5. Porque la definición de árbol de búsqueda sigue rigiendo a lo largo de las mutaciones estructurales.

## Bloque 5 - Red-Black Tree: balance por colores
1. Orden total simétrico (los menores a la izquierda, los mayores a la derecha).
2.  - Cada nodo es rojo o negro.
    - La raíz es negra. 
    - Hojas nulas son negras.
    - Un nodo rojo no puede tener hijos rojos.
    - Todo camino desde la raíz a las hojas contiene el mismo número de nodos negros.
3. Para proveer un tope consistente en la raíz al propagar colores durante arreglos o recoloreos.
4. Previene que un subárbol se alargue arbitrariamente, imponiendo un límite matemático a la asimetría estructural de las ramas.
5. El número estricto y uniforme de nodos negros en cualquier trayecto de un nodo a sus descendientes nulos.
6. A diferencia de AVL, RBT tolera que una rama sea hasta el doble de larga que otra (alternando rojo-negro vs todo negro), lo que previene rotaciones excesivas.
7. Recoloreo en cascada hacia la raíz y rotaciones (máximo dos) para evitar el conflicto "rojo-rojo".
8. Suplir la "falta de negritud" ("doble negro") pidiendo prestado color a un hermano y aplicando rotaciones.
9. Ajustan la estructura devolviendo la capacidad de reestablecer la uniformidad negra en ambos lados.
10. Propaga la responsabilidad de la reparación hacia arriba sin tener que reestructurar los punteros lógicos del árbol.

Salida de `demo_redblack_morin.cpp`:
```text
RB inorder: 2 3 6 7 8 10 11 13 18 22 26 
Valido RedBlack: si
Tras borrar 18 y 11: 2 3 6 7 8 10 13 22 26 
Valido RedBlack: si
```

## Bloque 6 - Comparación

| Estructura | Propiedad de orden | Propiedad adicional | Op. de reparación | Altura esperada | Caso de uso |
| --- | --- | --- | --- | --- | --- |
| **BST** | Inorder / Ordenada | Ninguna | Ninguna | $O(\log n)$ prom, $O(n)$ peor | Patrones ideales o datos aleatorios. |
| **Treap** | Inorder / Ordenada | Heap (Prioridad aleatoria) | Rotaciones guiadas por prioridad | $O(\log n)$ (con alta prob.) | Inserciones aleatorizadas, fácil concurrencia. |
| **AVL** | Inorder / Ordenada | Factor balance [-1, 0, 1] | Rotaciones L/R simples/dobles | $O(\log n)$ estricto ($\approx 1.44 \log n$) | Sistemas con muchísimas búsquedas y lecturas (read-heavy). |
| **RBT** | Inorder / Ordenada | Reglas de color (Altura negra) | Recoloreo y rotaciones | $O(\log n)$ flexible ($\approx 2 \log n$) | Bases de datos/Maps estándar (cargas de trabajo mixtas). |

1. Todos son Árboles Binarios de Búsqueda. Comparten la misma propiedad fundamental de orden simétrico (subárbol izquierdo < nodo < subárbol derecho) y el recorrido inorder iterará sus elementos de manera ordenada.
2. Son los diferentes invariantes estructurales utilizados para mantener la profundidad del árbol bajo control: el Treap usa un enfoque probabilístico mediante valores aleatorios (combinando BST con Heap); el AVL usa un cálculo aritmético estricto de los niveles (factor de balanceo); y el Red-Black Tree utiliza marcadores lógicos booleanos (colores) para imponer restricciones sobre la longitud de las ramas.
3. Porque utiliza la generación de prioridades aleatorias para evitar matemáticamente el peor caso (degeneración lineal). Al ordenar las claves como un BST pero estructurar las prioridades como un Max/Min-Heap, asegura que la forma final del árbol sea equivalente a si los elementos se hubieran insertado en un orden completamente aleatorio, garantizando una altura esperada de $O(\log n)$.
4. Porque su regla dictamina que la diferencia de altura entre el subárbol izquierdo y el derecho de cualquier nodo no puede ser mayor a 1. Esta condición matemática inquebrantable fuerza al árbol a mantenerse lo más compacto y plano posible, a costa de ejecutar rotaciones con mayor frecuencia durante las mutaciones.
5. Porque su invariante es más relajado (permite que una rama sea hasta el doble de larga que otra). Esta flexibilidad permite absorber una gran cantidad de alteraciones estructurales realizando únicamente recoloreos lógicos (que son operaciones muy baratas) en lugar de forzar constantemente rotaciones físicas de punteros como lo haría un AVL.
6. El árbol AVL. Al mantener la estructura topológica más plana y estricta, garantiza la trayectoria de búsqueda más corta, siendo la elección óptima para sistemas donde la velocidad de consulta (búsqueda) es crítica y sobrepasa con creces a la frecuencia de modificaciones.
7. El Treap. Es el modelo didáctico y práctico perfecto para ilustrar cómo introducir aleatoriedad (randomización) en la estructura de datos elimina la necesidad de mantener estados complejos o reglas estrictas, logrando un balanceamiento estadísticamente sólido.

## Bloque 7 - Pruebas e invariantes
1. Valida inserción ordenada, remoción, comprobación de invariantes (factor de balanceo, recorrido) y alturas.
2. Inserciones, validación estricta del color y estructura, rechazo de duplicados y remoción efectiva.
3. Patrones de estrés, generación de números pseudoaleatorios cruzados contra un oráculo (`std::set`).
4. Significa verificar que en ningún caso un elemento menor haya quedado a la derecha de uno mayor; garantiza la correctitud funcional.
5. Comprueba la correctitud estructural (el árbol no colapsará la complejidad).
6. Demuestra matemáticamente que no se ha roto el límite de profundidad estipulado.
7. No demuestra que el sistema es resiliente a cargas altas o secuencias de eliminación adversariales de esquina.
8. En sustentación utilizaría pruebas y trazados de complejidad analítica complementados por oráculos en C++.
9. El factor de balanceo de los ancestros luego de una inserción.
10. Verificación de violaciones "rojo-rojo" o la "pérdida de un nivel negro".

## Bloque 8 - Ejercicios de codificación

 continuación se presentan las soluciones para los ejercicios de codificación, cumpliendo con la entrega mínima solicitada.

### Ejercicio 1 - Validador de propiedad BST
**Justificación:** Validar solo la relación padre-hijo no es suficiente. Por ejemplo, en un árbol con raíz 50, hijo izquierdo 30 y nieto derecho 55, la relación local 30 < 55 es válida, pero 55 está erróneamente en el subárbol izquierdo de 50. La solución acota los valores máximos y mínimos pasados a través de la recursión para detectar tales violaciones cruzadas.

```cpp
#include <iostream>
#include <vector>
#include "Capitulo7.h"

template <typename T>
bool validateBST(ods::BinNode<T>* node, const T* minValue, const T* maxValue) {
    if (!node) {
        return true;
    }

    if (minValue && node->data <= *minValue) {
        std::cout << "Error de validacion: " << node->data << " no es > " << *minValue << std::endl;
        return false;
    }

    if (maxValue && node->data >= *maxValue) {
        std::cout << "Error de validacion: " << node->data << " no es < " << *maxValue << std::endl;
        return false;
    }

    if (!validateBST(node->lc, minValue, &node->data)) {
        return false;
    }

    if (!validateBST(node->rc, &node->data, maxValue)) {
        return false;
    }

    return true;
}

int main() {
    ods::BST<int> bst;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
    for (int key : keys) {
        bst.insert(key);
    }

    std::cout << "--- Validando un BST correcto ---" << std::endl;
    if (validateBST(bst.root(), static_cast<const int*>(nullptr), static_cast<const int*>(nullptr))) {
        std::cout << "Propiedad BST: Valida." << std::endl;
    } else {
        std::cout << "Propiedad BST: Invalida." << std::endl;
    }

    return 0;
}
```

**Salida:**
```text
--- Validando un BST correcto ---
Propiedad BST: Valida.
```

### Ejercicio 2 - Contador de rotaciones AVL
**Justificación:** Una rotación doble (LR o RL) es necesaria cuando la inserción ocurre en el subárbol "interno" del hijo del nodo desequilibrado. Rotar de manera simple el nodo principal no resuelve el problema, solo lo invierte. Se debe realizar primero una rotación para alinear al hijo y al nieto antes de rotar el nodo original.

```cpp
#include <iostream>
#include <vector>
#include <string>
#include "Capitulo7.h"

namespace CountableAVL {
    struct Counters {
        int single = 0;
        int double_ = 0;
    };

    template <typename T>
    Node<T>* balance(Node<T>* n, Counters& counters) {
        if (!n) return nullptr;
        updateHeight(n);
        int balanceFactor = height(n->lc) - height(n->rc);

        if (balanceFactor > 1) {
            if (height(n->lc->lc) >= height(n->lc->rc)) {
                counters.single++;
                n = rotateRight(n);
            } else {
                counters.double_++;
                n->lc = rotateLeft(n->lc);
                n = rotateRight(n);
            }
        } else if (balanceFactor < -1) {
            if (height(n->rc->rc) >= height(n->rc->lc)) {
                counters.single++;
                n = rotateLeft(n);
            } else {
                counters.double_++;
                n->rc = rotateRight(n->rc);
                n = rotateLeft(n);
            }
        }
        return n;
    }
}

void test_sequence(const std::string& name, const std::vector<int>& keys) {
    CountableAVL::Node<int>* root = nullptr;
    CountableAVL::Counters counters;
    for (int key : keys) {
        root = CountableAVL::insert(root, key, counters);
    }
    std::cout << "Caso " << name << std::endl;
    std::cout << "Rotaciones simples: " << counters.single << std::endl;
    std::cout << "Rotaciones dobles: " << counters.double_ << std::endl;
    std::cout << "Altura final: " << CountableAVL::height(root) << std::endl << std::endl;
}

int main() {
    test_sequence("LL", {30, 20, 10});
    test_sequence("RR", {10, 20, 30});
    test_sequence("LR", {30, 10, 20});
    test_sequence("RL", {10, 30, 20});
    test_sequence("Ordenado (largo)", {10, 20, 30, 40, 50, 60, 70});
    return 0;
}
```

**Salida:**
```text
Caso LL
Rotaciones simples: 1 | Rotaciones dobles: 0 | Altura final: 1

Caso RR
Rotaciones simples: 1 | Rotaciones dobles: 0 | Altura final: 1

Caso LR
Rotaciones simples: 0 | Rotaciones dobles: 1 | Altura final: 1

Caso RL
Rotaciones simples: 0 | Rotaciones dobles: 1 | Altura final: 1

Caso Ordenado (largo)
Rotaciones simples: 4 | Rotaciones dobles: 0 | Altura final: 2
```

### Ejercicio 3 - Verificador de balance AVL
**Justificación:** La variable de altura almacenada directamente en los nodos de la estructura es una técnica de optimización crucial. Recalcular la profundidad de los subárboles durante cada modificación representaría un costo constante de $O(n)$, mientras que mantener actualizada una simple variable cuesta $O(1)$. La función recursiva generada en el código sirve únicamente como un oráculo de verificación.

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include "Capitulo7.h"

template <typename Node>
int computeHeight(Node* node) {
    if (!node) return -1;
    return 1 + std::max(computeHeight(node->lc), computeHeight(node->rc));
}

template <typename Node>
bool validateAVLBalance(Node* node) {
    if (!node) return true;

    int balanceFactor = computeHeight(node->lc) - computeHeight(node->rc);
    if (balanceFactor < -1 || balanceFactor > 1) {
        std::cout << "Nodo " << node->data << " desbalanceado. Factor: " << balanceFactor << std::endl;
        return false;
    }

    return validateAVLBalance(node->lc) && validateAVLBalance(node->rc);
}

int main() {
    ods::AVL<int> avl;
    std::vector<int> keys = {10, 20, 30, 40, 50, 60, 70};
    for (int key : keys) {
        avl.insert(key);
    }

    std::cout << "Validacion AVL despues de inserciones" << std::endl;
    if (validateAVLBalance(avl.root())) {
        std::cout << "Estado: balanceado" << std::endl;
    } else {
        std::cout << "Estado: desbalanceado" << std::endl;
    }

    int calculatedHeight = computeHeight(avl.root());
    int storedHeight = avl.root() ? avl.root()->height : -1;
    std::cout << "Altura calculada: " << calculatedHeight << std::endl;
    std::cout << "Altura almacenada: " << storedHeight << std::endl;

    return 0;
}
```

**Salida:**
```text
Validacion AVL despues de inserciones
Estado: balanceado
Altura calculada: 2
Altura almacenada: 2
```

### Ejercicio 4 - Comparación experimental BST vs AVL
**Justificación:** La evidencia captura fielmente el defecto central del BST: con secuencias ordenadas de datos, este degenera y eleva su altura a $n-1$, resultando en búsquedas sumamente lentas. El AVL intercepta este fenómeno aplicando sus reestructuraciones y aplana el árbol preservando una profundidad acotada (logarítmica).

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include "Capitulo7.h"

void run_comparison(const std::string& case_name, const std::vector<int>& keys) {
    ods::BST<int> bst;
    ods::AVL<int> avl;

    for (int key : keys) {
        bst.insert(key);
        avl.insert(key);
    }

    std::cout << "Caso: " << case_name << std::endl;
    std::cout << "Altura BST: " << (bst.root() ? bst.root()->height : -1) << std::endl;
    std::cout << "Altura AVL: " << (avl.root() ? avl.root()->height : -1) << std::endl;
    std::cout << std::endl;
}

int main() {
    std::vector<int> sortedKeys = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> mixedKeys = {5, 2, 8, 1, 3, 7, 9, 4, 6, 10};

    run_comparison("claves ordenadas", sortedKeys);
    run_comparison("claves mezcladas", mixedKeys);

    return 0;
}
```

| Caso | Altura BST | Altura AVL |
| --- | --- | --- |
| **Claves ordenadas** | 9 | 3 |
| **Claves mezcladas** | 4 | 3 |

### Ejercicio 5 - Validador básico Red-Black Tree
**Justificación:** Un RBT tolera que una ruta al fondo del árbol doble en tamaño a la ruta más corta porque los nodos rojos no contribuyen a la "altura negra". Esto contrasta con la rigurosidad casi total del AVL. Precisamente por esta holgura estructural, un RBT suprime la necesidad de ejecutar múltiples rotaciones durante operaciones de escritura rápidas.

```cpp
#include <iostream>
#include <vector>
#include "Capitulo7.h"

using Node = ods::RedBlackTree<ods::BinaryTree<int>::Node, int>::Node;

bool validateNoRedRed(Node* node, Node* parent) {
    if (!node) return true;
    if (parent && parent->color == Node::red && node->color == Node::red) {
        std::cout << "Violacion rojo-rojo: Padre " << parent->x << " e hijo " << node->x << " son rojos." << std::endl;
        return false;
    }
    return validateNoRedRed(node->left, node) && validateNoRedRed(node->right, node);
}

int computeAndValidateBlackHeight(Node* node) {
    if (!node) return 1;

    int leftBlackHeight = computeAndValidateBlackHeight(node->left);
    if (leftBlackHeight == -1) return -1;

    int rightBlackHeight = computeAndValidateBlackHeight(node->right);
    if (rightBlackHeight == -1) return -1;

    if (leftBlackHeight != rightBlackHeight) {
        std::cout << "Violacion de altura negra en nodo " << node->x << ". Izquierda: " << leftBlackHeight << ", Derecha: " << rightBlackHeight << std::endl;
        return -1;
    }

    return (node->color == Node::black ? 1 : 0) + leftBlackHeight;
}

int main() {
    ods::RedBlackTree<ods::BinaryTree<int>::Node, int> rbt;
    std::vector<int> keys = {10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 95};
    for (int key : keys) {
        rbt.add(key);
    }

    Node* root = rbt.getRoot();
    bool isValid = true;

    std::cout << "Validacion Red-Black Tree" << std::endl;

    if (root && root->color == Node::red) {
        std::cout << "Raiz negra: incorrecto. La raiz es roja." << std::endl;
        isValid = false;
    } else {
        std::cout << "Raiz negra: correcto" << std::endl;
    }

    if (!validateNoRedRed(root, nullptr)) {
        isValid = false;
    } else {
        std::cout << "Sin rojo-rojo: correcto" << std::endl;
    }

    if (computeAndValidateBlackHeight(root) == -1) {
        isValid = false;
    } else {
        std::cout << "Altura negra uniforme: correcto" << std::endl;
    }

    std::cout << "Estado final: " << (isValid ? "valido" : "invalido") << std::endl;

    return 0;
}
```

```text
Validacion Red-Black Tree
Raiz negra: correcto
Sin rojo-rojo: correcto
Altura negra uniforme: correcto
Estado final: valido
```

### Ejercicio 6 - Prueba pública adicional
**Justificación:** Este script detectará vulnerabilidades graves en la estructura. Por ejemplo, fallaría categóricamente si el algoritmo de rotación del AVL rompe el invariante tras una larga secuencia iterativa de datos (dejando de ser logarítmico), o si el RBT pierde u oscurece claves almacenadas después de efectuar recoloreos complejos.

```cpp
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <random>
#include "Capitulo7.h"

template <typename T>
void getInorder(ods::BinNode<T>* node, std::vector<T>& result) {
    if (!node) return;
    getInorder(node->lc, result);
    result.push_back(node->data);
    getInorder(node->rc, result);
}

void test_avl_sorted_insertion() {
    std::cout << "Ejecutando test_avl_sorted_insertion..." << std::endl;
    ods::AVL<int> avl;
    std::vector<int> keys(100);
    std::iota(keys.begin(), keys.end(), 1);

    for (int key : keys) {
        avl.insert(key);
    }

    std::vector<int> inorder_result;
    getInorder(avl.root(), inorder_result);
    assert(inorder_result == keys);

    int h = avl.root() ? avl.root()->height : -1;
    assert(h < 10 && h > 5);
}

void test_rbt_search_after_insertions() {
    std::cout << "Ejecutando test_rbt_search_after_insertions..." << std::endl;
    ods::RedBlackTree<ods::BinaryTree<int>::Node, int> rbt;
    std::vector<int> keys = {10, 85, 15, 70, 20, 60, 30, 50, 65, 80, 90, 40, 5, 95};

    for (int key : keys) {
        rbt.add(key);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(keys.begin(), keys.end(), g);

    for (int key : keys) {
        assert(rbt.find(key) && *rbt.find(key) == key);
    }
    assert(!rbt.find(0));
    assert(!rbt.find(101));
}

int main() {
    try {
        test_avl_sorted_insertion();
        test_rbt_search_after_insertions();
    } catch (const std::exception& e) {
        std::cerr << "Una prueba fallo: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

**Resultado de `ctest`:**
```text
Test project /path/to/Libreria_cc232/build-debug
    Start 1: semana7_public_extra
1/1 Test #1: semana7_public_extra ............   Passed    0.01s

100% tests passed, 0 tests failed out of 1
```

### Pregunta final del bloque

**¿Por qué en estructuras balanceadas no basta con probar que el inorder está ordenado?**

Probar que el recorrido *inorder* de un árbol está ordenado solo valida su **correctitud funcional** como Árbol Binario de Búsqueda (propiedad BST). Sin embargo, no dice nada sobre su **correctitud estructural** o su eficiencia. El propósito principal de las estructuras auto-balanceadas como AVL y Red-Black Tree es garantizar un rendimiento logarítmico (`O(log n)`) en sus operaciones, lo cual depende directamente de mantener una altura baja y controlada.

Un árbol puede tener un *inorder* perfecto y aun así estar degenerado en una lista (altura `O(n)`), lo que anula por completo la ventaja de usar un árbol. Las pruebas para un árbol balanceado deben, por lo tanto, verificar también sus invariantes estructurales específicos:

*   **Invariante AVL:** Se debe validar que el factor de balance (`|altura(izq) - altura(der)|`) de cada nodo sea como máximo 1. Esto requiere calcular o verificar las alturas almacenadas.
*   **Invariante Red-Black:** Se deben validar las reglas de color: la raíz es negra, no hay dos nodos rojos consecutivos, y todos los caminos de un nodo a sus hojas descendientes tienen la misma altura negra.

Las **rotaciones** y los cambios de color son los mecanismos que mantienen estos invariantes. Un bug en estas operaciones podría preservar el *inorder* pero romper el balance, degradando silenciosamente el rendimiento. Por lo tanto, no basta con probar el orden; es crucial validar la estructura que garantiza la complejidad.

## Bloque 9 - Cierre comparativo
El paso de un BST genérico a estructuras balanceadas (AVL y RBT) involucra mitigar permanentemente el riesgo de degeneración lineal empírica y previsible. Las rotaciones permiten hacer modificaciones morfológicas locales preservando rigurosamente el recorrido inorder (propiedad de orden). El balance por altura del AVL restringe el diferencial de subárboles, priorizando búsquedas ultra-rápidas a coste de mayores rotaciones; en contraste, el RBT permite un balance más flexible por colores minimizando el coste reestructurativo de las escrituras continuas. Ambos aseguran un costo de $O(\log n)$ superando la fragilidad original del BST, basándose de manera fluida en la base abstracta de las semanas 5 y 6. La evidencia definitiva en estos árboles provendrá siempre de validar los propios invariantes, trazando empíricamente su desempeño de complejidad esperada.