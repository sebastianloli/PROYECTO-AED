// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : MainWindow.h
//  CARPETA : include/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Declara la ventana principal de la aplicación Qt.
//    Contiene la instancia de SparseMatrix y coordina todos los
//    widgets: barra de fórmulas, cuadrícula, menú y barra de estado.
//
//  QUÉ CONTIENE:
//    · sheet_       — la hoja de cálculo (SparseMatrix)
//    · grid_        — cuadrícula visual (GridWidget)
//    · formulaBar_  — campo de texto para editar fórmulas
//    · coordLabel_  — muestra la celda activa (ej: "B3")
//    · Slots para cada operación del menú
//
//  IMPLEMENTACIÓN:
//    Ver src/MainWindow.cpp
// ═══════════════════════════════════════════════════════════════════
#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include "SparseMatrix.h"
#include "Operations.h"
#include "GridWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onFormulaConfirmed();   // Enter en barra de fórmulas
    void onSelectionChanged();   // celda activa cambia → actualiza barra

    // Menú Editar
    void onDeleteRow();
    void onDeleteCol();
    void onDeleteRange();

    // Menú Operaciones
    void onSumRow();
    void onSumCol();
    void onSumRange();
    void onAvgRange();
    void onMaxRange();
    void onMinRange();

private:
    SparseMatrix sheet_;
    GridWidget*  grid_;
    QLineEdit*   formulaBar_;
    QLabel*      coordLabel_;

    void setupMenuBar();
    void showResult(const QString& op, double result);
    void showError(const QString& msg);
    bool askRange(int& r1, int& c1, int& r2, int& c2);
    bool askRow(int& row);
    bool askCol(int& col);
};
