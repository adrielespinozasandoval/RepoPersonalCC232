## Actividad 4-CC232

### Estudiante
- **Nombre completo:** Espinoza Sandoval, Adriel Alejandro

#### Bloque 1 (respuestas)

1. LIFO (Last In, First Out) es una estructura de pila donde el último elemento insertado es el primero en ser retirado, operando sobre un único extremo (el tope). FIFO (First In, First Out) es una estructura de cola donde el primer elemento insertado es el primero en ser procesado, operando con un extremo para entradas y otro para salidas.
2. La pila modela naturalmente dependencias anidadas o decisiones reversibles, porque la sub-tarea más reciente generada se sitúa en el tope y siempre será la primera en resolverse antes de poder concluir tareas previas.
3. Modela equidad cronológica. En escenarios como ventanillas bancarias, asegura que el tiempo de espera se distribuya en orden exacto de llegada, atendiendo primero al cliente más antiguo en el sistema.
4. Consiste en sustituir la memoria gestionada por el compilador (el "call stack" de las funciones) por una instancia manual de `Stack` dentro de un bucle iterativo, previniendo el desbordamiento (Stack Overflow) y otorgando control absoluto sobre el estado intermedio.
5. Debe guardar el estado local o variables instantáneas que permitan reanudar un proceso pausado.
6. Comparten la lógica de extraer el dígito con `n % base` y reducir el número con `n / base`. Cambia el control: la recursión deja que el compilador invierta el orden de impresión al regresar de las llamadas; la versión iterativa apila manualmente y luego desapila (`popAll`) para invertir la secuencia de dígitos.
7. Porque cada vez que aparece un símbolo de cierre, este debe emparejarse y resolver exclusivamente la apertura *más reciente* que sigue sin ser procesada.
8. Una pila `operands` almacena los valores temporales calculados, y otra pila `operators` retiene los operadores que están a la espera de que se resuelva su prioridad respecto a los operadores que vienen a continuación.
9. Ambos son procesos de búsqueda sistemática por ensayo y error. Avanzan acumulando un camino en la pila y, al detectar un estado sin salidas válidas, hacen *pop* para retroceder un paso e intentar la siguiente ruta alternativa.
10. Usar una pila atendería al último cliente que entró por la puerta, dejando a los clientes iniciales esperando indefinidamente. Las colas imponen la política FIFO justa de los bancos.
11. La estructura auxiliar alberga el progreso y garantiza que se cumplan las políticas de recorrido. Si la estructura almacena el estado invariante correctamente, permite barrer el árbol de soluciones íntegramente asegurando la correctitud del algoritmo subyacente.
12. "Resolver" encuentra un resultado final y estático a través de un espacio de estados (ej. ruta en un laberinto). "Simular" imita un proceso paso a paso durante un tiempo determinado; la evolución y las estadísticas obtenidas son el foco del estudio, no hay una única "solución".

#### Bloque 2 (tabla y respuestas)

|Archivo|Salida u observable importante|Estructura o técnica central|Qué concepto permite defender|
|------|------|------|------|
|`demo_stack_queue.cpp`|Secuencia invertida vs en orden.|`Stack` vs `Queue`|Visualizar la diferencia fundamental entre políticas LIFO y FIFO en los datos.|
|`demo_base_conversion.cpp`|Cadenas numéricas idénticas devueltas.|Transformación LIFO|Una pila local modela exactamente el ordenamiento de una llamada recursiva.|
|`demo_paren_rpn.cpp`|Aceptación o rechazo de balanceo y conversión de infijo a RPN.|Dos pilas para precedencia|El análisis y validación simultánea es trivial al separar variables de contexto y sintaxis.|
|`demo_nqueens.cpp`|Contador de soluciones y la gran cifra de `checks`.|Backtracking con `Stack`|La poda (rechazar rápido) ahorra millones de permutaciones inválidas.|
|`demo_maze.cpp`|Lista en orden de coordenadas del camino.|Backtracking con marcado espacial|El historial de la ruta reside enteramente en el contenido vivo de la pila en un instante `t`.|
|`demo_bank.cpp`|Disminución secuencial de los tiempos de clientes en listas.|Iteración discreta de `Queue`|Modela paso del tiempo extrayendo/insertando elementos concurrentemente en múltiples colas.|
|`demo_capitulo4_panorama.cpp`|Integración general de componentes y simulaciones diversas.|Composición de ADTs|Estructuras base lineales pueden escalarse a una infinidad de dominios algorítmicos.|

1. Al extraer los datos impresos, en un `Stack` se ven al revés del orden de ingreso y en el `Queue` idénticos a como entraron.
2. Ambas variantes imprimen exactamente el mismo string hexadecimal o binario al darles el mismo número original.
3. Si los paréntesis no están balanceados, la estructura falla; el correcto balanceo dicta cuándo extraer operadores para crear el ordenamiento final RPN.
4. `solutions` cuenta los tableros que cumplieron el tamaño meta `n`. `checks` cuenta cada verificación hecha contra `conflictsWithAny`; no miden lo mismo porque incontables `checks` resultan en bloqueos tempranos y nunca generan `solutions`.
5. Muestra la traza final exacta en coordenadas desde el inicio, sin incluir los desvíos sin salida, pues esos cayeron en `BACKTRACKED` y salieron de la pila.
6. Cada elemento es el tiempo de servicio (`c.time`) restante para cada cliente actualmente en espera en esa ventanilla específica.
7. Comprueba la adaptabilidad y reutilización del código base, mostrando bajo la misma abstracción lineal (`Stack` y `Queue`) aplicaciones espaciales (laberinto), numéricas (RPN) y estocásticas (banco).

#### Bloque 3 (respuestas)

1. Valida el estado base `empty()`, las adiciones con `push()`, y que las extracciones con `top()` y `pop()` sigan estrictamente LIFO.
2. Valida inicio vacío `empty()`, los ingresos con `enqueue()`, y que `front()` y `dequeue()` operen exactamente en política FIFO.
3. Valida la conversión del valor 12345 a base 8 comparando si ambas funciones, la iterativa y la recursiva, coinciden con la cadena "30071".
4. Verifica combinaciones válidas anidadas de forma simple y compleja (`expr2`), y rechaza correctamente un error de anidamiento de corchetes cruzados `([)]` devolviendo `false`.
5. Comprueba la habilidad de aplicar precedencia compleja, factorial (!), paréntesis, conversiones de cadena completa a RPN (formato postfijo) y la asertividad numérica del valor de salida evaluado como ~2012.0.
6. Valida que al buscar el tablero de N=4 encuentre la cantidad conocida de soluciones completas (2) y que haya registrado en memoria 2 vectores de tableros.
7. En un laberinto predeterminado en un array de strings de 5x5, asegura que la coordenada encontrada en `path.front()` sea (1,1) y que termine eficazmente en `path.back()` en (3,3).
8. Verifica que la función devuelva el índice numérico `2` para ventanillas vacías al comparar arreglos con diferente longitud de cola asignada (2 clientes, 1 cliente, 0 clientes).
9. La prueba interna aborda casos borde (*edge cases*) como expresiones mal formadas en donde faltan operandos, factorial a números no enteros, excepciones de desbordamiento, y NQueens con N extremadamente grande y `collectPlacements=false`.
10. Las pruebas unitarias confirman datos específicos. Definir análisis de algoritmos como invariantes y estado permite dar pruebas formales de que el algoritmo soporta la estructura por más variaciones que entren.
11. Un `bestWindow` que devuelva siempre el primer índice vacío que encuentre sin explorar si otra ventanilla también está vacía pasaría la prueba pública con éxito de "asignación razonable", pero destruiría un análisis estadístico uniforme del banco.

#### Bloque 4 (respuestas y experimentos 1-2)

1. En base iterativa, el residuo es cada dígito del cálculo que forma la cadena invertida; el cociente achica la cifra restando la escala de magnitud procesada. La pila sirve como "espejo" temporal reordenando todos los residuos extraídos de derecha a izquierda.
2. Se apilan primero porque la operación `n % base` descompone al número entregando su dígito menos significativo, pero al escribirlo, la notación posicional humana demanda poner primero el dígito de más peso (el último extraído).
3. A diferencia de delegar estado e inversión en el Frame de la Pila de Llamadas (*call stack*), en la iterativa se crea un objeto en el heap manual. Esto es visible, debugeable iterativamente, y está protegido contra los "Stack Overflow" cuando los árboles de procesos son muy grandes.
4. `divideParentheses` busca dividir el problema hallando un pivote intermedio donde todo lo que esté a la izquierda y a la derecha esté inherentemente balanceado localmente.
5. Ignora la semántica del símbolo en sí. Solo suma variables (`crc`), de manera que no logra constatar si una llave encaja conceptualmente con un paréntesis curvo en cierres asíncronos como `([)]`.
6. En iterativo, cualquier tope actual tiene la memoria obligatoria del nivel de anidamiento más inmediato; rechaza inmediatamente cualquier cierre sin sincronía.
7. Iterativo resulta sumamente natural y fácil de mantener en paréntesis. Recursivo se siente inherentemente expresivo, excelente para la división fractal de un problema como conversión de base.

##### Experimento 1
|Número|Base|Salida recursiva|Salida iterativa|¿Coinciden?|Comentario|
|------|------|------|------|------|------|
|255|16|FF|FF|Sí|Caso estándar hex.|
|10|2|1010|1010|Sí|Conversión binaria.|
|123|8|173|173|Sí|Conversión a octal.|
|0|10|0|0|Sí|Lógica de condición de inicio cero.|
|1024|16|400|400|Sí|Funciona correctamente con magnitudes elevadas.|

##### Experimento 2
|Expresión|Resultado recursivo|Resultado iterativo|Coinciden|Explicación del caso|
|------|------|------|------|------|
|`""`|`true`|`true`|Sí|Caso base de borde (vacío).|
|`"xyz"`|`true`|`true`|Sí|Expresión sin ningún signo especial.|
|`"((()))"`|`true`|`true`|Sí|Validar profundidad de curvas asíncronas.|
|`"(()"`|`false`|`false`|Sí|Desbalance estructural clásico.|
|`"([)]"`|Falla/`true` erróneo|`false`|No|El recursivo con un solo contador no valida el cruce; iterativo con stack captura el error.|
|`"{[()]}"`|`true`|`true`|No|Soporte múltiple de símbolos integrados en el switch iterativo.|
|`"(((((((((())))))))))"`|`true`|`true`|Sí|Estrés de profundidad a gran escala.|
|`"())(()"`|`false`|`false`|Sí|Falla provocada por empuje temprano incorrecto (negativo intermedio).|

#### Bloque 5 (respuestas y experimento 3)

1. Guarda el valor final `double value` proveniente del único elemento sobreviviente y su traza postfija en `std::string rpn`.
2. Se eliminan para aislar de ruido blanco la lógica de evaluación en el parser; permite obviar caracteres de padding durante los índices iterados de cadena.
3. Lo distingue analizando a su vecino anterior: si es el comienzo del índice `0` o tiene a su izquierda otro operador aritmético/apertura `(`, es innegablemente un signo matemático de prefijo, no una resta.
4. Trata a `!` extrayendo 1 elemento de la pila. Para ser íntegro, requiere operando exacto, y usa una verificación `std::fabs` para obligar el tipo de dato subyacente redondeado antes de procesar un bucle entero.
5. Cada vez que se alcanza un caso `>` en la jerarquía, el operador desapilado se agrega permanentemente al historial RPN mientras simultáneamente reduce en la pila de datos numérica la ecuación actual.
6. Si empujar operador inactivo a la pila temporalmente (`<`), resolver paréntesis internamente (`=`), o expulsar para evaluación por tener un peso aritmético indisputable o precedente (`>`).
7. Una expresión mal formada podría simular retornos engañosos. Un control estricto protege fallos de memoria u operaciones asimétricas al detectar pilas incongruentes lanzando `std::runtime_error`.
8. Ejecuta parsing, chequeo de sintaxis y evaluación temporal con una sola pasada asintótica, evitando ciclos múltiples por separado.

##### Experimento 3
|Expresión|RPN esperada|RPN obtenida|Valor esperado / Error|Valor obtenido / Error|Explicación breve|
|------|------|------|------|------|------|
|`2+3*4`|`2 3 4 * +`|`2 3 4 * +`|`14`|`14`|Muestra correcta precedencia de `*` antes que `+`.|
|`10/2-1`|`10 2 / 1 -`|`10 2 / 1 -`|`4`|`4`|Resolución directa lineal por peso de izquierda a derecha.|
|`(2+3)*4`|`2 3 + 4 *`|`2 3 + 4 *`|`20`|`20`|Demuestra que paréntesis rompe la precedencia usual, forzando extraer el `+` primero.|
|`((2+3)*2)^2`|`2 3 + 2 * 2 ^`|`2 3 + 2 * 2 ^`|`100`|`100`|Expresión compleja anidada comprobando orden superior.|
|`-5+3`|`-5 3 +`|`-5 3 +`|`-2`|`-2`|Signo menos correctamente consumido en parseo sin afectar binarios.|
|`2+*3`|N/A|(Lanza excepción)|Error esperado|`runtime_error`|Error semántico de jerarquía interceptado a tiempo.|

#### Bloque 6 (respuestas y experimentos 4-5)

1. En `Queen` dos reinas interfieren si ocupan una celda en común ya sea en filas/columnas, o si la diagonal absoluta `abs(x1 - x2) == abs(y1 - y2)` intersecta.
2. La pila `solution` funge como una instantánea del tablero real con `k` reinas posicionadas legalmente, sirviendo de base al ensayo actual.
3. Avanzar significa encontrar hueco y pasar al nivel inferior del árbol espacial (`q.x+1`). Retroceder interviene si no hay espacio válido, forzando un retorno (pop) a desandar caminos muertos alterando a la reina anterior.
4. Porque mide la magnitud bruta de la complejidad. Visualiza cuántos nudos se barrieron, permitiendo cotejar cuan agresiva o laxa es la técnica de poda para ahorrar miles de cómputos estériles.
5. Salva inmensas cantidades de memoria. Impide anexar listas a memoria física devolviendo meramente variables de estado o conteos macro (`solutions`).
6. En `Maze`: `AVAILABLE` señala caminos libres de explorar, `ROUTE` nodos actualmente en la pila de trayectos, `BACKTRACKED` marca cruces probados y bloqueados perpetuamente, y `WALL` los bloques estáticos del laberinto intransitables.
7. Previenen bucles muertos. Saben por qué lado se incursiona a la coordenada (`incoming`) y cuál fue la última vía ensayada (`outgoing`) de forma sistémica cardinal.
8. Al etiquetar físicamente los nodos se implementa un escaneo DFS implícito puro. Impide la "caída infinita" de iterar ida y vuelta.
9. Son equivalentes. Modifican un estado propuesto, y mediante colisiones cancelan la decisión más alta extraída con un pop de `Stack`. N-Reinas codifica posiciones (filas e interferencia) mientras Laberinto codifica coordenadas fijas en una vecindad (matriz ortogonal 2D).

##### Experimento 4
|`n`|Número de soluciones|Número de `checks`|Crecimiento|Comentario|
|------|------|------|------|------|
|4|2|84|Medio|Base clásica, resolución muy superficial.|
|5|10|405|Medio-Rápido|Aumento exponencial en la huella de búsqueda.|
|6|4|2016|Acelerado|Menor porcentaje de soluciones.|
|8|92|46572|Masivo|Crecimiento no trivial, requiere poda masiva profunda en cada línea extraída.|

1. Aumenta de manera factorial/exponencial, demostrando que el abanico espacial exploratorio crece a cada nuevo estrato.
2. `checks` es un medidor directo del rechazo iterativo, un recuento de bloqueos o pasos al margen que se dieron en las sombras y jamás tocaron forma o matriz. Soluciones es el simple número residual absoluto.
3. Reside incrustada lógicamente en el código al rechazar empujar (`push`) toda una rama a la pila de exploración si colisiona tan solo un renglón `conflictsWithAny` se dispara verdadero, eliminando la permutación en esa rama sin bajar el `x` extra.

##### Experimento 5
|Tipo Laberinto|Grilla usada|Inicio/Destino|Longitud camino|Evidencia retroceso|Interpretación|
|------|------|------|------|------|------|
|Directo y Claro|5x5 lineal ("Tubo")|(1,1) a (1,3)|3|Ninguna|Encontró ruta en un paso perfecto avanzando.|
|Sin Salida Fija|5x5 cajón (`#` rodeando final)|(1,1) a (3,3)|0|Máxima|Explora a fondo y al final se retracta hasta inicio y la pila queda estéril.|
|Zigzag forzado (Ramificado falso)|5x5 con rama muerta central lateral|(1,1) a (4,4)|7|Media-alta|Avanzó por hueco falso, quedó ciego en pared y deshizo varios puntos con `pop` hasta el cruce de vías para tomar el atajo alternativo real.|

#### Bloque 7 (respuestas y experimento 6)

1. En la matriz, `windows[i]` asila de forma independiente al bloque de gente asignada a una taquilla individual, guardando instancias personalizadas de su retraso `Customer.time`.
2. Prioriza `size()` minúsculo. Ante empate asume al primero encontrado de izquierda a derecha (0, 1, 2, ...), cargando un poco sesgadamente a las primeras cajas.
3. Instala una inicialización base controlada en `std::mt19937` permitiendo el principio determinista. Toda distribución pseudorandom será idéntica siempre, un rasgo útil en auditoría.
4. La llegada de eventos antecede a la terminación matemática de tiempo total de servicio, y la diferencia visible denota siempre un porcentaje de colas residuales por procesar en la banda bancaria al tocar campana final de corte (ServTime).
5. Archiva la constancia inmutable paso a paso. Facilita crear reconstrucciones o trazar gráficas de saturación visual instantánea.
6. La moral y el contexto determinan el proceso como un derecho secuencial, FIFO respeta la fila indiana.
7. El modelo puro carece de deserción, frustración, impaciencia humana, turnos prioritarios con VIP o un modelo de Poisson orgánico para las tasas de inmersión probabilísticas.
8. Una fila monolítica única (política de "serpiente") empujando a ventanillas sueltas, balancearía las atenciones anulando variaciones donde una fila se detiene por 1 cliente de caso duro, pero demandaría distinto código.

##### Experimento 6

|`nWin`|`servTime`|`seed`|`totalArrivals`|`totalServed`|Tamaño final de cada cola|Observación|
|------|------|------|------|------|------|------|
|3|1000|2026|746|56|`[230, 230, 230]`|Fuerte congestión. Llegan clientes mucho más rápido (aprox. 75% de probabilidad por instante) de lo que las ventanillas logran atender.|
|3|1000|2026|746|56|`[230, 230, 230]`|Repetir `seed` produce una réplica exacta; no hay ninguna variación en llegadas ni atención.|
|3|1000|9999|753|64|`[230, 230, 229]`|Varianza por la nueva semilla estocástica; llegan más clientes y se atienden más, pero la carga residual sigue perfectamente balanceada.|
|1|1000|2026|518|21|`[497]`|Cuello de botella colosal extremo. Una sola ventanilla no soporta la carga y retiene a casi el 96% de los clientes en espera.|

1. La diferencia entre la rápida tasa de llegadas frente al lento tiempo de servicio configurado internamente (`service(1, 98)`). Las ventanillas no tienen el "ancho de banda" suficiente para procesar a la gente al mismo ritmo que entra.
2. Absolutamente nada. Se clona el escenario paso a paso de forma atómica. Las métricas de clientes atendidos (`56`) y en cola (`746` llegadas) son idénticas, lo cual es vital para auditar algoritmos probabilísticos.
3. Entra en juego la desviación y varianza estocástica. El número total de llegadas osciló de `746` a `753` y las atenciones de `56` a `64`; sin embargo, la distribución estructural del banco se mantuvo coherente y balanceada.
4. El arreglo resultante `[230, 230, 229]` con la semilla 9999. Es la prueba de que la política impide por completo que una ventanilla quede vacía o subutilizada mientras otra colapsa. El balanceo de carga es casi perfecto y equitativo.

#### Bloque 8 (respuesta final)

Cuando se usa ADTs básicos, se está preocupado únicamente por su implementación mecánica de inserciones o consultas limitadas a los principios absolutos `LIFO` y `FIFO`. Sin embargo, al transicionar a utilizarlas como mecanismos sistémicos de control, se pasa de usar memoria a moldear el *tiempo e historial lógico* del problema propuesto; una lista estática ahora orquesta lógicamente qué rama del infinito algoritmo sigue viva.

Al aplicar una `Stack`, se abandona los bloqueos letales asociados al call stack local con una recursión implícita limitada, dando paso a una matriz explícita que moldea y recicla soluciones en tiempo real para optimizar la memoria. En el evaluador de expresiones se observa esto cuando un simple contenedor estático impone el orden del álgebra humana, emparejando la gramática a través de `RPN`. Esta misma jerarquía estructural se sublima en implementaciones profundas de backtracking analizadas, como el simulador del laberinto y el ajedrez (N-Reinas), en los que cada paso de `pop()` y `push()` actúa como la herramienta de navegación en un mapa o árbol de caminos, resucitando decisiones invalidadas prestando una consciencia virtual con técnicas secuenciales convencionales.

Por su parte, cuando se cambia la perspectiva usando colas (`Queue`), se construye puentes con la percepción temporal dictaminada por las colisiones; se visualiza perfectamente al recrear flujos reales asincrónicos, como las simulaciones estadísticas de un banco, integrando de paso comprobaciones formales de correctitud y simetría experimental controlando los procesos por el sistema generador `seed`. En resumen, en la conversión de bases el apilamiento moldea posiciones inversas simples, en validadores como los paréntesis se usa de memoria volátil simétrica, pero en resoluciones topológicas completas (Laberintos/Ajedrez) la pila representa toda la *arquitectura de vida ramificada* y en simulación bancaria la cola muestra la constante irreversible equitativa y finita.

#### Autoevaluación breve
- **Qué puedo defender con seguridad:** La lógica universal de usar una pila (`Stack`) para deshacer y bifurcar procesos a través del descarte, además de las leyes de precedencia usadas en `RPN`.
- **Qué todavía confundo:** Visualizar mentalmente la matriz de `Maze` sin salida visible sin trazar a papel sus múltiples bifurcaciones encadenadas falsas.
- **Qué experimento me dio mejor evidencia:** La variación del estrés factorial provocado en las pruebas dinámicas del conteo bruto del Experimento 4 (N-Reinas) con saltos en el número de operaciones comparadas frente a las de soluciones reales con tan solo alterar la escala por 2 tableros extra.
- **Qué evidencia usaría en una sustentación:** Recurrir a la tabla visual del `demo_bank.cpp`, probando que una `Queue` asienta de forma escalable datos temporales sin fallas de memoria si se sigue el esquema FIFO implementado internamente.