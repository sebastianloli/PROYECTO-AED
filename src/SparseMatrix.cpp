#include "SparseMatrix.h"
#include <iostream>
#include <cctype>
#include <stdexcept>

// ════════════════════════════════════════════
//  Constructor / Destructor
// ════════════════════════════════════════════
SparseMatrix::SparseMatrix()
    : rowList_(nullptr), colList_(nullptr) {}

SparseMatrix::~SparseMatrix()
{
    // Liberar todos los nodos recorriendo las filas
    RowHeader *rh = rowList_;
    while (rh)
    {
        Node *n = rh->first;
        while (n)
        {
            Node *tmp = n->nextInRow;
            delete n;
            n = tmp;
        }
        RowHeader *tmp = rh->nextRow;
        delete rh;
        rh = tmp;
    }
    // Liberar cabeceras de columna (los nodos ya fueron borrados)
    ColHeader *ch = colList_;
    while (ch)
    {
        ColHeader *tmp = ch->nextCol;
        delete ch;
        ch = tmp;
    }
}

// ════════════════════════════════════════════
//  Helpers internos
// ════════════════════════════════════════════

// Obtiene o crea la cabecera de la fila row (lista ordenada)
RowHeader *SparseMatrix::getOrCreateRow(int row)
{
    RowHeader *prev = nullptr;
    RowHeader *cur = rowList_;
    while (cur && cur->row < row)
    {
        prev = cur;
        cur = cur->nextRow;
    }
    if (cur && cur->row == row)
        return cur;

    RowHeader *nh = new RowHeader(row);
    nh->nextRow = cur;
    if (prev)
        prev->nextRow = nh;
    else
        rowList_ = nh;
    return nh;
}

ColHeader *SparseMatrix::getOrCreateCol(int col)
{
    ColHeader *prev = nullptr;
    ColHeader *cur = colList_;
    while (cur && cur->col < col)
    {
        prev = cur;
        cur = cur->nextCol;
    }
    if (cur && cur->col == col)
        return cur;

    ColHeader *nh = new ColHeader(col);
    nh->nextCol = cur;
    if (prev)
        prev->nextCol = nh;
    else
        colList_ = nh;
    return nh;
}

RowHeader *SparseMatrix::getRowHead(int row) const
{
    RowHeader *rh = rowList_;
    while (rh && rh->row != row)
        rh = rh->nextRow;
    return rh;
}

ColHeader *SparseMatrix::getColHead(int col) const
{
    ColHeader *ch = colList_;
    while (ch && ch->col != col)
        ch = ch->nextCol;
    return ch;
}

void SparseMatrix::removeRowHeaderIfEmpty(int row)
{
    RowHeader *prev = nullptr;
    RowHeader *cur = rowList_;
    while (cur && cur->row != row)
    {
        prev = cur;
        cur = cur->nextRow;
    }
    if (!cur || cur->first)
        return; // no existe o aún tiene nodos
    if (prev)
        prev->nextRow = cur->nextRow;
    else
        rowList_ = cur->nextRow;
    delete cur;
}

void SparseMatrix::removeColHeaderIfEmpty(int col)
{
    ColHeader *prev = nullptr;
    ColHeader *cur = colList_;
    while (cur && cur->col != col)
    {
        prev = cur;
        cur = cur->nextCol;
    }
    if (!cur || cur->first)
        return;
    if (prev)
        prev->nextCol = cur->nextCol;
    else
        colList_ = cur->nextCol;
    delete cur;
}

// ════════════════════════════════════════════
//  1. Insertar / actualizar (upsert)
//  Si la celda existe, modifica el valor in-place
//  sin crear un nodo nuevo.
// ════════════════════════════════════════════
void SparseMatrix::insert(int row, int col, CellValue value)
{
    RowHeader *rh = getOrCreateRow(row);

    // Buscar posición en la fila
    Node *prevR = nullptr;
    Node *cur = rh->first;
    while (cur && cur->col < col)
    {
        prevR = cur;
        cur = cur->nextInRow;
    }

    // Si ya existe, solo actualizar (operación 3 implícita)
    if (cur && cur->col == col)
    {
        cur->value = std::move(value);
        return;
    }

    // Crear nodo nuevo
    Node *nn = new Node(row, col, std::move(value));

    // Enlazar en la fila
    nn->nextInRow = cur;
    if (prevR)
        prevR->nextInRow = nn;
    else
        rh->first = nn;

    // Enlazar en la columna (lista ordenada por fila)
    ColHeader *ch = getOrCreateCol(col);
    Node *prevC = nullptr;
    Node *curC = ch->first;
    while (curC && curC->row < row)
    {
        prevC = curC;
        curC = curC->nextInCol;
    }

    nn->nextInCol = curC;
    if (prevC)
        prevC->nextInCol = nn;
    else
        ch->first = nn;
}

// ════════════════════════════════════════════
//  2. Consultar
// ════════════════════════════════════════════
std::optional<CellValue> SparseMatrix::query(int row, int col) const
{
    RowHeader *rh = getRowHead(row);
    if (!rh)
        return std::nullopt;
    Node *n = rh->first;
    while (n && n->col < col)
        n = n->nextInRow;
    if (n && n->col == col)
        return n->value;
    return std::nullopt;
}

// ════════════════════════════════════════════
//  3. Modificar sin crear nodo nuevo
// ════════════════════════════════════════════
bool SparseMatrix::modify(int row, int col, CellValue value)
{
    RowHeader *rh = getRowHead(row);
    if (!rh)
        return false;
    Node *n = rh->first;
    while (n && n->col < col)
        n = n->nextInRow;
    if (!n || n->col != col)
        return false;
    n->value = std::move(value);
    return true;
}

// ════════════════════════════════════════════
//  4. Eliminar celda individual
//     Ajusta punteros en AMBAS direcciones.
//     No hace nada si la celda no existe.
// ════════════════════════════════════════════
void SparseMatrix::deleteCell(int row, int col)
{
    RowHeader *rh = getRowHead(row);
    if (!rh)
        return; // fila vacía — sin crash

    // Remendar enlace en fila
    Node *prevR = nullptr;
    Node *cur = rh->first;
    while (cur && cur->col < col)
    {
        prevR = cur;
        cur = cur->nextInRow;
    }
    if (!cur || cur->col != col)
        return; // celda no existe — sin crash

    if (prevR)
        prevR->nextInRow = cur->nextInRow;
    else
        rh->first = cur->nextInRow;

    // Remendar enlace en columna
    ColHeader *ch = getColHead(col);
    Node *prevC = nullptr;
    Node *curC = ch->first;
    while (curC && curC != cur)
    {
        prevC = curC;
        curC = curC->nextInCol;
    }

    if (prevC)
        prevC->nextInCol = cur->nextInCol;
    else
        ch->first = cur->nextInCol;

    delete cur;

    // Limpiar cabeceras huérfanas
    removeRowHeaderIfEmpty(row);
    removeColHeaderIfEmpty(col);
}

// ════════════════════════════════════════════
//  5. Eliminar fila completa
// ════════════════════════════════════════════
void SparseMatrix::deleteRow(int row)
{
    RowHeader *rh = getRowHead(row);
    if (!rh)
        return; // fila vacía — sin crash

    Node *cur = rh->first;
    while (cur)
    {
        // Remendar columna antes de borrar el nodo
        ColHeader *ch = getColHead(cur->col);
        Node *prevC = nullptr;
        Node *curC = ch->first;
        while (curC && curC != cur)
        {
            prevC = curC;
            curC = curC->nextInCol;
        }
        if (prevC)
            prevC->nextInCol = cur->nextInCol;
        else
            ch->first = cur->nextInCol;
        removeColHeaderIfEmpty(cur->col);

        Node *tmp = cur->nextInRow;
        delete cur;
        cur = tmp;
    }

    // Eliminar la cabecera de fila
    RowHeader *prev = nullptr;
    RowHeader *rh2 = rowList_;
    while (rh2 && rh2->row != row)
    {
        prev = rh2;
        rh2 = rh2->nextRow;
    }
    if (prev)
        prev->nextRow = rh2->nextRow;
    else
        rowList_ = rh2->nextRow;
    delete rh2;
}

// ════════════════════════════════════════════
//  6. Eliminar columna completa
// ════════════════════════════════════════════
void SparseMatrix::deleteCol(int col)
{
    ColHeader *ch = getColHead(col);
    if (!ch)
        return;

    Node *cur = ch->first;
    while (cur)
    {
        // Remendar fila antes de borrar el nodo
        RowHeader *rh = getRowHead(cur->row);
        Node *prevR = nullptr;
        Node *curR = rh->first;
        while (curR && curR != cur)
        {
            prevR = curR;
            curR = curR->nextInRow;
        }
        if (prevR)
            prevR->nextInRow = cur->nextInRow;
        else
            rh->first = cur->nextInRow;
        removeRowHeaderIfEmpty(cur->row);

        Node *tmp = cur->nextInCol;
        delete cur;
        cur = tmp;
    }

    // Eliminar la cabecera de columna
    ColHeader *prev = nullptr;
    ColHeader *ch2 = colList_;
    while (ch2 && ch2->col != col)
    {
        prev = ch2;
        ch2 = ch2->nextCol;
    }
    if (prev)
        prev->nextCol = ch2->nextCol;
    else
        colList_ = ch2->nextCol;
    delete ch2;
}

// ════════════════════════════════════════════
//  7. Eliminar rango rectangular
// ════════════════════════════════════════════
void SparseMatrix::deleteRange(int r1, int c1, int r2, int c2)
{
    if (r1 > r2)
        std::swap(r1, r2);
    if (c1 > c2)
        std::swap(c1, c2);
    for (int r = r1; r <= r2; ++r)
        for (int c = c1; c <= c2; ++c)
            deleteCell(r, c);
}

// ════════════════════════════════════════════
//  Conversión de coordenadas
// ════════════════════════════════════════════
std::pair<int, int> SparseMatrix::parseCoord(const std::string &coord)
{
    int col = 0, i = 0;
    while (i < (int)coord.size() && std::isalpha(coord[i]))
    {
        col = col * 26 + (std::toupper(coord[i]) - 'A' + 1);
        ++i;
    }
    col -= 1; // base-0
    int row = std::stoi(coord.substr(i)) - 1;
    return {row, col};
}

std::string SparseMatrix::coordToString(int row, int col)
{
    std::string s;
    int c = col + 1;
    while (c > 0)
    {
        --c;
        s = char('A' + c % 26) + s;
        c /= 26;
    }
    return s + std::to_string(row + 1);
}

// ════════════════════════════════════════════
//  Debug
// ════════════════════════════════════════════
void SparseMatrix::printAll() const
{
    RowHeader *rh = rowList_;
    while (rh)
    {
        for (Node *n = rh->first; n; n = n->nextInRow)
        {
            std::cout << coordToString(n->row, n->col) << "=";
            std::visit([](auto &&v)
                       { std::cout << v; }, n->value);
            std::cout << "  ";
        }
        std::cout << "\n";
        rh = rh->nextRow;
    }
}
