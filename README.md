# SpreadsheetDS — Hoja de cálculo con matrices dispersas en C++17

## Compilación

```bash
mkdir build && cd build && cmake .. && make
```

Alternativa sin CMake:
```bash
g++ -std=c++17 -Iinclude src/SparseMatrix.cpp src/Operations.cpp src/main.cpp -o spreadsheet
g++ -std=c++17 -Iinclude src/SparseMatrix.cpp src/Operations.cpp tests/test_matrix.cpp -o test_matrix
```

## Justificación de la estructura de datos

Lista enlazada cruzada: O(k) memoria donde k = celdas ocupadas,
frente a O(N*M) de un arreglo denso. Con N=M=1000 y k=1000 celdas:
la lista usa 1000x menos memoria que el arreglo.

## Complejidad de operaciones clave

| Operación          | Complejidad |
|--------------------|-------------|
| Insertar celda     | O(k_f+k_c)  |
| Consultar celda    | O(k_f)      |
| Modificar celda    | O(k_f)      |
| Eliminar celda     | O(k_f+k_c)  |
| Eliminar fila      | O(k_f*k_c)  |
| Suma rango         | O(k)        |
| Promedio / Max/Min | O(k)        |
