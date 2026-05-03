// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : MainWindow.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Implementa la ventana principal. Crea todos los widgets, conecta
//    los signals/slots y delega cada operación al core (SparseMatrix
//    y Operations) a través de la GridWidget.
// ═══════════════════════════════════════════════════════════════════
#include "MainWindow.h"

#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QAction>
#include <QString>
#include <QFont>

// ─────────────────────────────────────────────────────────────────
//  Constructor — monta toda la UI
// ─────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("SpreadsheetDS — Hoja de Cálculo");
    resize(900, 600);

    // ── Widget central ────────────────────────────────────────────
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* vlay = new QVBoxLayout(central);
    vlay->setContentsMargins(4, 4, 4, 4);
    vlay->setSpacing(4);

    // ── Barra de fórmulas ─────────────────────────────────────────
    QHBoxLayout* flay = new QHBoxLayout();
    coordLabel_ = new QLabel("A1", this);
    coordLabel_->setFixedWidth(48);
    coordLabel_->setAlignment(Qt::AlignCenter);
    coordLabel_->setStyleSheet(
        "border: 1px solid #cbd5e1; border-radius: 3px;"
        "background: #f1f5f9; font-weight: bold; padding: 2px 6px;");

    QLabel* fxIcon = new QLabel("fx", this);
    fxIcon->setFixedWidth(24);
    fxIcon->setAlignment(Qt::AlignCenter);
    fxIcon->setStyleSheet("color: #64748b; font-style: italic;");

    formulaBar_ = new QLineEdit(this);
    formulaBar_->setPlaceholderText(
        "Escribe un valor, texto o fórmula (ej: =A1+B2)");
    formulaBar_->setStyleSheet(
        "border: 1px solid #cbd5e1; border-radius: 3px; padding: 2px 6px;");

    flay->addWidget(coordLabel_);
    flay->addWidget(fxIcon);
    flay->addWidget(formulaBar_);
    vlay->addLayout(flay);

    // ── Cuadrícula ────────────────────────────────────────────────
    grid_ = new GridWidget(sheet_, 50, 20, this);
    vlay->addWidget(grid_);

    // ── Barra de estado ───────────────────────────────────────────
    statusBar()->showMessage("Listo  |  Celdas vacías no ocupan memoria");

    // ── Menú y conexiones ─────────────────────────────────────────
    setupMenuBar();

    // Barra de fórmulas → confirmar con Enter
    connect(formulaBar_, &QLineEdit::returnPressed,
            this, &MainWindow::onFormulaConfirmed);

    // Selección cambia → actualizar coordLabel_ y formulaBar_
    connect(grid_, &QTableWidget::currentCellChanged,
            this, [this](int, int, int, int) { onSelectionChanged(); });

    // Core cambió → refrescar barra de estado
    connect(grid_, &GridWidget::cellDataChanged,
            this, &MainWindow::onSelectionChanged);
}

// ─────────────────────────────────────────────────────────────────
//  setupMenuBar — construye los menús con todas las operaciones
// ─────────────────────────────────────────────────────────────────
void MainWindow::setupMenuBar() {
    // ── Menú Editar ───────────────────────────────────────────────
    QMenu* editMenu = menuBar()->addMenu("&Editar");

    QAction* actDelRow = editMenu->addAction("Eliminar fila...");
    QAction* actDelCol = editMenu->addAction("Eliminar columna...");
    QAction* actDelRng = editMenu->addAction("Eliminar rango...");

    connect(actDelRow, &QAction::triggered, this, &MainWindow::onDeleteRow);
    connect(actDelCol, &QAction::triggered, this, &MainWindow::onDeleteCol);
    connect(actDelRng, &QAction::triggered, this, &MainWindow::onDeleteRange);

    // ── Menú Operaciones ──────────────────────────────────────────
    QMenu* opMenu = menuBar()->addMenu("&Operaciones");

    QAction* actSumRow = opMenu->addAction("SUMA fila...");
    QAction* actSumCol = opMenu->addAction("SUMA columna...");
    opMenu->addSeparator();
    QAction* actSumRng = opMenu->addAction("SUMA rango...");
    QAction* actAvgRng = opMenu->addAction("PROMEDIO rango...");
    QAction* actMaxRng = opMenu->addAction("MÁXIMO rango...");
    QAction* actMinRng = opMenu->addAction("MÍNIMO rango...");

    connect(actSumRow, &QAction::triggered, this, &MainWindow::onSumRow);
    connect(actSumCol, &QAction::triggered, this, &MainWindow::onSumCol);
    connect(actSumRng, &QAction::triggered, this, &MainWindow::onSumRange);
    connect(actAvgRng, &QAction::triggered, this, &MainWindow::onAvgRange);
    connect(actMaxRng, &QAction::triggered, this, &MainWindow::onMaxRange);
    connect(actMinRng, &QAction::triggered, this, &MainWindow::onMinRange);

    // ── Menú Ayuda ────────────────────────────────────────────────
    QMenu* helpMenu = menuBar()->addMenu("&Ayuda");
    QAction* actAbout = helpMenu->addAction("Acerca de...");
    connect(actAbout, &QAction::triggered, this, [this]() {
        QMessageBox::information(this, "SpreadsheetDS",
            "Hoja de cálculo con matrices dispersas\n"
            "Estructura: lista enlazada cruzada (cross-linked list)\n"
            "Proyecto 1 — AED");
    });
}

// ─────────────────────────────────────────────────────────────────
//  onSelectionChanged — actualiza coordLabel_ y formulaBar_
// ─────────────────────────────────────────────────────────────────
void MainWindow::onSelectionChanged() {
    coordLabel_->setText(grid_->currentCoord());
    formulaBar_->setText(grid_->currentRawValue());
}

// ─────────────────────────────────────────────────────────────────
//  onFormulaConfirmed — el usuario presionó Enter en la barra
// ─────────────────────────────────────────────────────────────────
void MainWindow::onFormulaConfirmed() {
    int row = grid_->currentRow();
    int col = grid_->currentColumn();
    if (row < 0 || col < 0) return;

    QString text = formulaBar_->text().trimmed();

    if (text.isEmpty()) {
        sheet_.deleteCell(row, col);
    } else if (text.startsWith("=")) {
        auto result = evaluateFormula(sheet_, text.toStdString());
        if (result) {
            sheet_.insert(row, col, *result);
            statusBar()->showMessage(
                QString("Fórmula %1 = %2").arg(text).arg(*result));
        } else {
            showError("Fórmula inválida o división por cero: " + text);
            return;
        }
    } else {
        bool ok;
        double d = text.toDouble(&ok);
        if (ok) sheet_.insert(row, col, d);
        else    sheet_.insert(row, col, text.toStdString());
    }

    grid_->refresh();
    grid_->setFocus();
}

// ─────────────────────────────────────────────────────────────────
//  Helpers para pedir rango, fila y columna al usuario
// ─────────────────────────────────────────────────────────────────
bool MainWindow::askRange(int& r1, int& c1, int& r2, int& c2) {
    bool ok;
    QString input = QInputDialog::getText(
        this, "Rango", "Ingresa el rango (ej: A1:C4):", QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return false;

    // Separar por ":"
    QStringList parts = input.toUpper().split(":");
    if (parts.size() != 2) { showError("Formato inválido. Usa A1:C4"); return false; }

    try {
        auto [ra, ca] = SparseMatrix::parseCoord(parts[0].toStdString());
        auto [rb, cb] = SparseMatrix::parseCoord(parts[1].toStdString());
        r1 = ra; c1 = ca; r2 = rb; c2 = cb;
        return true;
    } catch (...) {
        showError("Coordenadas inválidas: " + input);
        return false;
    }
}

bool MainWindow::askRow(int& row) {
    bool ok;
    int r = QInputDialog::getInt(
        this, "Fila", "Número de fila (1 = primera):", 1, 1, 9999, 1, &ok);
    if (!ok) return false;
    row = r - 1;   // convertir a base-0
    return true;
}

bool MainWindow::askCol(int& col) {
    bool ok;
    QString input = QInputDialog::getText(
        this, "Columna", "Letra de columna (ej: A, B, C):",
        QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) return false;
    try {
        // parseCoord espera "A1", "B1", etc.
        auto [r, c] = SparseMatrix::parseCoord(
            input.toUpper().toStdString() + "1");
        col = c;
        return true;
    } catch (...) {
        showError("Columna inválida: " + input);
        return false;
    }
}

// ─────────────────────────────────────────────────────────────────
//  Helpers de UI
// ─────────────────────────────────────────────────────────────────
void MainWindow::showResult(const QString& op, double result) {
    statusBar()->showMessage(
        QString("%1 = %2").arg(op).arg(result));
    QMessageBox::information(this, op,
        QString("Resultado: <b>%1</b>").arg(result));
}

void MainWindow::showError(const QString& msg) {
    statusBar()->showMessage("Error: " + msg);
    QMessageBox::warning(this, "Error", msg);
}

// ─────────────────────────────────────────────────────────────────
//  Slots de Menú Editar
// ─────────────────────────────────────────────────────────────────
void MainWindow::onDeleteRow() {
    int row;
    if (!askRow(row)) return;
    sheet_.deleteRow(row);
    grid_->refresh();
    statusBar()->showMessage(
        QString("Fila %1 eliminada").arg(row + 1));
}

void MainWindow::onDeleteCol() {
    int col;
    if (!askCol(col)) return;
    sheet_.deleteCol(col);
    grid_->refresh();
    statusBar()->showMessage(
        QString("Columna eliminada"));
}

void MainWindow::onDeleteRange() {
    int r1, c1, r2, c2;
    if (!askRange(r1, c1, r2, c2)) return;
    sheet_.deleteRange(r1, c1, r2, c2);
    grid_->refresh();
    statusBar()->showMessage("Rango eliminado");
}

// ─────────────────────────────────────────────────────────────────
//  Slots de Menú Operaciones
// ─────────────────────────────────────────────────────────────────
void MainWindow::onSumRow() {
    int row;
    if (!askRow(row)) return;
    double res = sumRow(sheet_, row);
    showResult(QString("SUMA fila %1").arg(row + 1), res);
}

void MainWindow::onSumCol() {
    int col;
    if (!askCol(col)) return;
    double res = sumCol(sheet_, col);
    showResult(QString("SUMA columna"), res);
}

void MainWindow::onSumRange() {
    int r1, c1, r2, c2;
    if (!askRange(r1, c1, r2, c2)) return;
    double res = sumRange(sheet_, r1, c1, r2, c2);
    showResult("SUMA rango", res);
}

void MainWindow::onAvgRange() {
    int r1, c1, r2, c2;
    if (!askRange(r1, c1, r2, c2)) return;
    auto res = avgRange(sheet_, r1, c1, r2, c2);
    if (res) showResult("PROMEDIO rango", *res);
    else     showError("El rango no contiene valores numéricos.");
}

void MainWindow::onMaxRange() {
    int r1, c1, r2, c2;
    if (!askRange(r1, c1, r2, c2)) return;
    auto res = maxRange(sheet_, r1, c1, r2, c2);
    if (res) showResult("MÁXIMO rango", *res);
    else     showError("El rango no contiene valores numéricos.");
}

void MainWindow::onMinRange() {
    int r1, c1, r2, c2;
    if (!askRange(r1, c1, r2, c2)) return;
    auto res = minRange(sheet_, r1, c1, r2, c2);
    if (res) showResult("MÍNIMO rango", *res);
    else     showError("El rango no contiene valores numéricos.");
}
