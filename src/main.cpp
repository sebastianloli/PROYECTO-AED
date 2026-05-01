// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : main.cpp
//  CARPETA : src/
//  AUTOR   : Miembro 3
//  ───────────────────────────────────────────────────────────────────
//  QUÉ HACE:
//    Punto de entrada de la aplicación. Cuando la GUI esté lista,
//    aquí se inicializa la ventana principal y se arranca el loop
//    de eventos. Por ahora contiene una prueba rápida de consola
//    para verificar que el core funciona correctamente.
//
//  QUÉ CONTIENE ACTUALMENTE:
//    · Prueba de consola con insert, query, modify, sumRow, sumCol,
//      sumRange, avgRow, maxRange, evaluateFormula, deleteCell y
//      deleteRow — cubre los casos principales visualmente.
//
//  QUÉ DEBE HACER CUANDO LA GUI ESTÉ LISTA:
//    1. Crear una instancia de SparseMatrix (sheet)
//    2. Crear una instancia de MainWindow pasándole la sheet
//    3. Llamar a mainWindow.show() y arrancar el loop de la librería
//       Ejemplo con Qt:
//         QApplication app(argc, argv);
//         MainWindow w(sheet);
//         w.show();
//         return app.exec();
//
//  DEPENDENCIAS:
//    · SparseMatrix.h — estructura de datos principal
//    · Operations.h   — funciones de agregación y fórmulas
// ═══════════════════════════════════════════════════════════════════
// main.cpp — Entry point (Miembro 3)
// TODO: reemplazar el bloque de prueba de consola por la
//       inicialización de la ventana Qt/SFML/wxWidgets.

#include "SparseMatrix.h"
#include "Operations.h"
#include <iostream>

int main() {
    SparseMatrix sheet;

    // ── Prueba rápida — borrar cuando GUI esté lista ──
    sheet.insert(0, 0, 10.0);                    // A1 = 10
    sheet.insert(0, 1, 20.0);                    // B1 = 20
    sheet.insert(1, 0, 5.0);                     // A2 = 5
    sheet.insert(1, 1, std::string("texto"));    // B2 = "texto"

    std::cout << "=== Contenido inicial ===\n";
    sheet.printAll();

    // Consultar
    auto v = sheet.query(0, 0);
    if (v) std::cout << "A1 = " << std::get<double>(*v) << "\n";

    // Modificar sin crear nodo
    bool ok = sheet.modify(0, 0, 99.0);
    std::cout << "modify A1 -> 99: " << (ok ? "OK" : "FAIL") << "\n";

    // Agregación
    std::cout << "sumRow(0)    = " << sumRow(sheet, 0) << "\n";   // 119
    std::cout << "sumCol(0)    = " << sumCol(sheet, 0) << "\n";   // 104
    std::cout << "sumRange 0:0-1:1 = "
              << sumRange(sheet, 0, 0, 1, 1) << "\n";             // 124

    auto avg = avgRow(sheet, 0);
    if (avg) std::cout << "avgRow(0) = " << *avg << "\n";

    auto mx = maxRange(sheet, 0, 0, 1, 1);
    if (mx) std::cout << "max(A1:B2) = " << *mx << "\n";

    // Fórmula
    auto f = evaluateFormula(sheet, "=A1+A2");
    if (f) std::cout << "=A1+A2 = " << *f << "\n";

    // Eliminar celda
    sheet.deleteCell(0, 0);
    std::cout << "\n=== Tras eliminar A1 ===\n";
    sheet.printAll();

    // Eliminar fila
    sheet.deleteRow(1);
    std::cout << "\n=== Tras eliminar fila 1 ===\n";
    sheet.printAll();

    // Caso borde: celda inexistente
    sheet.deleteCell(99, 99);   // sin crash
    auto q = sheet.query(99, 99);
    std::cout << "query celda inexistente: "
              << (q ? "tiene valor" : "nullopt (correcto)") << "\n";

    std::cout << "\n=== Fin ===\n";
    return 0;
}
