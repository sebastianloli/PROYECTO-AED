// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : GridWidget.h
//  CARPETA : include/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Declara la clase GridWidget, que es la cuadrícula visual de
//    la hoja de cálculo. Hereda de QTableWidget (Qt) y mantiene
//    una referencia a la SparseMatrix para leer y escribir datos.
//
//  QUÉ CONTIENE:
//    · Bloque comentado con la declaración Qt completa — incluye
//      refresh(), highlightOccupied() y el slot onItemChanged().
//    · Placeholder vacío que permite compilar sin GUI.
//
//  MÉTODOS CLAVE A IMPLEMENTAR:
//    · refresh()            — repinta la tabla desde sheet_
//    · highlightOccupied()  — colorea celdas con contenido
//    · onItemChanged(item)  — detecta edición y actualiza sheet_
//
//  IMPLEMENTACIÓN:
//    Ver src/GridWidget.cpp
// ═══════════════════════════════════════════════════════════════════
#pragma once
//
// Este componente es responsable de:
//  - Mostrar todas las celdas ocupadas resaltadas
//  - Permitir edición directa haciendo clic en una celda
//  - Llamar a sheet_.insert / sheet_.deleteCell según la acción
//  - Refrescar la vista cuando cambian los datos
//
// ── Ejemplo con Qt ────────────────────────────────────────────────────
/*
#include <QTableWidget>
#include "SparseMatrix.h"
#include "Operations.h"

class GridWidget : public QTableWidget {
    Q_OBJECT
public:
    // sheet es la misma instancia que vive en MainWindow
    GridWidget(SparseMatrix& sheet, int rows, int cols,
               QWidget* parent = nullptr);

    // Repinta la tabla leyendo el estado actual de sheet_
    void refresh();

    // Resalta las celdas que tienen contenido
    void highlightOccupied();

signals:
    void cellEdited(int row, int col, const QString& value);

private slots:
    void onItemChanged(QTableWidgetItem* item);

private:
    SparseMatrix& sheet_;
    bool updating_; // evita recursión en onItemChanged
};
*/

// Placeholder
class GridWidget
{
public:
    GridWidget() = default;
    void refresh() {}
};
