// test_matrix.cpp — Casos borde del enunciado (Miembro 1 + 2)
// Compilar directamente:
//   g++ -std=c++17 -I../include ../src/SparseMatrix.cpp \
//       ../src/Operations.cpp test_matrix.cpp -o test_matrix && ./test_matrix

#include "SparseMatrix.h"
#include "Operations.h"
#include <cassert>
#include <iostream>

// ── Helpers ──────────────────────────────────
static void pass(const char *name)
{
    std::cout << "[PASS] " << name << "\n";
}

// ════════════════════════════════════════════
//  Casos borde de celda
// ════════════════════════════════════════════
void test_consultar_celda_inexistente()
{
    SparseMatrix m;
    assert(!m.query(5, 5).has_value());
    assert(!m.query(0, 0).has_value());
    pass("consultar_celda_inexistente");
}

void test_eliminar_celda_inexistente()
{
    SparseMatrix m;
    m.deleteCell(5, 5); // no debe crashear
    m.deleteCell(0, 0);
    pass("eliminar_celda_inexistente");
}

void test_modificar_celda_inexistente()
{
    SparseMatrix m;
    assert(m.modify(0, 0, 1.0) == false);
    pass("modificar_celda_inexistente");
}

// ════════════════════════════════════════════
//  Casos borde de fila / columna
// ════════════════════════════════════════════
void test_eliminar_fila_vacia()
{
    SparseMatrix m;
    m.deleteRow(0); // no debe crashear
    m.deleteRow(99);
    pass("eliminar_fila_vacia");
}

void test_eliminar_columna_vacia()
{
    SparseMatrix m;
    m.deleteCol(0);
    m.deleteCol(99);
    pass("eliminar_columna_vacia");
}

// ════════════════════════════════════════════
//  Casos borde de agregación
// ════════════════════════════════════════════
void test_agregacion_sin_numericos()
{
    SparseMatrix m;
    m.insert(0, 0, std::string("texto"));
    m.insert(0, 1, std::string("otro"));

    assert(sumRange(m, 0, 0, 0, 1) == 0.0);
    assert(!avgRange(m, 0, 0, 0, 1).has_value());
    assert(!maxRange(m, 0, 0, 0, 1).has_value());
    assert(!minRange(m, 0, 0, 0, 1).has_value());
    pass("agregacion_sin_numericos");
}

void test_agregacion_rango_vacio()
{
    SparseMatrix m;
    assert(sumRange(m, 0, 0, 5, 5) == 0.0);
    assert(!avgRange(m, 0, 0, 5, 5).has_value());
    pass("agregacion_rango_vacio");
}

// ════════════════════════════════════════════
//  Texto junto con numéricos
// ════════════════════════════════════════════
void test_texto_no_contamina_suma()
{
    SparseMatrix m;
    m.insert(0, 0, std::string("etiqueta"));
    m.insert(0, 1, 42.0);
    m.insert(0, 2, 8.0);

    assert(sumRow(m, 0) == 50.0); // texto ignorado
    pass("texto_no_contamina_suma");
}

// ════════════════════════════════════════════
//  Upsert: insert sobre celda existente
// ════════════════════════════════════════════
void test_upsert_no_duplica_nodo()
{
    SparseMatrix m;
    m.insert(0, 0, 1.0);
    m.insert(0, 0, 2.0); // debe actualizar, no crear segundo nodo
    auto v = m.query(0, 0);
    assert(v.has_value());
    assert(std::get<double>(*v) == 2.0);
    // sumRow debe ser 2, no 3
    assert(sumRow(m, 0) == 2.0);
    pass("upsert_no_duplica_nodo");
}

// ════════════════════════════════════════════
//  Eliminación de rango
// ════════════════════════════════════════════
void test_eliminar_rango()
{
    SparseMatrix m;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            m.insert(r, c, (double)(r * 3 + c));

    m.deleteRange(0, 0, 1, 1); // A1:B2

    assert(!m.query(0, 0).has_value());
    assert(!m.query(0, 1).has_value());
    assert(!m.query(1, 0).has_value());
    assert(!m.query(1, 1).has_value());
    assert(m.query(2, 2).has_value()); // C3 intacta
    pass("eliminar_rango");
}

// ════════════════════════════════════════════
//  Fórmulas
// ════════════════════════════════════════════
void test_formulas()
{
    SparseMatrix m;
    m.insert(0, 0, 3.0); // A1
    m.insert(0, 1, 4.0); // B1

    auto add = evaluateFormula(m, "=A1+B1");
    assert(add && *add == 7.0);

    auto mul = evaluateFormula(m, "=A1*B1");
    assert(mul && *mul == 12.0);

    auto sub = evaluateFormula(m, "=B1-A1");
    assert(sub && *sub == 1.0);

    auto div = evaluateFormula(m, "=B1/A1");
    assert(div && std::abs(*div - (4.0 / 3.0)) < 1e-9);

    // División por cero → nullopt
    auto dz = evaluateFormula(m, "=A1/C1"); // C1 vacía → 0
    assert(!dz.has_value());

    // Expresión inválida
    assert(!evaluateFormula(m, "A1+B1").has_value()); // sin "="
    pass("formulas");
}

// ════════════════════════════════════════════
//  Punteros tras eliminar fila: columnas OK
// ════════════════════════════════════════════
void test_punteros_columna_tras_deleteRow()
{
    SparseMatrix m;
    m.insert(0, 0, 1.0); // A1
    m.insert(1, 0, 2.0); // A2
    m.insert(2, 0, 3.0); // A3

    m.deleteRow(1); // elimina A2

    // Columna A debe tener A1 y A3; A2 no debe aparecer
    assert(m.query(0, 0).has_value());
    assert(!m.query(1, 0).has_value());
    assert(m.query(2, 0).has_value());

    assert(sumCol(m, 0) == 4.0); // 1 + 3
    pass("punteros_columna_tras_deleteRow");
}

// ════════════════════════════════════════════
//  Main
// ════════════════════════════════════════════
int main()
{
    test_consultar_celda_inexistente();
    test_eliminar_celda_inexistente();
    test_modificar_celda_inexistente();
    test_eliminar_fila_vacia();
    test_eliminar_columna_vacia();
    test_agregacion_sin_numericos();
    test_agregacion_rango_vacio();
    test_texto_no_contamina_suma();
    test_upsert_no_duplica_nodo();
    test_eliminar_rango();
    test_formulas();
    test_punteros_columna_tras_deleteRow();

    std::cout << "\nTodos los tests pasaron.\n";
    return 0;
}
