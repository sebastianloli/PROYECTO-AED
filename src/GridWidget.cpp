// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : GridWidget.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Implementa la cuadrícula visual. Se sincroniza con SparseMatrix
//    en ambas direcciones: lee de sheet_ para pintarse y escribe en
//    sheet_ cuando el usuario edita una celda.
//
//  PUNTOS CLAVE:
//    · updating_ evita el bucle infinito: setItem() dispara
//      itemChanged → onItemChanged → setItem → ...
//    · Solo se crean QTableWidgetItems para celdas con contenido;
//      las vacías quedan como nullptr (sin objeto en memoria).
//    · El color de fondo distingue números (azul claro) de texto
//      (amarillo claro) para cumplir el requisito de "celdas
//      ocupadas resaltadas".
// ═══════════════════════════════════════════════════════════════════
#include "GridWidget.h"
#include <QColor>
#include <QBrush>
#include <QHeaderView>
#include <variant>

// ─────────────────────────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────────────────────────
GridWidget::GridWidget(SparseMatrix &sheet, int rows, int cols,
                       QWidget *parent)
    : QTableWidget(rows, cols, parent),
      sheet_(sheet),
      updating_(false)
{
    setupHeaders();

    // Tamaño de columnas y filas compacto
    horizontalHeader()->setDefaultSectionSize(80);
    verticalHeader()->setDefaultSectionSize(22);

    // Fuente monoespaciada para alinear números
    QFont f("Courier New", 10);
    setFont(f);

    // Conectar signal — cuando el usuario termina de editar
    connect(this, &QTableWidget::itemChanged,
            this, &GridWidget::onItemChanged);
}

// ─────────────────────────────────────────────────────────────────
//  setupHeaders — encabezados A B C ... y 1 2 3 ...
// ─────────────────────────────────────────────────────────────────
void GridWidget::setupHeaders()
{
    // Encabezados de columna: A, B, C, ..., Z, AA, AB, ...
    for (int c = 0; c < columnCount(); ++c)
    {
        setHorizontalHeaderItem(c,
                                new QTableWidgetItem(
                                    QString::fromStdString(
                                        SparseMatrix::coordToString(0, c)
                                            .substr(0, SparseMatrix::coordToString(0, c).size() - 1))));
    }
    // Encabezados de fila: 1, 2, 3, ...
    for (int r = 0; r < rowCount(); ++r)
    {
        setVerticalHeaderItem(r,
                              new QTableWidgetItem(QString::number(r + 1)));
    }
}

// ─────────────────────────────────────────────────────────────────
//  refresh — repinta la tabla desde sheet_
// ─────────────────────────────────────────────────────────────────
void GridWidget::refresh()
{
    updating_ = true;

    // Limpiar todos los items actuales
    clearContents();

    // Recorrer solo los nodos que existen en la lista enlazada
    for (RowHeader *rh = sheet_.firstRow(); rh; rh = rh->nextRow)
    {
        if (rh->row >= rowCount())
            continue;
        for (Node *n = rh->first; n; n = n->nextInRow)
        {
            if (n->col >= columnCount())
                continue;

            QString text;
            bool isNumber = false;

            if (std::holds_alternative<double>(n->value))
            {
                double d = std::get<double>(n->value);
                // Mostrar entero si no tiene decimales
                text = (d == static_cast<int>(d))
                           ? QString::number(static_cast<int>(d))
                           : QString::number(d, 'f', 4);
                isNumber = true;
            }
            else
            {
                text = QString::fromStdString(
                    std::get<std::string>(n->value));
            }

            auto *item = new QTableWidgetItem(text);

            // Alineación: números a la derecha, texto a la izquierda
            item->setTextAlignment(
                isNumber ? Qt::AlignRight | Qt::AlignVCenter
                         : Qt::AlignLeft | Qt::AlignVCenter);

            // Color de fondo y texto — contraste alto para tema oscuro y claro
            if (isNumber)
            {
                item->setBackground(QBrush(QColor(37, 99, 235)));   // azul
                item->setForeground(QBrush(QColor(255, 255, 255))); // texto blanco
            }
            else
            {
                item->setBackground(QBrush(QColor(161, 98, 7)));    // ámbar
                item->setForeground(QBrush(QColor(255, 255, 255))); // texto blanco
            }

            setItem(n->row, n->col, item);
        }
    }

    updating_ = false;
}

// ─────────────────────────────────────────────────────────────────
//  currentCoord — devuelve "B3" de la celda seleccionada
// ─────────────────────────────────────────────────────────────────
QString GridWidget::currentCoord() const
{
    int r = currentRow();
    int c = currentColumn();
    if (r < 0 || c < 0)
        return "";
    return QString::fromStdString(SparseMatrix::coordToString(r, c));
}

// ─────────────────────────────────────────────────────────────────
//  currentRawValue — valor actual para mostrar en barra de fórmulas
// ─────────────────────────────────────────────────────────────────
QString GridWidget::currentRawValue() const
{
    int r = currentRow();
    int c = currentColumn();
    if (r < 0 || c < 0)
        return "";

    auto val = sheet_.query(r, c);
    if (!val)
        return "";

    if (std::holds_alternative<double>(*val))
    {
        double d = std::get<double>(*val);
        return (d == static_cast<int>(d))
                   ? QString::number(static_cast<int>(d))
                   : QString::number(d, 'f', 4);
    }
    return QString::fromStdString(std::get<std::string>(*val));
}

// ─────────────────────────────────────────────────────────────────
//  onItemChanged — el usuario editó una celda
// ─────────────────────────────────────────────────────────────────
void GridWidget::onItemChanged(QTableWidgetItem *item)
{
    if (updating_)
        return; // evitar bucle infinito

    int row = item->row();
    int col = item->column();
    QString text = item->text().trimmed();

    if (text.isEmpty())
    {
        // Celda vacía → eliminar nodo de la estructura
        sheet_.deleteCell(row, col);
        item->setBackground(QBrush(Qt::NoBrush));
        emit cellDataChanged();
        return;
    }

    if (text.startsWith("="))
    {
        // Fórmula aritmética
        auto result = evaluateFormula(sheet_, text.toStdString());
        if (result)
        {
            sheet_.insert(row, col, *result);
        }
        else
        {
            // Fórmula inválida — mostrar #ERR en rojo
            updating_ = true;
            item->setText("#ERR");
            item->setForeground(QBrush(QColor(220, 38, 38)));
            item->setBackground(QBrush(QColor(254, 226, 226)));
            updating_ = false;
            return;
        }
    }
    else
    {
        // Intentar parsear como número
        bool ok;
        double d = text.toDouble(&ok);
        if (ok)
        {
            sheet_.insert(row, col, d);
        }
        else
        {
            sheet_.insert(row, col, text.toStdString());
        }
    }

    // Refrescar para aplicar colores correctos
    refresh();
    emit cellDataChanged();
}