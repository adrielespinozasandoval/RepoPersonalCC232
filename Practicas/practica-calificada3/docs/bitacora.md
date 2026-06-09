## 29 mayo 2026

### Avance de actividades

Se procedió a subir los avances parciales de las actividades de las semanas 5 y 6

## 3 junio 2026

### Reestructuración del repositorio

Se procedió a restructurar el repositorio completo para coincidir con la estructura de la rúbrica de la Práctica Calificada 3

## 6 junio 2026

### Implementación de Heap

Se implementó dicha estructura en un incio para la solución del problema.
- Utilizando la invariante de que el nodo padre es menor/mayor que los hijos se podían construir un min-heap y max-heap para `window` y `least`

Sin embargo, según la rúbrica no era un tema de apoyo (Heap es de semana 4 mientras se pedía semana 5 y 6), y además introducía *lazy deletion* lo cual trae un consumo de memoria innecesario y la solución de `Compare` podía romper la invarianza, lo cual requería ser solucionado por `heapify`, trayendo así ineficiencia en el lado temporal también.
- Finalmente, se tomó la decisión de eliminar esta solución en favor de una estructura más óptima para este problema en específico.

### Implementación de Treap

Se implementó esta estructura, haciendo obsoleto a Heap.
- Utilizando la invariante BST, la cual es útil para hallar el máximo y mínimo.
- Esta estructura además es tema de apoyo explícito en la rúbrica y no introduce *lazy deletion*.
- Aunque se sigue usando un comparador *custom* (`TreapComparator`) que sirve como desempate entre dos elementos iguales, no rompe la invarianza como en la versión anterior.
- Y más en general, trae una sobrecarga menor y por lo tanto un mejor rendimiento.

### Implementación de LeastElements

Se implementó la interfaz que encapsula la solución del problema:
- Utilizando un min-heap y max-heap al principio, después cambiando a favor de dos Treaps, `window_` y `least_`.
- Teniendo un constructor que incializa las estructuras, y separando también en métodos para iterar en las soluciones y sumar los $k$ menores elementos.

### Estructuración de código
- Para mantener la generalidad de tipos, es decir que no sea exclusivo de por ejemplo `std::vector<int>`, se decidió usar templates para el tipo trabajado (`T`), contenedor (`Container`), iterador (`Iterador`) y generador de prioridades (`PriorityGen`)
 - A su vez se implementó `traits` para políticas para los tipos. Ejemplo: contenedor debe tener iterador, comparador debe poder comparar entre dos tipos iguales, etc.
- En el caso de `Treap`, la implementación fue más directa y tuvo fuerte inspiración en la misma estructura de la librería `cc232`.
 - Se le añadió tipo de iterador explícito y métodos que operan sobre ellos (ya que `const Node*` no puede ser convertido a `Node*` sin `const_cast` es cual no es recomendado)
- En el caso de `LeastElements` se decidió usar una clase la cual da una interfaz más sencilla de entender y usar para los desarrolladores, usando encapsulación de métodos e inferencia de tipos.

### Ejecución de pruebas
Se tuvo en un inicio a `public_tests/test.cpp` que funciona como compilable que verificara los errores de compilación y en alguna cabecera, y con ayuda de GDB para la depuración en el caso de bugs (los cuales fueron mínimos a comparación de los de compilación).
- Se encontró sobre todo, errores referentes a operación de clases y *typos*.

### Adición de QA (pruebas públicas e internas)
Se añadió `QAUtils.h` que incluye librerías estándar de C++ y utilidades para la ejecución de pruebas, demos, benchmarks, etc.
A su vez, también se añadieron pruebas públicas e internas, tomando el ejemplo del problema asignado así como pruebas propias y casos borde (verificadas manualmente).

### Adición de gitignore
Añadido para evitar filtración de ejecutables generados en el repositorio.

## 7 junio 2026

### Continuación de QA
Se procedieron a añadir demos, benchmark y más utilidades correspondientes.
- Se incluyó una demo interactiva, cuyas entradas están determinadas por el usuario.

### Adición de `CMakeLists.txt`
Se añadió dicho archivo para generar los ejecutables con CMake.
- También se añadio en el `.gitignore` ignorar las carpetas `build` generadas.

### Adición de `results/`
Se añadieron, como indica la rúbrica, los resultados al compilar las pruebas y los comandos ejecutados para llegar a ellos.

### Adición de `docs/`
Se añadieron, según rúbrica, la mayoría de las respuestas obligatorias y la bitácora que resume el historial de progreso del proyecto.

### Adición de README
Se añadió el documento que presenta el proyecto, el estudiante, el ejercicio propuesto, resumen de solución, pasos para compilar e información del repositorio.

## 8 junio 2026

### Adición de video y reto en vivo
Se realizó el reto en vivo, realizando acciones pedidas por rúbrica incluido el reto grabado obligatorio específico del problema.

### Actualización de `docs/`
Actualizado `preguntas_obligatorias.md` -> `respuestas_obligatorias.md` para incluir respuestas del reto grabado, y la bitácora.

### Corección menor del README
Corregido opción del *build type* de CMake.

### Adición de verificación estática de tipo
Se añadió `is_substractable_v` ya que en la nueva solución del reto se usa resta para actualizar la suma de los $k$ menores.

### Adición y término de la actividad 4
Se completó la actividad de clase CC232 de la semana 4.