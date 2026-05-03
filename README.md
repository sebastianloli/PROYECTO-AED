# SpreadsheetDS — Hoja de cálculo con matrices dispersas en C++17 + Qt6

## Integrantes
- Sebastian Loli Gonzalez
- Fernando Aguirre Milla
- Gabriel Vaccaro Ortiz

## Requisitos
- g++ con soporte C++17
- CMake >= 3.16
- Qt6 Widgets

## Instalación de Qt6 (Windows con MSYS2)

Abrir la terminal MSYS2 UCRT64 y ejecutar:
```bash
pacman -S mingw-w64-ucrt-x86_64-qt6-base
```

Verificar que Qt6 quedó disponible:
```bash
qmake6 --version
```

## Compilación

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

En Linux/macOS:
```bash
mkdir build && cd build
cmake ..
make
```

## Ejecución

```bash
./spreadsheet      # aplicación con GUI
./test_matrix      # suite de casos borde (sin GUI)
```

## Uso de la aplicación

- **Editar una celda**: hacer clic y escribir directamente, o usar la barra de fórmulas
- **Fórmulas**: escribir `=A1+B2`, `=A1*3`, `=B1-A1`, `=C1/2`
- **Eliminar fila/columna/rango**: menú Editar
- **Suma, promedio, máximo, mínimo**: menú Operaciones
- Celdas numéricas se resaltan en azul claro
- Celdas de texto se resaltan en amarillo claro

## Justificación de la estructura de datos

Lista enlazada cruzada: O(k) memoria donde k = celdas ocupadas,
frente a O(N*M) de un arreglo denso. Con N=M=1000 y k=1000 celdas:
la lista usa 1000x menos memoria que el arreglo.

## Complejidad de operaciones clave

| Operación          | Complejidad  |
|--------------------|--------------|
| Insertar celda     | O(k_f + k_c) |
| Consultar celda    | O(k_f)       |
| Modificar celda    | O(k_f)       |
| Eliminar celda     | O(k_f + k_c) |
| Eliminar fila      | O(k_f · k_c) |
| Eliminar columna   | O(k_c · k_f) |
| Eliminar rango     | O(r · c · k) |
| Suma fila/columna  | O(k)         |
| Suma rango         | O(k)         |
| Promedio / Max/Min | O(k)         |

k_f = nodos en la fila, k_c = nodos en la columna, k = nodos en el rango
