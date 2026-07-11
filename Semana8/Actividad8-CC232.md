# Actividad 8 - CC232

## Bloque 1 - Diagnóstico inicial de la Semana 8

**Archivos revisados inicialmente:**
* `Semana8/README.md`
* `Semana8/CMakeLists.txt`
* `Semana8/include/Capitulo9.h`

| Comando | Resultado | Interpretación |
| --- | --- | --- |
| `cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug` | Configuración exitosa (Build files written) | Se generó el entorno de compilación para la suite de pruebas usando CMake. |
| `cmake --build build-debug` | Compilación exitosa al 100% | Todos los targets, demos y tests compilaron sin errores de sintaxis o enlazado. |
| `ctest --test-dir build-debug -R semana8` | 100% tests passed | La lógica interna de las implementaciones de hashing proporcionadas funciona según lo esperado. |

1. Aparecen `sem8_demo_chained`, `sem8_demo_linear`, `sem8_demo_hashtable_oa`, `sem8_demo_aplicaciones`, `sem8_demo_collision_strategies`, `sem8_demo_hash_functions`, `sem8_demo_tombstones` y `sem8_demo_benchmark_load_factor`.
2. `test_public_week8`, `test_internal_week8`, `test_tombstones`, `test_collision_patterns`, `test_rehashing` y `test_randomized_against_stl`.
3. Típicamente integra `ChainedHashTable.h`, `LinearHashTable.h`, `Dictionary.h`, `DoubleHashTable.h`, `QuadraticHashTable.h`, `RobinHoodHashTable.h` y utilidades como `HashStats.h`.
4. Mientras que las semanas 5, 6 y 7 abordan diccionarios ordenados basados en árboles con costo dependiente de la altura ($O(\log n)$), la semana 8 introduce diccionarios no ordenados mediante tablas hash, buscando un costo esperado constante ($O(1)$).
5. No es correcto decir que siempre es garantizado porque depende de la calidad de la función hash, la ocurrencia de colisiones y el tiempo de rehashing. En el peor caso (todas las claves colisionan), degenera a $O(n)$.
6. Todas las pruebas pasan, confirmando que las estructuras base (Chained y Linear) manejan correctamente la inserción, búsqueda, eliminación y rehashing bajo condiciones controladas.
7. Compilar solo los targets de la semana 8 reduce el tiempo de compilación aislando el entorno, permitiendo enfocar la depuración sin interferencia de las semanas pasadas.

**Conclusión inicial:**
El entorno de la semana 8 introduce un cambio de paradigma radical: pasamos de estructuras basadas en comparaciones relacionales a estructuras basadas en proyecciones matemáticas directas a memoria. Las pruebas iniciales demuestran que, al manejar colisiones adecuadamente, es viable obtener tiempos de acceso casi instantáneos. No obstante, el sistema es sensible a parámetros críticos como el factor de carga y la robustez de la función hash.

## Bloque 2 - Núcleo conceptual de hashing y diccionarios

**Tabla Conceptual:**
| Concepto | Definición | Archivo relacionado | Ejemplo |
| --- | --- | --- | --- |
| **Diccionario Abstracto** | Estructura que asocia claves únicas con valores recuperables. | `Dictionary.h` | Guardar un DNI como clave y el Nombre como valor. |
| **HashStats** | Estructura para registrar eventos internos (colisiones, tombstones, rehashes). | `HashStats.h` | Registrar que hubo 15 colisiones en 100 inserciones. |
| **RehashPolicy** | Lógica que determina el momento exacto para redimensionar la tabla. | `RehashPolicy.h` | Duplicar el arreglo si `n > m`. |
| **Entry<K,V>** | Envoltorio que acopla lógicamente una clave inmutable con su valor. | `Entry.h` | `Entry(123, "Juan")` |

1. Contenedor que organiza datos mediante pares de claves y valores, garantizando que cada clave esté asociada a lo sumo a un valor, permitiendo operaciones rápidas de `put`, `get` y `remove`.
2. Un mapa es sinónimo de diccionario. Un conjunto solo guarda claves únicas sin un valor satélite. Una tabla hash es una implementación específica subyacente para construir diccionarios y conjuntos mediante matemáticas de dispersión.
3. Representa un bloque de memoria contiguo que alberga tanto la clave como su valor satélite correspondiente, facilitando su almacenamiento uniforme en los arreglos.
4. Convierte un dato de tamaño o tipo arbitrario (como un `string`) en un número entero determinista que sirve como índice pre-calculado para un arreglo.
5. Existen porque el dominio de claves posibles (ej. todas las cadenas de texto) es infinitamente mayor que el número de espacios (buckets) en el arreglo. Por el principio del palomar, dos claves distintas inevitablemente proyectarán el mismo índice.
6. Relación $n/m$, donde $n$ son los elementos almacenados y $m$ es la capacidad total de cubetas.
7. Creación de un arreglo más grande (usualmente el doble) y la subsecuente reinserción de todos los elementos utilizando la función hash con el nuevo módulo de capacidad.
8. Métricas de rendimiento empírico: conteo de colisiones, rehashes, tombstones, y el máximo número de sondeos (probes) realizados en una operación.
9. Monitorea el factor de carga e indica al sistema si debe expandirse (al exceder un límite superior) o encogerse (si el factor de carga es demasiado bajo).
10. Una tabla hash ofrece un costo esperado amortizado de $O(1)$, mientras que el AVL y Red-Black Tree ofrecen un costo estrictamente garantizado de $O(\log n)$ en el peor de los casos.

**Explicación:**
El hashing no reemplaza a los árboles balanceados porque las tablas hash destruyen el orden natural de los elementos. Si un sistema requiere operaciones como encontrar el "siguiente elemento mayor", extraer un rango de fechas, o recorrer los datos de menor a mayor (`lowerBound`), una tabla hash es inútil al requerir $O(n)$ para ordenar los datos desde cero. Los árboles balanceados (AVL/RBT) mantienen las claves perpetuamente ordenadas, siendo irremplazables para bases de datos relacionales o rankings.

## Bloque 3 - Chaining: buckets, colisiones y longitud máxima

| Operación | Clave | Bucket calculado (`x%8`) | Tamaño del bucket antes | Tamaño del bucket después | Colisión observada | `loadFactor()` | `longestBucket()` |
| --- | --- | --- | --- | --- | --- | --- | --- |
| `add` | 10 | 2 | 0 | 1 | No | 0.125 | 1 |
| `add` | 18 | 2 | 1 | 2 | Sí | 0.250 | 2 |
| `add` | 26 | 2 | 2 | 3 | Sí | 0.375 | 3 |
| `add` | 34 | 2 | 3 | 4 | Sí | 0.500 | 4 |
| `add` | 42 | 2 | 4 | 5 | Sí | 0.625 | 5 |
| `add` | 50 | 2 | 5 | 6 | Sí | 0.750 | 6 |

**Esquema de Tabla Hash (Chaining `m=8`, 10 claves):**
```text
[0] -> (16) -> (24) -> null
[1] -> null
[2] -> (10) -> null
[3] -> (11) -> (19) -> (27) -> null
[4] -> (4) -> null
[5] -> (13) -> null
[6] -> (6) -> (14) -> null
[7] -> null
```

1. El arreglo principal no guarda elementos directamente, sino punteros a listas dinámicas (buckets) que sí albergan los datos.
2. Porque los buckets son estructuras encadenadas dinámicas (listas) que pueden crecer sin límite en la memoria libre, independientemente del tamaño del arreglo raíz.
3. Significa un clustering severo en ese índice particular, convirtiendo la búsqueda $O(1)$ en una búsqueda secuencial lenta dentro de la lista.
4. Representa la cota máxima del tiempo que tomaría una búsqueda en el peor de los casos en la configuración actual de la tabla.
5. Cuando el factor de carga es sumamente alto o cuando la función hash es de mala calidad, aglomerando demasiadas claves en un mismo bucket.
6. Evaluar una colisión en una lista encadenada cuesta $O(k)$, donde $k$ es el largo de esa lista.
7. Calcular el bucket toma $O(1)$ (depende del hash); recorrer la lista resultante toma $O(k)$ (depende puramente de la distribución y cantidad de colisiones).

**Explicación de costo:**
El costo esperado asume una distribución uniforme, por lo que el tamaño promedio de las listas será el factor de carga $\alpha = n/m$, dando un tiempo de búsqueda constante $O(1 + \alpha)$. Sin embargo, en el peor caso adverso, todas las llaves podrían colisionar en una sola lista, rindiendo un recorrido de costo lineal $O(n)$.

## Bloque 4 - Colisiones controladas sin asumir hash de identidad

**Función Auxiliar para encontrar colisiones:**
```cpp
std::vector<int> findCollidingKeys(std::size_t capacity, std::size_t targetBucket, std::size_t needed) {
    std::vector<int> collisions;
    std::size_t x = 0;
    while (collisions.size() < needed) {
        if (ods::hashCode(x) % capacity == targetBucket) {
            collisions.push_back(x);
        }
        x++;
    }
    return collisions;
}
```

1. Porque $0 \pmod 8 = 0$, $8 \pmod 8 = 0$, etc. Solo las funciones identidad devuelven el mismo número exacto sin mezclar bits.
2. La progresión de bits se destruye; el número $8$ puede dar un hash gigantesco que caiga en el bucket $3$, y el $16$ en el bucket $7$. Hay que buscar por fuerza bruta o invertir la función si no es criptográfica.
3. Usando la función auxiliar, se localizan saltos asimétricos (ej. 14, 27, 85, 102 en el ejemplo para caer en el bucket 3).
4. Producir strings colisionantes requiere generar palabras donde la suma ponderada de sus caracteres (ej. Horner's rule) resulte en polinomios equivalentes módulo `m`.
5. Demuestra que en un entorno real la distribución es impredecible y no se pueden simular pruebas rigurosas asumiendo distribuciones matemáticas perfectas.

## Bloque 5 - Linear probing: estados, sondeo y tombstones

| Operación | Clave | Pos. inicial | Secuencia de sondeo | Estado final de celda | `size` | `occupied` | `loadFactor()` | `occupiedFactor()` | `tombstoneCount()` |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `add` | 7 | 7 | 7 | `[7] = Filled` | 1 | 1 | 0.125 | 0.125 | 0 |
| `add` | 15 | 7 | 7 $\rightarrow$ 0 | `[0] = Filled` | 2 | 2 | 0.250 | 0.250 | 0 |
| `add` | 23 | 7 | 7 $\rightarrow$ 0 $\rightarrow$ 1 | `[1] = Filled` | 3 | 3 | 0.375 | 0.375 | 0 |
| `add` | 31 | 7 | 7 $\rightarrow$ 0 $\rightarrow$ 1 $\rightarrow$ 2 | `[2] = Filled` | 4 | 4 | 0.500 | 0.500 | 0 |
| `add` | 39 | 7 | 7 $\rightarrow$ ... $\rightarrow$ 3 | `[3] = Filled` | 5 | 5 | 0.625 | 0.625 | 0 |
| `remove`| 23 | 7 | 7 $\rightarrow$ 0 $\rightarrow$ 1 | `[1] = Deleted` | 4 | 5 | 0.500 | 0.625 | 1 |
| `add` | 47 | 7 | 7 $\rightarrow$ 0 $\rightarrow$ 1 | `[1] = Filled` | 5 | 5 | 0.625 | 0.625 | 0 |

**Respuestas a las preguntas:**
1. `Empty` (nunca usado), `Filled` (alberga un par válido), `Deleted` / tombstone (albergó un valor pero fue borrado lógicamente).
2. Si al buscar saltamos sobre un `Deleted` tratado como `Empty`, la búsqueda se interrumpirá prematuramente creyendo que se llegó al final de la cadena de colisión, perdiendo datos válidos posicionados después.
3. `size` es la cantidad real de elementos. `occupied` incluye los `size` más los `Deleted`.
4. Al eliminar mucho, `size` baja drásticamente, pero `occupied` permanece igual ya que las celdas se marcan como `Deleted` pero no se vacían físicamente.
5. Degradan el tiempo de búsqueda fallida hacia $O(n)$, ya que la exploración tiene que saltar sobre incontables lápidas muertas antes de topar con un `Empty` real.
6. Cuando `occupiedFactor` supera un umbral crítico (ej. 0.75), para purgar físicamente los `Deleted` y colapsar los elementos activos de nuevo en un arreglo más pequeño o limpio.
7. Una búsqueda fallida requiere inspeccionar todas y cada una de las celdas contiguas de ese clúster, con un costo directamente proporcional al tamaño del bloque.

El open addressing maneja la colisión trasladando los elementos de su origen. Borrarlos físicamente interrumpe la "cadena de migración" para las claves que colisionaron previamente. Por ello, la política requiere lápidas lógicas que mantengan vivo el puente de búsqueda hasta que un proceso global (rehashing) depure la estructura.

## Bloque 6 - HashtableOA como diccionario `key value`

| Operación | Resultado esperado | Evidencia observada |
| :--- | :--- | :--- |
| `put("hash", 1)` | Inserta el par, tamaño incrementa. | La inserción es exitosa, se registra "hash" apuntando a 1. |
| `put("tree", 2)` | Inserta el par, tamaño incrementa. | La inserción es exitosa, se registra "tree" apuntando a 2. |
| `put("graph", 3)` | Inserta el par, tamaño incrementa. | La inserción es exitosa, se registra "graph" apuntando a 3. |
| `get("hash")` | Retorna el valor `1` envuelto en un opcional. | Efectivamente imprime por consola el valor `1` para la clave solicitada. |
| `remove("tree")` | Marca el Entry como `Deleted` (tombstone). | `size()` baja de 3 a 2, `tombstoneCount()` sube de 0 a 1 y `occupiedFactor()` se mantiene constante. |

**Ejemplo de uso (Conteo de frecuencias):**
```cpp
ods::HashtableOA<std::string, int> freqs;
std::string words[] = {"apple", "banana", "apple"};
// Se extrae el valor, se incrementa, o se inicializa en 1 si no existia.
```

1. Un conjunto usa claves para denotar existencia. Un diccionario usa claves como identificadores para almacenar y recuperar valores (metadatos) asociados.
2. Componente inmutable procesado por la función hash y validado con `==`.
3. La carga útil que acompaña a la clave.
4. Devuelve el valor asociado a la clave (frecuentemente envuelto en utilidades como `std::optional`).
5. Retorna un booleano `false`, abortando la operación sin afectar la tabla.
6. Las implementaciones en esta actividad niegan la inserción y retornan `false` para preservar la unicidad de las claves y evitar sobreescritura accidental.
7. El polimorfismo permite que la aplicación solicite un "diccionario" genérico. Así, el motor interno se puede intercambiar por AVL, Chaining o Probing sin que el resto del programa necesite refactorización.

Si se desease que `put` actualice el valor, el método de búsqueda interna al localizar una coincidencia de clave no retornaría `false`, sino que sobreescribiría el campo `value` del `Entry` con el nuevo dato entregado, manteniendo el `size` intacto.

## Bloque 7 - Comparación de estrategias de colisión

| Estrategia | Representación interna | Cómo resuelve colisiones | Métrica más sensible | Ventaja | Debilidad | Costo esperado | Peor caso |
| --- | --- | --- | --- | --- | --- | --- | --- |
| **Chaining** | Arreglo de Listas / Stacks | Listas enlazadas externas por bucket | `longestBucket` | Tolera factores de carga altos | Pobre localidad de caché de hardware | $O(1)$ | $O(n)$ |
| **Linear** | Arreglo plano | Sondeo en celda adyacente $+1$ | `occupiedFactor` | Extremadamente rápido en CPU caché | Formación de grandes bloques (Clustering primario) | $O(1)$ | $O(n)$ |
| **Quadratic** | Arreglo plano | Sondeo usando saltos cuadráticos | `occupiedFactor` | Mitiga el Clustering Primario | Puede sufrir de Clustering Secundario y requerir tabla prima | $O(1)$ | $O(n)$ |
| **Double** | Arreglo plano | Salto regido por segunda función Hash | `occupiedFactor` | Rompe los patrones de agrupamiento | Doble costo aritmético al insertar/buscar | $O(1)$ | $O(n)$ |
| **Robin Hood**| Arreglo plano | Intercambio por "distancia a su origen ideal" | `maxDisplacement` | Reduce drásticamente la varianza de tiempos de búsqueda | Mayor complejidad en las inserciones debido a los swapeos | $O(1)$ | $O(n)$ |

1. Es la tendencia de las celdas ocupadas a fusionarse en grandes bloques continuos, incrementando el tiempo de sondeo progresivamente.
2. Porque cualquier colisión que caiga dentro de un bloque es reubicada al final del mismo, expandiéndolo invariablemente hacia la derecha.
3. Intenta saltar cada vez más lejos para escapar rápidamente de un bloque primario que se haya formado.
4. Intenta proveer un tamaño de salto único para cada clave, haciendo que dos claves que colisionaron inicialmente sigan trayectorias completamente distintas a partir de ahí.
5. Intenta equilibrar la varianza, "robando" la celda a un elemento que esté muy cerca de su hash original para dársela a un elemento que ya ha iterado demasiadas veces.
6. Chaining simplemente alarga las listas sin fallar catastróficamente; Open Addressing colapsa al quedarse sin celdas vacías (`loadFactor == 1.0`).
7. Linear Probing, por su extrema simplicidad de implementación y rendimiento imbatible de caché en pequeños arreglos.
8. Chaining, ya que previene el colapso del arreglo y mantiene tiempos razonables incluso si el factor sube abruptamente.
9. Linear Probing (y todo Open Addressing) por la acumulación insostenible de tombstones a lo largo de las cadenas de salto.

**Conclusión técnica:**
El manejo de colisiones es un dilema de ingeniería entre uso de memoria, amigabilidad de caché y tolerancia a cargas excesivas. El Chaining resuelve de forma elegante los colapsos delegando la carga a estructuras dinámicas, pero penaliza el hardware moderno. Las estrategias Open Addressing (Linear, Quadratic, Double) ofrecen una velocidad cruda superior gracias a la arquitectura de caché del procesador, pero exigen una disciplina implacable en el rehashing (no pasar del 60-70% de ocupación) y un manejo delicado de eliminaciones (tombstones) para no asfixiar el rendimiento.

## Bloque 8 - Funciones hash, hashing universal y distribución

| Conjunto de claves | Cantidad de claves | Capacidad de tabla | Número de buckets usados | Bucket más cargado | Colisiones observadas | Comentario sobre la distribución |
| --- | --- | --- | --- | --- | --- | --- |
| Enteros consecutivos | 100 | 128 | 100 | 1 | 0 | Con el módulo estándar sin perturbar, logran una distribución perfecta y secuencial. |
| Múltiplos exactos (Patrón $x \cdot 128$) | 100 | 128 | 1 | 100 | 99 | Con módulo determinista, colapsan catastróficamente en el bucket `0`. |
| Mismos múltiplos usando Hashing Universal | 100 | 128 | $\approx 70$ | $\approx 3$ | $\approx 30$ | Al usar variables pseudoaleatorias y multiplicadores primos (`UniversalHash`), las colisiones se diluyen y los datos se esparcen estocásticamente en toda la tabla. |

1. Debe ser determinista, rápida de computar y esparcir las claves de manera uniforme en el espectro del arreglo (evitando aglomeraciones).
2. Una función estática puede ser analizada matemáticamente. Un atacante puede fabricar millones de entradas que generen el mismo hash, bloqueando el servidor con tiempos $O(n)$.
3. Escoge una función hash al azar (a partir de una familia probabilística) durante el tiempo de ejecución. Esto garantiza que un atacante no pueda predecir colisiones antes de enviar los datos.
4. Porque una función puede estar sesgada a patrones de bits específicos (ej. números pares, múltiplos de $m$). Testear con variaciones asegura que funciona sin importar la entropía de los datos.
5. A mejor dispersión matemática, menor número de colisiones, lo cual garantiza de manera directa el cumplimiento estadístico del costo esperado $O(1)$.

**Conclusión sobre el papel de la función hash:**
La función hash es el motor exclusivo que separa a una Hashtable de una ineficiente lista enlazada. Sin dispersión uniforme, la teoría detrás del coste $O(1)$ colapsa de inmediato.

## Bloque 9 - Rehashing, política de carga y costo amortizado

| `maxLoad` | Capacidad | Número de elementos | Factor de Carga (`load`) | Máximo de sondeos (`maxProbe`) | Promedio de sondeos (`avgProbe`) | Desempeño (`time_us`) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **0.45** | 32768 | 5000 | 0.152588 | 13 | 1.32887 | 1027 |
| **0.65** | 8192 | 5000 | 0.610352 | 31 | 1.93973 | 710 |
| **0.85** | 8192 | 5000 | 0.610352 | 69 | 2.44807 | 773 |

1. Cuando la proporción lógica $n/m$ sobrepasa el límite establecido por diseño (frecuentemente $0.5$ o $0.75$).
2. Cuando la proporción ocupada (`size` + `deleted`) rebasa el límite, obligando al arreglo a reestructurarse para expulsar físicamente la basura virtual.
3. Cuando tras repetidas eliminaciones el factor de carga cae por debajo de cierto límite perezoso (ej. $\alpha < 0.125$), para liberar memoria al SO.
4. Porque se debe instanciar un arreglo nuevo y reevaluar matemáticamente el hash de cada uno de los $n$ elementos presentes.
5. Porque el evento costoso ($O(n)$) sucede tras haber procesado individualmente $n$ inserciones baratas ($O(1)$). Al promediar la penalidad entre todas las transacciones, el impacto tiende matemáticamente al costo unitario constante.
6. A mayor factor de carga, los buckets se densifican o los bloques colisionan, elevando irremediablemente la cantidad de sondeos requeridos para hallar un espacio libre.
7. Muestra incrementos escalonados en el log, registrando cuántas transferencias de memoria masiva ocurrieron a lo largo de la ejecución en vivo.
8. De permitir una saturación, el rendimiento decaerá exponencialmente hacia el $O(n)$, y en esquemas de Open Addressing provocará un loop infinito al saturarse todas las celdas disponibles.

**Interpretación de `test_rehashing.cpp`:**
La prueba valida que, al inyectar miles de datos simultáneamente, el sistema no crashea ni degrada su búsqueda. Asegura que la tabla crezca y, lo más importante, que todos los datos previamente insertados sigan siendo localizables tras redimensionarse los módulos subyacentes.

## Bloque 10 - Aplicaciones de hashing

**Trazados Manuales:**
1. **`hasDuplicates`** (Detectar colisión lógica rápida)
   * Entrada: `[15, 20, 15]`
   * Proceso:
     * Iterar `15`: Hash table inserta `15` -> true.
     * Iterar `20`: Hash table inserta `20` -> true.
     * Iterar `15`: Hash table intenta insertar `15` -> false (¡Ya existe!).
   * Salida: Retorna `true` (se encontró un duplicado).
2. **`frequencyCount`** (Contar incidencias usando diccionario `Valor -> Frecuencia`)
   * Entrada: `["a", "b", "a"]`
   * Proceso:
     * `"a"`: get("a") es null, put("a", 1).
     * `"b"`: get("b") es null, put("b", 1).
     * `"a"`: get("a") retorna 1, se actualiza -> put("a", 2).
   * Salida: `[("a", 2), ("b", 1)]`.

**Respuestas para `hasDuplicates`:**
1. **Entrada:** Un arreglo lineal genérico de datos.
2. **Salida:** Un booleano advirtiendo la existencia de elementos idénticos.
3. **Se guarda:** Los elementos únicos procesados hasta el momento.
4. **Operación dominante:** La búsqueda de presencia (el `get` virtual que antecede al `insert`).
5. **Costo lineal:** Como el Hash provee inserción/búsqueda $O(1)$, iterar todo el arreglo cuesta $O(n)$.
6. **Caso adverso:** Una función hash atroz que force colisiones estructurales por cada elemento.
7. **Resolución con AVL:** Añadiendo datos al árbol secuencialmente. Pero como AVL demora $O(\log n)$ por paso, iterar costaría $O(n \log n)$.
8. **Gana/Pierde:** Gana velocidad asintótica pura ($O(n)$ vs $O(n \log n)$). Se pierde la capacidad de emitir los duplicados en orden lógico.

**Respuestas para `frequencyCount`:**
1. **Entrada:** Vector de palabras.
2. **Salida:** Diccionario asimétrico asociando la palabra con su conteo absoluto.
3. **Se guarda:** `<string, int>`. Clave (palabra), Valor (contador numérico).
4. **Operación dominante:** Recuperación y modificación in-situ de valores.
5. **Costo lineal:** Actualizar el registro toma un promedio $O(1)$. Un barrido completo rinde $O(n)$.
6. **Caso adverso:** Sobresaturar el arreglo causando rehashes violentos si el número de palabras únicas es astronómico.
7. **Resolución con RBT:** Un `std::map<string, int>`. Búsquedas costarían recorridos logarítmicos, encareciendo el bucle a $O(n \log n)$.
8. **Gana/Pierde:** Se gana latencia bajísima. Se sacrifica listar subsecuentemente las frecuencias en estricto orden alfabético.

## Bloque 11 - Modificación controlada de código

Opción A
```cpp
#include <iostream>
#include <vector>
#include "ChainedHashTable.h"
#include "LinearHashTable.h"
#include "HashStats.h"

// MOD-A8: Función auxiliar para reporte uniforme de métricas
void printHashStats(const ods::HashStats& stats) {
    std::cout << "  - Colisiones: " << stats.collisions << std::endl;
    std::cout << "  - Sondeos totales: " << stats.totalProbeLength << std::endl;
    std::cout << "  - Maximo de sondeos: " << stats.maxProbeLength << std::endl;
    std::cout << "  - Rehashes: " << stats.rehashes << std::endl;
    std::cout << "  - Tombstones (en el momento del reporte): " << stats.tombstones << std::endl;
}

int main() {
    std::cout << "--- Demostracion de Reporte Uniforme de Metricas ---" << std::endl;
    
    // Caso 1: ChainedHashTable
    ods::ChainedHashTable<int> chained_table;
    chained_table.enableStats();
    for (int i = 0; i < 100; ++i) {
        chained_table.add(i * 2);
    }
    
    std::cout << "\nMetricas para ChainedHashTable:" << std::endl;
    printHashStats(chained_table.getStats());

    // Caso 2: LinearHashTable
    ods::LinearHashTable<int> linear_table;
    linear_table.enableStats();
    for (int i = 0; i < 50; ++i) {
        linear_table.add(i * 3);
    }
    for (int i = 0; i < 20; ++i) {
        linear_table.remove(i * 3);
    }
    
    linear_table.countTombstones(); // Actualizar conteo antes de reportar
    
    std::cout << "\nMetricas para LinearHashTable (con eliminaciones):" << std::endl;
    printHashStats(linear_table.getStats());

    return 0;
}
```

- Se agregó la función `void printHashStats(const ods::HashStats& stats)`, que toma una referencia constante a un objeto `HashStats` y muestra sus campos por consola de manera legible.
La función en sí no mantiene un invariante de estructura, ya que solo actúa como un observador. Su correctitud depende de que las estructuras subyacentes (`ChainedHashTable`, `LinearHashTable`, etc.) mantengan sus propios invariantes y actualicen correctamente la estructura `HashStats` durante cada mutación. Por ejemplo, `LinearHashTable` debe garantizar que el invariante de estados (`size <= occupied <= capacity`) se cumpla al generar el reporte.
- Evidencia el cambio. Muestra cómo la misma función `printHashStats` sirve de interfaz polimórfica para reportar el rendimiento de dos implementaciones totalmente distintas (`ChainedHashTable` y `LinearHashTable`), demostrando utilidad y uniformidad.
- El costo es **$O(1)$** constante. La función `printHashStats` únicamente accede en lectura a los campos en memoria de un `struct` que ya fueron calculados previamente. No contiene ciclos iterativos ni depende de la cantidad de elementos ($N$) en la tabla hash.
- Esta modificación es pasiva; es una utilidad de observación y diagnóstico, no de operación. La función de reporte no altera la lógica matemática de cómo se resuelven las colisiones (probing o chaining), no genera saltos hash, ni oculta los métodos nativos del código original. Simplemente audita los resultados sin intervenir en la topología de la estructura.


## Bloque 12 - Comparación final con BST, AVL, Red-Black Tree y Treap

**Matriz de Decisión:**
| Estructura | Mantiene orden | Búsq. promedio | Búsq. peor caso | Inserción | Eliminación | Memoria adicional | Ventaja principal | Riesgo principal | Uso recomendado |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| **BST** | Sí | $O(\log n)$ | $O(n)$ | $O(\log n)$ | $O(\log n)$ | Ninguna | Sencillo de programar | Degeneración lineal | Uso académico / datos aleatorios pre-validados |
| **Treap** | Sí | $O(\log n)$ | $O(n)$ | $O(\log n)$ | $O(\log n)$ | Prioridad (int) | Inmune a entradas ordenadas usando azar | Casos malos probabilísticos raros | Sistemas concurrentes / particiones rápidas |
| **AVL** | Sí | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | Factor altura | Búsquedas ultra-rápidas | Rotaciones frecuentes | Aplicaciones `Read-Heavy` (Diccionarios de consulta) |
| **Red-Black**| Sí | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | Color (1 bit) | Inserciones y escrituras robustas y rápidas | Mayor código y complejidad | Aplicaciones mixtas (Maps nativos en C++) |
| **Chained** | No | $O(1)$ | $O(n)$ | $O(1)$ | $O(1)$ | Punteros a Listas | Soporta enormes factores de carga | Rendimiento caché deficiente | Datos hiper-dinámicos impredecibles |
| **Linear OA**| No | $O(1)$ | $O(n)$ | $O(1)$ | $O(1)$ | Celdas sobrantes | Imbatible acceso a CPU Cache | Clustering Primario | Memorias controladas o estáticas |
| **Hash OA** | No | $O(1)$ | $O(n)$ | $O(1)$ | $O(1)$ | Ninguna/Tombstones| Versátil y personalizable | Costoso manejo de tombstones | Implementaciones rápidas embebidas |

1. Cuando importan latencias microscópicas y el orden estructural de la data es irrelevante.
2. En sistemas donde el orden importa y la gran mayoría de transacciones sean operaciones de lectura y consultas.
3. En contextos relacionales ordenados donde existen inyecciones y remociones incesantes de datos.
4. Cuando se valora la inmutabilidad persistente o se requieren divisiones probabilísticas sin complicadas lógicas estructurales.
5. Porque la naturaleza estocástica del hash atomiza los datos, imposibilitando ubicar vecinos lógicos (claves similares en valor).
6. El invariante topológico universal (izquierda menor, derecha mayor) obliga al árbol a escupir una lista nativamente ordenada mediante el recorrido `inorder`.
7. Garantizado ($O(\log n)$) indica el techo absoluto del peor caso mediante pruebas rigurosas. Esperado ($O(1)$) asume una media estadística bajo distribuciones prolijas.
8. Implementación `ChainedHashTable` u `Open Addressing`.
9. Un `RedBlackTree` o `AVL`.
10. Obligatoriamente cualquier árbol autobalanceado (AVL / RBT), pues requieren descensos comparativos basados en umbrales.

**Conclusión técnica personal:**
El desarrollo lógico desde el BST hasta el Hashing culmina en la comprensión de las contrapartidas fundamentales del diseño de estructuras de datos. Las topologías basadas en árboles balanceados (AVL y RBT) garantizan su eficiencia imponiendo contratos matemáticos estrictos que suprimen los peores casos asintóticos, consagrándolas como las estructuras soberanas cuando el orden de los datos debe prevalecer en todo momento (permitiendo consultas paramétricas y rangos). Por otro lado, la semana 8 nos revela la ventaja incomparable de abandonar el ordenamiento lógico: el hashing convierte los lentos descensos logarítmicos en saltos de memoria directos. Cuando los requerimientos del software exigen latencias $O(1)$ y el orden es prescindible, las tablas hash orquestadas sobre buenas funciones de dispersión se consolidan como las implementaciones indiscutidas.
- **Ejemplo donde hashing gana:** Indexación de sesiones en un proxy web masivo mediante *Token -> UserID*.
- **Ejemplo donde AVL/RBT gana:** Un motor de base de datos relacional buscando transacciones emitidas entre `FechaA` y `FechaB` (`lowerBound` y rangos).