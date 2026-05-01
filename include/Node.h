#pragma once
#include <string>
#include <variant>

// Valor de una celda: número flotante o texto
using CellValue = std::variant<double, std::string>;

// ─────────────────────────────────────────────
//  Nodo de la lista enlazada cruzada.
//  Cada celda con contenido es un nodo.
//  Las celdas vacías NO existen en memoria.
// ─────────────────────────────────────────────
struct Node
{
    int row;         // fila (base-0 internamente)
    int col;         // columna (base-0 internamente)
    CellValue value; // double o std::string

    Node *nextInRow; // siguiente nodo en la misma fila
    Node *nextInCol; // siguiente nodo en la misma columna

    Node(int r, int c, CellValue v)
        : row(r), col(c), value(std::move(v)),
          nextInRow(nullptr), nextInCol(nullptr) {}
};

// ─────────────────────────────────────────────
//  Cabecera de fila
//  rowList_[r].first -> primer nodo de la fila r
// ─────────────────────────────────────────────
struct RowHeader
{
    int row;
    Node *first;        // primer nodo (ordenado por col)
    RowHeader *nextRow; // siguiente cabecera de fila activa

    explicit RowHeader(int r)
        : row(r), first(nullptr), nextRow(nullptr) {}
};

// ─────────────────────────────────────────────
//  Cabecera de columna
//  colList_[c].first -> primer nodo de la col c
// ─────────────────────────────────────────────
struct ColHeader
{
    int col;
    Node *first;        // primer nodo (ordenado por fila)
    ColHeader *nextCol; // siguiente cabecera de columna activa

    explicit ColHeader(int c)
        : col(c), first(nullptr), nextCol(nullptr) {}
};
