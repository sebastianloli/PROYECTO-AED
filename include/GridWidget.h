// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : GridWidget.h
//  CARPETA : include/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Declara la cuadrícula visual interactiva. Hereda de QTableWidget
//    y mantiene referencia a SparseMatrix para leer/escribir datos.
//
//  QUÉ CONTIENE:
//    · refresh()           — repinta la tabla desde sheet_
//    · highlightOccupied() — colorea celdas con contenido
//    · currentCoord()      — retorna la celda activa como "B3"
//    · onItemChanged()     — detecta edición y actualiza sheet_
//
//  IMPLEMENTACIÓN:
//    Ver src/GridWidget.cpp
// ═══════════════════════════════════════════════════════════════════
#pragma once

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QString>
#include "SparseMatrix.h"
#include "Operations.h"

class GridWidget : public QTableWidget {
    Q_OBJECT

public:
    // rows y cols definen el tamaño visible de la grilla
    GridWidget(SparseMatrix& sheet, int rows = 30, int cols = 15,
               QWidget* parent = nullptr);

    // Repinta toda la tabla leyendo el estado actual de sheet_
    void refresh();

    // Devuelve la coordenada de la celda seleccionada, ej: "B3"
    QString currentCoord() const;

    // Devuelve el valor raw de la celda seleccionada para la barra de fórmulas
    QString currentRawValue() const;

signals:
    // Emitido cuando el usuario termina de editar una celda
    void cellDataChanged();

private slots:
    void onItemChanged(QTableWidgetItem* item);

private:
    SparseMatrix& sheet_;
    bool updating_;   // evita bucle infinito en onItemChanged

    void highlightOccupied();
    void setupHeaders();
};
