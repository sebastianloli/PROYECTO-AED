// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : MainWindow.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Implementa la ventana principal de la aplicación. Coordina
//    todos los componentes de la GUI y conecta la interfaz con
//    la lógica del core (SparseMatrix + Operations).
//
//  QUÉ DEBE IMPLEMENTAR (TODO):
//    · Constructor — crear y organizar los widgets en la ventana:
//        - Barra de menú: Editar (eliminar fila/col), Operaciones
//        - Barra de fórmulas: QLineEdit donde el usuario escribe
//          el valor o una fórmula tipo "=A1+B2"
//        - GridWidget: la cuadrícula principal (ver GridWidget.cpp)
//        - Barra de estado: muestra resultados de agregaciones
//
//    · onDeleteRow()  — pide número de fila, llama sheet_.deleteRow()
//                       y refresca la cuadrícula
//    · onDeleteCol()  — igual para columnas
//    · onSumRange()   — pide rango (ej: "A1:C4"), llama sumRange(),
//                       muestra resultado en status bar
//    · onAvgRange()   — igual con avgRange()
//    · onMaxRange()   — igual con maxRange()
//    · onMinRange()   — igual con minRange()
//
//  FLUJO DE UNA EDICIÓN (referencia para implementar):
//    1. Usuario hace clic en una celda del GridWidget
//    2. El valor actual aparece en la barra de fórmulas
//    3. Usuario escribe nuevo valor o fórmula
//    4. Al presionar Enter:
//       · Si empieza con "=" → evaluateFormula() → insert resultado
//       · Si es número       → sheet_.insert(row, col, double)
//       · Si es texto        → sheet_.insert(row, col, string)
//    5. GridWidget::refresh() repinta la cuadrícula
//
//  DEPENDENCIAS:
//    · MainWindow.h, SparseMatrix.h, Operations.h, GridWidget.h
// ═══════════════════════════════════════════════════════════════════

#include "MainWindow.h"

// Placeholder — reemplazar con implementación real
// cuando la GUI esté configurada
