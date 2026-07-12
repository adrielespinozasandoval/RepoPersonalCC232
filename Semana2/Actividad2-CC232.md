## Actividad 2-CC232

### Integrantes
- Espinoza Sandoval, Adriel Alejandro
- Pantoja Limache, Fabio Alberto

#### Bloque 1
1. Es un arreglo cuyos elementos están guardados con posiciones seguidas en la memoria física de la computadora.
2. Porque la memoria de la computadora es limitada, acceder a esa memoria tiene un costo acotado.
3. En un vector, size es el tamaño de los elementos usados, y capacity el tamaño de memoria reservada para guardar los elementos.
4. Porque las demás direcciones de memoria cercana podrían estar siendo usadas para otros procesos.
5. Tanto ArrayStack como DengVector son implementaciones de un arreglo dinámico que puede cambiar su tamaño. Difieren en su interfaz, ya que uno 
6. En el 
7. Mejora en la implementación de funciones nativas de C++, haciendo las copias más rápidas. Esto no cambia su costo asintótico.
8. RootishArrayStack "divide" un ArrayStack en bloques separados en la memoria.
9. porque asi la capacidad total crece como suma triangular, permitiendo ajustar el espacio de forma mas fina y mantener bajo el desperdicio, en lugar de reservar grandes sobrantes contiguos.
10. La representacion define que tan facil es acceder o mover datos. Con bloque contiguo unico se logra acceso simple y rapido, pero inserciones/eliminaciones internas desplazan muchos elementos y puede haber holgura de capacidad. Con representacion por bloques se complica el mapeo interno, pero se mejora el uso de espacio. 

#### Bloque 2
| Archivo | Salida u observable importante | Idea estructural | Argumento de costo o espacio |
| --- | --- | --- | --- |
|`Semana2/demos/demo_array_basico.cpp`|Despues de `b=a`, el arreglo `b` pasa a tener los datos de `a` y su tamaño|<ul><li> Diseñada como un contenedor minimo con dos campos: puntero a memoria dinamica `a` y tamaño logico `length`. </li><li>La decision estructural central es `ownership unico` del buffer interno: la asignacion mueve ese ownership al destino, en vez de clonar contenido.|<ul><li> Complejidad temporal $O(1)$</li> |
|`Semana2/demos/demo_arraystack.cpp`|Despues de `add(1,15)`, el elemento  se inserta en posicion 1 y 20 se desplaza a la derecha. El `remove(1)` extrae ese 15 y los restantes se desplazan :`10 20`|<ul><li> Mantiene un array dinamico `a` con un contador logico `n` de elementos activos.</li><li> Las inserciones/eliminaciones desplazan elementos en el array para mantener continuidad</li> |<ul><li> Insercion/eliminacion en posicion `i`: $O(n)$.</li><li> Acceso get(i) :$O(1)$.</li><li> Redimension amortizada: $O(1)$.</li><li> Espacio $O(n)$ con overhead por capacidad de reserva</li> |
|`Semana2/demos/demo_arraystack_explicado.cpp`|Despues de `push_back(20)` hubo resize, luego inserta 20 <br> Despues de `add(1,15)` hubo resize, se desplaza 20 a la derecha.<br> Despues de `remove(0)` remueve 10, desplaza valores a la izquierda. |<ul><li>`add(i,x)` inserta en posicion i, desplazando a la derecha desde `i` hasta `n-1`.</li><li>`remove(i)` elimina en `i`, desplanzado a la izquierda desde `i+1` hasta `n-1`.</li><li> `resize` rehace el arreglo con tamaño `max(2n,1).</li><li>`push_back(x) es solo un `add(n,x)`</li> |<ul><li>`push_back(x)` es `$O(1)$`amortizado, porque normalmente inserta la final sin desplazar; ocacionalemnte hace resize `$O(n)$` </li><li>`add(i,x)` es `$O(n-i)$` por desplazamiento y en algunos casos `$O(n)$` adicional por resize.</li><li>`remove(i)` es `$O(n-i-1)$` por desplazamiento</li> |
|`Semana2/demos/demo_fastarraystack.cpp`|<ul><li>Inserta `0,1,2,3,4,5`al final con `s.add(s.size(),i)`.</li><li>Luego inserta `99` en el indice `3` con `s.add(3,99)`, desplazando a la derecha los elementos desde esa posicion.</li></ul> |<ul><li> `add(i,x)` <ul><li>Si no alcanza capacidad redimensiona.</li><li>Usa `std::copy_backward(a+i,a+n,a+n++i)` para abrir espacio.</li><li>Escribe `x` en `a[i]` y aumenta `n`</li></li></ul><li>`remove(i)` <ul><li>Guarda el valor `a[i]`</li><li>Usa `std::copy(a+i+1,a+n,a+i)` para cerrar el hueco</li><li>Decrementa `n` y puede "achicar"</li><li>Finalmente devuelve `a[i]`</li> </li> |<ul><li>`add(i,x)` <ul><li>Desplazamiento de (n-i) elementos `$O(n-i)$`</li><li>Si hay `resize`, copia n elementos `$O(n)$` extra</li><li>Peor caso `O(n)`</li></ul><li>`remove(i)` <ul><li>Desplaza `(n-i-1)` elementos `$O(n-i)$`</li><li>Reduccion ocasional puede costar `$O(n)$`.</li><li>Peor caso: `$O(n)$`</li></ul> </li> </li> |
|`Semana2/demos/demo_rootisharraystack.cpp`|<ul><li>Se insertan 8 lementos con i= 0...7 usando `add(i,i*10)`, en este caso equivale a insertar al final cada vez.</li><li>La secuencia quedad `0 10 20 30 40 50 60 70`</li> |La estructura en `RootishArrayStack.h` no usa solo un arreglo grande, usa bloques de tamaños creciente 1,2,3,4... guardados en blocks.<ul><li>Ubicar un indice i : <ul><li>calcula el bloque b con `i2b(i)`</li><li>calcula desplazamiento interno `j=i-b(b+1)/2` </li><li>accede como blocks[b][j]</li></ul><li>Para crecer/disminuir <ul><li>grow agrega un nuevo bloque de tamaño `r+1`</li><li>shrink elimina bloques sobrantes al final</li></ul> </li></li> |<ul><li>`add(i,x)` `$O(n-i)$` por desplamientos</li><li>`remove(i)` `$O(n-1)$` por desplazamientos</li><li>`grow`/`shrink` son ocacionales, su costo amortizado.</li></ul>|
|`Semana2/demos/demo_rootisharraystack_explicado.cpp`|<ul><li>La secuencia logica de elementos antes y despues de `add`/`remove`</li><li>Como se mapean indices logicos a (bloque,offset) con locate.</li><li>El estado interno por bloques que imprime `debug_print`</li></ul> |<ul><li>No hay un solo arreglo grande</li><li>Hay bloques de tamaños crecientes 1,2,3...</li><li>Un indice logico `i` se transforma a bloque y desplazamiento interno</li><li>`add`/`remove` hacen desplazamientos logicos usando `set`/`get` (como array dinamico), pero fisicamente los datos estan repartidos en bloques.</li></ul> |<ul><li>`add(i,x)` es `$O(n-i)$` por desplamientos</li><li>`remove(i)` es `$O(n-i)$` por desplamientos</li><li>`push_back(x)` es `$O(1)$` amortizado, con crecimiento ocaciona.</li></ul> |
|`Semana2/demos/demo_deng_vector.cpp`|<ul><li>Despues de insertar 8 lementos, se observa `size` y `capacity` mostrando crecimiento</li><li>Despues de `insert(2,99)` se observa el arreglo con 99 en la posicion 2</li><li>`remove(3)` devuelve 20, evidencia que elimina y retorna el elemento correcto.</li><li>`remove(1,3)` elimina 2 elementos.</li><li>`find(50)` devuelve -1, evidencia de busqueda lineal y que 50 no esta.</li></ul> |La ides estrucutral de Dengvector es un arrglo dinamico clasico:<ul><li>Crecimiento<ul><li>En `DengVector.h`, si `size==capacity`, duplica capacidad</li></ul></li><li> Reduccion <ul><li>En `DengVector.h`, reduce la capacidad a la mitad cuando esta poco ocupado (cuando `size<=capacity/4`)</li></ul></li><li>Operaciones <ul><li>`insert(r,e)`desplaza a la derecha desde la posicion r</li><li>`remove(r)` desplaza a la izquierda para cerrar el hueco</li><li>`find(e)` busqueda lineal (de derecha a izquierda) </li></ul></li> </li> </ul> |Operaciones <ul><li>`insert(r,e)` `$O(n-r)$` por desplazamientos </li><li>`remove(r)` `$O(n-r)$` por desplazamiento</li><li>`find(e)` O(n) en el peor caso.</li><li>`traverse` `$O(n)$`</li><li>Constructor de copia y asignacion `$O(n)$`.</li></ul></li> </li> </ul>|
|`Semana2/demos/demo_stl_vector_contraste.cpp`|Se realiza 8 inserciones con `push_back(i*10)` e imprime `size` y `capacity` en cada paso.|`std:vector` es un arreglo dinamico generico estandar de c++:<ul><li>Internamente matiene un puntero a un arreglo contiguo en el heap</li><li>Mantiene`size` y `capacity`</li><li>Inserciones al final `push_back`</li></li> |<ul><li>`push_back(x)` O(1) amortizado, </li></ul>|

1. Queda claro que un arreglo tiene longitud fija desde su creacion, que los elementos viven en memoria contigua y que la asignacion se hace por indice. Tambien suele evidenciarse que no existe redimensionamientos automatico, si falta espacion no "crece solo"; hay que crear otro arreglo y copiar.
2. La operacion que mjeor exhibe el costo por desplazamientos es insertar o eliminar cerca del inicio. Ahi se deben mover muchos elementos para mantener el orden lo que hace visible el costo lineal $O(n)$.
3. Cambia la estrategia de implementacion, usa `std:copy` / `std::copy_backward` en lugar de bucles manuales.
4. El ejemplo de mapeo indice~bloque, suele tomar un indice concreto y mostrar como se ubica en un bloque truangular y offset interno. 
5. El observable que permite defender el crecimiento de capacity es la traza de size vs capacity durante multiples inserciones.
6. Ambos manejan size y capacity, usan memoria contigua y aplican crecimiento geometrico para lograr push_back amortizado (cerno a $O(1)$).
7. Para defender amortizacion: `demo_deng_vector.cpp`. Para defender uso de espacio : `demo_rootisharraystack_explicado.cpp`

#### Bloque 3
1. Prueba el funcionamiento de `add` (inserción), `size`, `get` y `remove`.
2. Prueba el funcionamiento de `add`, `size`, `get` y `remove`.
3. Prueba el funcionamiento de `add`, `size`, `get` y `remove`.
4. Demuestra un comportamiento válido mínimo esperado.
5. No demuestra comportamientos más internos, como invariantes, costos, casos bordes, etc.
6. Se intenta estresar su estabilidad, ya que en las pruebas los arreglos dinámicos se hacen crecer y decrecer, requiriendo de una reserva de un bloque nuevo y copiar todos sus elementos hacia allá.
7. Porque el comportamiento de un algoritmo su análisis se centra más en la implementación de código.

#### Bloque 4
1. <ul><li>_size: Cantidad logica de elementos validos del vector.</li><li>_capacity: espacio fisico reservado en el arreglo interno.</li><li>_elem: puntero al bloque contiguo donde estan guardados los elementos.</li></ul>
2. Se invoca antes de insertar, y expande cuando _size == _capacity.
3. Para abrir hueco en la posicion r, el sufijo [r,_size) debe moverse una celda a la derecha.Si no se desplaza se perderian datos o se romperia el orden logico.
4. <ul><li>remove(i): elimina un solo elemento y devuelve ese elemento</li><li>remove(lo,hi): elimina un intervalo semiexclusivo [lo,hi) y devuelve cuantos elementos borro</li></ul> 
5. Se crea una copia por constructor y otra por asignacion. Luego se aplican tranformaciones distintas, a una se le suma 1 y a la otra se le multiplica por 2. Que ambos resultados diverjan demuestra que no comparten el mismo arreglo interno.
6. Separa el recorrido de la operacion aplicada a cada elemento, permite mostrar dos estilos de vista ( funsion y objeto funcion) y ayuda a aprender abstraccion.
7. Hace explicitos los costos reales de insertar/eliminar y el porque de la amortizacion. Obliga a entender la representacion interna, invariantes y manejo de memoria dinamica.

#### Bloque 5
1. Los elementos se guardan en bloques de tamaño creciente respecto al anterior. Siendo el primer bloque de tamaño 1, el segundo 2, etc.
2. Ya que cada bloque i tiene tamaño i, la capacidad sería 1 + 2 + ... + r = r*(r+1)/2
3. Halla el bloque en el que está guardada la posición i del RootishArrayStack.
4. Además de hallar el bloque en el que está guardada, también hallar la posición de ese elemento respecto al bloque.
5. Al fragmentar el espacio, no es necesario a 
6. Los métodos de insertar elementos, eliminar, escribir y acceder a elementos.
7.

#### Bloque 6
1. operador[] aporta una interfaz natural deacceso por rango/posicion, equivalente al arreglo.
2. find(e) supone que existe una nocion de igualdad entre elementos del tipo almacenado. Sin esa igualdad, la busqueda por valor no esta bien definida.
3. traverse() muestra que toda la estructura puede procesarse de manera uniforme aplicando una misma operacion a cada elemento, separando el recorrido de la logica especifica de transformacion o vista.
4. Porque explica exactamente sus fundamentos: Representacion interna, redimensionamiento dinamico, costos de insersion/eliminacion y analisis amortizado

#### Bloque 7
Al pasar de usar un arreglo a diseñar una estructura dinámica basada en arreglo, se cambia de una colección fija de posiciones a un diseño con representación explícita (tamaño lógico, capacidad y política de redimensionamiento, o bloques con mapeo índice-bloque), donde la correctitud depende de mantener invariantes en cada operación (tamaño consistente, orden correcto tras desplazamientos y ausencia de pérdida de datos), el rendimiento se justifica con costo amortizado (aunque un resize puntual cueste mucho, una secuencia larga de inserciones al final puede sostener O(1) amortizado), y el uso de espacio se vuelve una decisión de ingeniería (aceptar holgura para acelerar tiempo o reducir desperdicio); en esa línea, ArrayStack es la versión más directa con arreglo contiguo, FastArrayStack mantiene la misma idea y complejidad asintótica pero mejora constantes de implementación, y RootishArrayStack conserva la interfaz de lista pero reorganiza en bloques 1,2,3,... para reducir desperdicio espacial a cambio de mayor complejidad interna de mapeo.