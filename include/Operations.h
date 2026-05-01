// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : Operations.h
//  CARPETA : include/
//  AUTOR   : Miembro 2
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Declara todas las funciones de agregación y el evaluador de
//    fórmulas. Son funciones libres (no métodos de clase) que
//    operan sobre una SparseMatrix recibida por referencia const.
//
//  QUÉ CONTIENE:
//    · sumRow / sumCol             — Op. 8:  suma de fila o columna
//    · sumRange(r1,c1,r2,c2)      — Op. 9:  suma de rango rectangular
//    · avgRow / avgCol / avgRange  — Op. 10: promedio (nullopt si no
//                                            hay valores numéricos)
//    · maxRange / minRange         — Op. 11: máximo y mínimo en rango
//    · evaluateFormula(m, "=A1+B2")— Op. 12: evalúa expresión aritmética
//                                            con referencias a celdas
//
//  IMPLEMENTACIÓN:
//    Ver src/Operations.cpp
//
//  NOTAS:
//    · Las funciones ignoran celdas con texto — solo procesan doubles.
//    · evaluateFormula retorna nullopt si la expresión es inválida
//      o si hay división por cero.
// ═══════════════════════════════════════════════════════════════════
#pragma once
#include "SparseMatrix.h"
#include <optional>
#include <string>

// ─────────────────────────────────────────────
//  Operations — funciones de agregación y
//  soporte de fórmulas sobre SparseMatrix.
//  Son funciones libres; no tienen estado.
// ─────────────────────────────────────────────

// ── 8. Suma ──────────────────────────────────
double sumRow(const SparseMatrix &m, int row);
double sumCol(const SparseMatrix &m, int col);
double sumRange(const SparseMatrix &m, int r1, int c1, int r2, int c2);

// ── 10. Promedio ─────────────────────────────
// nullopt si no hay valores numéricos
std::optional<double> avgRow(const SparseMatrix &m, int row);
std::optional<double> avgCol(const SparseMatrix &m, int col);
std::optional<double> avgRange(const SparseMatrix &m, int r1, int c1, int r2, int c2);

// ── 11. Máximo y mínimo ──────────────────────
// nullopt si el rango no tiene valores numéricos
std::optional<double> maxRange(const SparseMatrix &m, int r1, int c1, int r2, int c2);
std::optional<double> minRange(const SparseMatrix &m, int r1, int c1, int r2, int c2);

// ── 12. Evaluador de fórmulas ────────────────
// Evalúa expresión que empieza con "="
// Ej: "=A1+B2", "=A1*2", "=C4-B3/2"
// nullopt si la expresión es inválida o tiene error
std::optional<double> evaluateFormula(const SparseMatrix &m,
                                      const std::string &formula);
