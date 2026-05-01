// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : Operations.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 2
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Implementa todas las funciones de agregación y el parser de
//    fórmulas. No toca punteros directamente — usa la API pública
//    de SparseMatrix (getRowHead, getColHead, firstRow, firstCol)
//    para recorrer la estructura.
//
//  QUÉ IMPLEMENTA:
//    · asDouble (helper interno)
//        Extrae un double de un CellValue. Retorna nullopt si el
//        valor es texto — así las sumas ignoran texto automáticamente.
//
//    · sumRow / sumCol / sumRange
//        Recorren los nodos de la fila/columna/rango sumando solo
//        los valores numéricos. Complejidad O(k).
//
//    · avgRow / avgCol / avgRange
//        Igual que sum pero dividen por el conteo de numéricos.
//        Retornan nullopt si no hay ningún numérico (caso borde).
//
//    · maxRange / minRange
//        Un solo recorrido del rango comparando valores numéricos.
//        Retornan nullopt si el rango no tiene numéricos.
//
//    · resolveToken (helper interno)
//        Resuelve un token a double: si empieza con letra es una
//        referencia a celda (usa parseCoord + query); si no, es
//        un número literal (usa stod). Celda vacía o con texto → 0.
//
//    · evaluateFormula
//        Tokeniza la expresión quitando el "=" inicial, separa
//        operandos y operadores, y evalúa de izquierda a derecha.
//        Retorna nullopt si la expresión es inválida o hay
//        división por cero.
// ═══════════════════════════════════════════════════════════════════
#include "Operations.h"
#include <vector>
#include <stdexcept>
#include <cctype>

// ── Helper: extraer double de CellValue ──────
static std::optional<double> asDouble(const CellValue& v) {
    if (std::holds_alternative<double>(v))
        return std::get<double>(v);
    return std::nullopt;
}

// ════════════════════════════════════════════
//  8. Suma fila
// ════════════════════════════════════════════
double sumRow(const SparseMatrix& m, int row) {
    RowHeader* rh = m.getRowHead(row);
    if (!rh) return 0.0;
    double t = 0.0;
    for (Node* n = rh->first; n; n = n->nextInRow) {
        auto d = asDouble(n->value);
        if (d) t += *d;
    }
    return t;
}

// ════════════════════════════════════════════
//  8. Suma columna
// ════════════════════════════════════════════
double sumCol(const SparseMatrix& m, int col) {
    ColHeader* ch = m.getColHead(col);
    if (!ch) return 0.0;
    double t = 0.0;
    for (Node* n = ch->first; n; n = n->nextInCol) {
        auto d = asDouble(n->value);
        if (d) t += *d;
    }
    return t;
}

// ════════════════════════════════════════════
//  9. Suma rango
// ════════════════════════════════════════════
double sumRange(const SparseMatrix& m,
                int r1, int c1, int r2, int c2) {
    if (r1 > r2) std::swap(r1, r2);
    if (c1 > c2) std::swap(c1, c2);
    double t = 0.0;
    for (RowHeader* rh = m.firstRow(); rh; rh = rh->nextRow) {
        if (rh->row < r1 || rh->row > r2) continue;
        for (Node* n = rh->first; n; n = n->nextInRow) {
            if (n->col < c1 || n->col > c2) continue;
            auto d = asDouble(n->value);
            if (d) t += *d;
        }
    }
    return t;
}

// ════════════════════════════════════════════
//  10. Promedio
// ════════════════════════════════════════════
std::optional<double> avgRow(const SparseMatrix& m, int row) {
    RowHeader* rh = m.getRowHead(row);
    if (!rh) return std::nullopt;
    double t = 0.0; int cnt = 0;
    for (Node* n = rh->first; n; n = n->nextInRow) {
        auto d = asDouble(n->value);
        if (d) { t += *d; ++cnt; }
    }
    return cnt ? std::optional<double>(t / cnt) : std::nullopt;
}

std::optional<double> avgCol(const SparseMatrix& m, int col) {
    ColHeader* ch = m.getColHead(col);
    if (!ch) return std::nullopt;
    double t = 0.0; int cnt = 0;
    for (Node* n = ch->first; n; n = n->nextInCol) {
        auto d = asDouble(n->value);
        if (d) { t += *d; ++cnt; }
    }
    return cnt ? std::optional<double>(t / cnt) : std::nullopt;
}

std::optional<double> avgRange(const SparseMatrix& m,
                               int r1, int c1, int r2, int c2) {
    if (r1 > r2) std::swap(r1, r2);
    if (c1 > c2) std::swap(c1, c2);
    double t = 0.0; int cnt = 0;
    for (RowHeader* rh = m.firstRow(); rh; rh = rh->nextRow) {
        if (rh->row < r1 || rh->row > r2) continue;
        for (Node* n = rh->first; n; n = n->nextInRow) {
            if (n->col < c1 || n->col > c2) continue;
            auto d = asDouble(n->value);
            if (d) { t += *d; ++cnt; }
        }
    }
    return cnt ? std::optional<double>(t / cnt) : std::nullopt;
}

// ════════════════════════════════════════════
//  11. Máximo y mínimo de rango
// ════════════════════════════════════════════
std::optional<double> maxRange(const SparseMatrix& m,
                               int r1, int c1, int r2, int c2) {
    if (r1 > r2) std::swap(r1, r2);
    if (c1 > c2) std::swap(c1, c2);
    std::optional<double> res;
    for (RowHeader* rh = m.firstRow(); rh; rh = rh->nextRow) {
        if (rh->row < r1 || rh->row > r2) continue;
        for (Node* n = rh->first; n; n = n->nextInRow) {
            if (n->col < c1 || n->col > c2) continue;
            auto d = asDouble(n->value);
            if (d && (!res || *d > *res)) res = d;
        }
    }
    return res;
}

std::optional<double> minRange(const SparseMatrix& m,
                               int r1, int c1, int r2, int c2) {
    if (r1 > r2) std::swap(r1, r2);
    if (c1 > c2) std::swap(c1, c2);
    std::optional<double> res;
    for (RowHeader* rh = m.firstRow(); rh; rh = rh->nextRow) {
        if (rh->row < r1 || rh->row > r2) continue;
        for (Node* n = rh->first; n; n = n->nextInRow) {
            if (n->col < c1 || n->col > c2) continue;
            auto d = asDouble(n->value);
            if (d && (!res || *d < *res)) res = d;
        }
    }
    return res;
}

// ════════════════════════════════════════════
//  12. Evaluador de fórmulas
//  Soporta: +  -  *  /  entre valores numéricos
//  y referencias a celdas (ej. A1, B3, AA12)
//  Evalúa de izquierda a derecha (sin precedencia
//  de operadores — suficiente para el enunciado).
//
//  Celdas vacías o con texto se tratan como 0.
//  Retorna nullopt si la expresión es inválida.
// ════════════════════════════════════════════

// Resuelve un token (referencia o número literal) a double
static double resolveToken(const SparseMatrix& m,
                           const std::string& tok) {
    if (tok.empty()) throw std::invalid_argument("token vacio");

    if (std::isalpha(tok[0])) {
        // Referencia a celda
        auto [r, c] = SparseMatrix::parseCoord(tok);
        auto val = m.query(r, c);
        if (!val) return 0.0;
        auto d = asDouble(*val);
        return d.value_or(0.0);   // texto → 0
    }
    // Número literal
    return std::stod(tok);
}

std::optional<double> evaluateFormula(const SparseMatrix& m,
                                      const std::string& formula) {
    if (formula.empty() || formula[0] != '=') return std::nullopt;

    std::string expr = formula.substr(1);

    // Tokenizar separando en operandos y operadores
    std::vector<std::string> operands;
    std::vector<char>        operators;
    std::string cur;

    for (char ch : expr) {
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            if (cur.empty()) return std::nullopt;   // operador al inicio
            operands.push_back(cur);
            cur.clear();
            operators.push_back(ch);
        } else if (!std::isspace(ch)) {
            cur += ch;
        }
    }
    if (!cur.empty()) operands.push_back(cur);

    if (operands.empty()) return std::nullopt;
    if (operators.size() != operands.size() - 1) return std::nullopt;

    try {
        double result = resolveToken(m, operands[0]);
        for (int i = 0; i < (int)operators.size(); ++i) {
            double rhs = resolveToken(m, operands[i + 1]);
            switch (operators[i]) {
                case '+': result += rhs; break;
                case '-': result -= rhs; break;
                case '*': result *= rhs; break;
                case '/':
                    if (rhs == 0.0) return std::nullopt;
                    result /= rhs;
                    break;
            }
        }
        return result;
    } catch (...) {
        return std::nullopt;   // token inválido
    }
}
