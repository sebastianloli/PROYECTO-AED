// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : MainWindow.h
//  CARPETA : include/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Declara la clase MainWindow, que es la ventana principal de
//    la aplicación. Contiene la instancia de SparseMatrix (sheet_)
//    y coordina todos los widgets de la GUI.
//
//  QUÉ CONTIENE:
//    · Bloque comentado con la declaración Qt completa — lista para
//      descomentar cuando se instale Qt5/Qt6.
//    · Placeholder vacío que permite compilar el proyecto sin GUI
//      mientras M1 y M2 terminan el core.
//
//  CÓMO ACTIVAR LA GUI:
//    1. Instalar Qt: pacman -S mingw-w64-ucrt-x86_64-qt6-base
//    2. Descomentar el bloque Qt en este archivo
//    3. Actualizar CMakeLists.txt con find_package(Qt6 ...) y
//       target_link_libraries(spreadsheet Qt6::Widgets)
//    4. Implementar los métodos en MainWindow.cpp
//
//  IMPLEMENTACIÓN:
//    Ver src/MainWindow.cpp
// ═══════════════════════════════════════════════════════════════════
#pragma once
//
// TODO: descomentar el bloque correspondiente a la librería elegida
// y completar la implementación en MainWindow.cpp
//
// Opciones recomendadas:
//   - Qt5/Qt6  : la más completa, tiene QTableWidget listo para usar
//   - SFML     : más simple, requiere dibujar la grilla manualmente
//   - wxWidgets: similar a Qt pero menos documentación en español

// ── Ejemplo con Qt (descomentar si usan Qt) ──────────────────────────
/*
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "SparseMatrix.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onCellChanged(int row, int col);
    void onDeleteRow();
    void onDeleteCol();
    void onSumRange();
    void onAvgRange();
    void onMaxRange();
    void onMinRange();

private:
    SparseMatrix sheet_;
    QTableWidget* grid_;
    QLineEdit*    formulaBar_;
    QLabel*       statusLabel_;

    void setupUI();
    void refreshGrid();
    std::pair<int,int> parseRangeInput(const QString& input);
};
*/

// ── Placeholder para compilar sin GUI ────────────────────────────────
// Eliminar este bloque cuando se implemente la GUI real
class MainWindow
{
public:
    MainWindow() = default;
    ~MainWindow() = default;
    void show() {}
};
