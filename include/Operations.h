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
