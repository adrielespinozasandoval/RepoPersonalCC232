# PC3 - CC232

- **Tema:** Ventana deslizante de los $k$ menores elementos
- **Estudiante:** Espinoza Sandoval, Adriel Alejandro
- **Código:** 20243502F
- **Problema asignado:** 10. AtCoder ABC281 E - Least Elements
- **Enlace:** https://atcoder.jp/contests/abc281/tasks/abc281_e

## Referencia oficial

- CSV oficial: https://github.com/kapumota/CC-232/blob/main/Practicas/Practica3_CC232/Problemas-Evaluacion3.csv

## Relación con semanas 4-6

- Semana principal: 5
- Estructura usada: Treap
- Estructura de la librería `cc232` relacionada: `Treap.h`

## Descripción

Conocido como `LeastElements` en la implementación de C++17, es una estructura diseñada para extraer y sumar los $k$ menores elementos de una subsecuencia (ventana) que se *desliza*, es decir que va avanzando y añadiendo nuevos elementos mientras los más viejos se van eliminando.

En esta implementación usa dos Treaps, usando principalmente la invarianza BST para orden y alcanza una complejidad asintótica de $O(n \log m)$.

## Contenido

La carpeta contiene las siguientes cabeceras:

- `Traits.h`: interfaz para verificación estática de tipos para comparadores, generadores, iteradores y contenedores.
- `Treap.h`: estructura de árbol que mantiene tanto la propiedad BST para los valores del nodo como Heap para las prioridades.
- `LeastElements.h`: interfaz para solución final para el problema.
- `QAUtils.h`: utilidades de *quality assurance* como pruebas públicas, pruebas internas, demostraciones y benchmark.

## Organización

Ademas de las cabeceras, la carpeta contiene

- `demos/`: programas para demostración del funcionamiento de las estructuras implementadas
- `public_tests/`: pruebas públicas esperadas para validar comportamiento básico
- `internal_tests/`: pruebas adicionales de cobertura de casos borde y comprobación de estados
- `benchmark/`: pruebas de medición del rendimiento de las implementaciones
- `docs/`: documentos como respuestas al desarrollo de la práctica y del repositorio
- `results/`: resultados obtenidos a la ejecución de las pruebas y demostraciones

## Diseño

Se mantuvo un estilo *header-only* similar a libería `cc232` para mantener la coherencia, así como templates para tipos genéricos que cumplan con un cierto ADT dictado por la STL (como contenedores). Se añadió principalmente:

- `cc232::traits`: implementación de verificación estática de tipos
- `cc232::Treap<T, Compare, PriorityGen>`: implementación de Treap con comparador y generador de prioridades personalizables.
- `cc232::LeastElements<T, Iterator, Compare>`: implementación de interfaz para la resolución del problema propuesto.

Así como también algunas utilidades extra para el funcionamiento de dichas estructuras y QA.

## Compilación

Es recomendable realizar la compilación dentro de la carpeta del proyecto
```sh
cd practica-calificada3
```

A continuación se presenta la compilación en modo de prueba:

### Linux

**1. Configurar el proyecto**
```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
```

**2. Compilar los targets**
```bash
cmake --build build-debug
```

**3. Ejecutar todas las pruebas**
```bash
ctest --test-dir build-debug --output-on-failure
```

**4. Ejecutar demos**

Un ejemplo:
```bash
./build-debug/pc3_demo_interactive
```
Para la lista de los targets de demostraciones mirar Targets.

### Windows

**1. Configurar el proyecto**
```powershell
cmake -S . -B build-debug
```

**2. Compilar los targets**
```powershell
cmake --build build-debug --config Debug
```

**3. Ejecutar todas las pruebas**
```powershell
ctest --test-dir build-debug -C debug --output-on-failure
```

**4. Ejecutar demos**

Un ejemplo:
```powershell
.\build-debug\pc3_demo_interactive.exe
```
Para la lista de los targets de demostraciones mirar Targets.

## Targets

### Demos

- `pc3_demo_leastelements`
- `pc3_demo_interactive`
- `pc3_demo_treap`

### Pruebas

- `pc3_test_public`
- `pc3_test_internal_edge_cases`
- `pc3_test_internal_states`

### Benchmark

- `pc3_bench_leastelements`

## Declaración de autoría

```
Declaro que entiendo el código entregado, que puedo explicarlo, compilarlo, ejecutarlo y modificarlo sin ayuda externa durante la grabación.
También declaro que el repositorio entregado corresponde al trabajo mostrado en el video y que no contiene builds, ejecutables ni archivos generados usados para aparentar funcionamiento.
```