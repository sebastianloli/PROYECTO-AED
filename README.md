# SpreadsheetDS — Hoja de cálculo con matrices dispersas en C++17 + Qt6

## Integrantes
- Sebastian Loli Gonzalez
- Fernando Aguirre Milla
- Gabriel Vaccaro Ortiz

## Requisitos
 
- g++ con soporte C++17 (GCC 11 o superior recomendado)
- CMake >= 3.16
- Qt6 Widgets (instalado vía MSYS2 UCRT64)

## Instalación de dependencias (Windows con MSYS2)
 
Abrir la terminal **MSYS2 UCRT64** y ejecutar:
 
```bash
pacman -S mingw-w64-ucrt-x86_64-toolchain \
          mingw-w64-ucrt-x86_64-cmake \
          mingw-w64-ucrt-x86_64-ninja \
          mingw-w64-ucrt-x86_64-qt6-base
```
 
## Compilación
 
```bash
mkdir build && cd build
 
export CXX=/ucrt64/bin/g++.exe
export CC=/ucrt64/bin/gcc.exe
export PATH=/ucrt64/bin:$PATH
 
cmake .. -G "Ninja"
ninja
```
 
## Ejecución
 
```bash
./spreadsheet.exe    # aplicación principal con GUI Qt6
./test_matrix.exe    # suite de 12 tests de casos borde
```
 
## Uso de la aplicación
 
- **Editar una celda**: hacer clic sobre la celda y escribir, o usar la barra de fórmulas arriba
- **Fórmulas aritméticas**: escribir `=A1+B2`, `=A1*3`, `=B1-A1`, `=C1/2`
- **Eliminar fila / columna / rango**: menú Editar
- **Suma, promedio, máximo, mínimo**: menú Operaciones → ingresar rango (ej: A1:C4)
- Celdas numéricas se resaltan en azul con texto blanco
- Celdas de texto se resaltan en ámbar con texto blanco
---
 
## Justificación de la estructura de datos elegida
 
### Contexto del problema
 
Una hoja de cálculo puede tener miles de filas y columnas, pero en la práctica solo
una fracción pequeña de las celdas contiene datos en un momento dado. Por ejemplo,
una hoja de 1000 × 1000 tiene un millón de posiciones posibles, pero un usuario
típico llena apenas unos cientos de ellas.
 
Representar esto con un arreglo bidimensional estático `double grid[1000][1000]`
implica reservar en memoria las 1 000 000 posiciones siempre, sin importar cuántas
estén realmente ocupadas. Eso equivale a 8 MB fijos (con doubles de 8 bytes) que
no se liberan aunque la hoja esté casi vacía. Además, operaciones como insertar o
eliminar en un arreglo denso requieren desplazar elementos o recorrer toda la
estructura, lo cual es ineficiente cuando la densidad de datos es baja.
 
### Estructura elegida: lista enlazada cruzada (cross-linked list)
 
Se optó por una **lista enlazada cruzada** donde cada celda con contenido es un
nodo independiente que contiene:
 
- `row` y `col`: coordenadas de la celda
- `value`: el valor almacenado (double o string, usando std::variant de C++17)
- `nextInRow`: puntero al siguiente nodo ocupado en la misma fila
- `nextInCol`: puntero al siguiente nodo ocupado en la misma columna
Adicionalmente se mantienen listas de cabeceras (`RowHeader` y `ColHeader`) para
cada fila y columna activa, permitiendo acceder al primer nodo de cualquier fila o
columna en tiempo constante.
 
Las celdas vacías no generan ningún nodo en memoria — simplemente no existen en
la estructura. Esto es la ventaja fundamental frente al arreglo denso.
 
### Ventaja cuantitativa de memoria
 
Con una hoja de 1000 × 1000 y 100 celdas ocupadas:
 
- **Arreglo denso**: 1 000 000 × 8 bytes = **8 MB** — fijo siempre
- **Lista enlazada cruzada**:
  - 100 nodos × 64 bytes (tamaño aproximado del struct Node con alineación) = 6 400 bytes
  - Cabeceras de fila y columna activas ≈ 16 000 bytes
  - **Total ≈ 22 400 bytes = 21.87 KB**
La lista enlazada cruzada usa aproximadamente **356 veces menos memoria** que el
arreglo denso en este escenario. A medida que la hoja crece pero los datos siguen
siendo dispersos, esta ventaja se amplía.
 
### Ventaja en operaciones de inserción y eliminación
 
En el arreglo denso, insertar o eliminar una fila implica desplazar O(N·M) elementos.
En la lista enlazada cruzada, insertar un nodo cuesta O(k_f + k_c) donde k_f y k_c
son los nodos existentes en esa fila y columna respectivamente. Dado que k_f << M
y k_c << N en una hoja dispersa, la diferencia es significativa.
 
La eliminación de celda es la operación más crítica: requiere remendar los punteros
en ambas direcciones (fila y columna) antes de liberar la memoria, para evitar
punteros colgantes. Esto está implementado explícitamente en `deleteCell()`,
`deleteRow()` y `deleteCol()` en `SparseMatrix.cpp`.
 
 
## Análisis de complejidad de las operaciones implementadas
 
En la tabla siguiente, las variables tienen el siguiente significado:
 
- `k_f` = número de nodos ocupados en la fila involucrada
- `k_c` = número de nodos ocupados en la columna involucrada
- `k`   = número de nodos dentro del rango o fila/columna procesada
- `t`   = número de tokens en una expresión de fórmula
- `N`   = número total de filas de la hoja (arreglo denso)
- `M`   = número total de columnas de la hoja (arreglo denso)
| Operación               | Lista enlazada cruzada | Arreglo denso | Observación |
|-------------------------|------------------------|---------------|-------------|
| Insertar celda          | O(k_f + k_c)           | O(N·M)        | Enlaza en fila y columna |
| Consultar celda         | O(k_f)                 | O(1)*         | Recorre la fila hasta la columna |
| Modificar celda         | O(k_f)                 | O(1)*         | Igual que consultar, sin crear nodo |
| Eliminar celda          | O(k_f + k_c)           | O(1)*         | Remenda punteros en ambas direcciones |
| Eliminar fila entera    | O(k_f · k_c)           | O(M)          | Por cada nodo de la fila, remenda su columna |
| Eliminar columna entera | O(k_c · k_f)           | O(N)          | Por cada nodo de la columna, remenda su fila |
| Eliminar rango          | O(r · c · k)           | O(r · c)      | Llama deleteCell por cada posición del rango |
| SUMA fila / columna     | O(k)                   | O(N o M)      | Solo recorre nodos existentes |
| SUMA rango              | O(k)                   | O(r · c)      | k = nodos dentro del rango |
| PROMEDIO / MAX / MIN    | O(k)                   | O(N o M)      | Un único recorrido |
| Evaluar fórmula =A1+B2  | O(k · t)               | O(t)          | Cada referencia requiere una consulta O(k_f) |
 
*El arreglo denso accede en O(1) por índice directo, pero paga ese costo con
O(N·M) de memoria siempre reservada, independientemente de cuántas celdas
estén ocupadas.
