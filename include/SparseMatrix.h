#pragma once
#include "Node.h"
#include <optional>
#include <string>

// ─────────────────────────────────────────────
//  SparseMatrix — hoja de cálculo con lista
//  enlazada cruzada (cross-linked list).
//
//  Complejidad:
//    insertar / modificar / eliminar celda : O(k)
//      donde k = nodos en esa fila o columna
//    eliminar fila entera                  : O(k_fila * log)
//    agregar sobre rango                   : O(k)
// ─────────────────────────────────────────────
class SparseMatrix
{
public:
    SparseMatrix();
    ~SparseMatrix();

    // Prohibir copia (gestión manual de punteros)
    SparseMatrix(const SparseMatrix &) = delete;
    SparseMatrix &operator=(const SparseMatrix &) = delete;

    // ── Operaciones básicas de celda ─────────
    // 1. Insertar o actualizar (upsert)
    void insert(int row, int col, CellValue value);

    // 2. Consultar — nullopt si la celda no existe
    std::optional<CellValue> query(int row, int col) const;

    // 3. Modificar sin crear nodo nuevo; retorna false si no existe
    bool modify(int row, int col, CellValue value);

    // 4. Eliminar celda; no hace nada si no existe (sin crash)
    void deleteCell(int row, int col);

    // ── Operaciones sobre filas y columnas ───
    // 5. Eliminar fila completa
    void deleteRow(int row);

    // 6. Eliminar columna completa
    void deleteCol(int col);

    // 7. Eliminar rango rectangular [r1,c1] -> [r2,c2]
    void deleteRange(int r1, int c1, int r2, int c2);

    // ── Conversión de coordenadas ─────────────
    // "B3" -> {row=2, col=1}  (base-0)
    static std::pair<int, int> parseCoord(const std::string &coord);
    // {2,1} -> "B3"
    static std::string coordToString(int row, int col);

    // ── Acceso para Operations (solo lectura) ─
    RowHeader *getRowHead(int row) const;
    ColHeader *getColHead(int col) const;
    RowHeader *firstRow() const { return rowList_; }
    ColHeader *firstCol() const { return colList_; }

    // ── Debug ─────────────────────────────────
    void printAll() const;

private:
    RowHeader *rowList_; // lista de cabeceras de fila activas
    ColHeader *colList_; // lista de cabeceras de columna activas

    RowHeader *getOrCreateRow(int row);
    ColHeader *getOrCreateCol(int col);
    void removeRowHeaderIfEmpty(int row);
    void removeColHeaderIfEmpty(int col);
};
