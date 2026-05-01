// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : Node.h
//  CARPETA : include/
//  AUTOR   : Miembro 1
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Define las estructuras de datos fundamentales de la hoja de
//    cálculo. Es el archivo más básico del proyecto — todos los
//    demás lo incluyen directa o indirectamente.
//
//  QUÉ CONTIENE:
//    · CellValue   — alias de std::variant<double, std::string>.
//                    Permite que una celda guarde un número o texto
//                    sin usar herencia ni void*.
//    · Node        — representa UNA celda con contenido. Tiene dos
//                    punteros: nextInRow (siguiente en su fila) y
//                    nextInCol (siguiente en su columna). Las celdas
//                    vacías NO generan un Node.
//    · RowHeader   — cabecera de una fila activa. Apunta al primer
//                    Node de esa fila ordenado por columna.
//    · ColHeader   — cabecera de una columna activa. Apunta al primer
//                    Node de esa columna ordenado por fila.
//
//  POR QUÉ IMPORTA:
//    La estructura Node + RowHeader + ColHeader forma la "lista
//    enlazada cruzada". Gracias a los dos punteros por nodo,
//    podemos recorrer la hoja tanto por filas como por columnas
//    en O(k), donde k es el número de celdas ocupadas en esa
//    fila o columna — sin tocar celdas vacías.
// ═══════════════════════════════════════════════════════════════════
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
