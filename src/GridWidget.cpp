// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : GridWidget.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Implementa la cuadrícula visual interactiva de la hoja de
//    cálculo. Es el componente más visible de la aplicación —
//    la tabla de filas y columnas donde el usuario ingresa datos.
//
//  QUÉ DEBE IMPLEMENTAR (TODO):
//    · Constructor
//        Inicializa la tabla con un tamaño base (ej: 20 filas × 10
//        columnas). Conecta el signal itemChanged al slot
//        onItemChanged para detectar ediciones del usuario.
//        Establece encabezados de columna A, B, C... y de fila
//        1, 2, 3...
//
//    · refresh()
//        Limpia la tabla visualmente y la repinta leyendo el
//        estado actual de sheet_. Recorre sheet_.firstRow() →
//        nextRow → first → nextInRow para visitar solo los nodos
//        con contenido. Llama highlightOccupied() al final.
//
//    · highlightOccupied()
//        Cambia el color de fondo de las celdas que tienen contenido
//        para distinguirlas visualmente de las vacías.
//        Sugerencia: fondo azul claro para números, amarillo para texto.
//
//    · onItemChanged(item)
//        Se dispara cuando el usuario edita una celda. Lógica:
//        · Si el texto está vacío → sheet_.deleteCell(row, col)
//        · Si empieza con "="     → evaluateFormula() → insert double
//        · Si es número literal   → insert double
//        · Si es texto            → insert string
//        IMPORTANTE: usar el flag updating_ para evitar que
//        refresh() dentro de este slot vuelva a disparar el signal
//        (bucle infinito).
//
//  DEPENDENCIAS:
//    · GridWidget.h, SparseMatrix.h, Operations.h
// ═══════════════════════════════════════════════════════════════════

#include "GridWidget.h"

// Placeholder — reemplazar con implementación real
