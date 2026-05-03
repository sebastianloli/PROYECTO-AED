// main_console.cpp — versión temporal SIN GUI
// Se usa automáticamente si Qt6 no está instalado
#include "SparseMatrix.h"
#include "Operations.h"
#include <iostream>

int main() {
    std::cout << "SpreadsheetDS — modo consola (Qt6 no encontrado)\n";
    std::cout << "Instala Qt6: pacman -S mingw-w64-ucrt-x86_64-qt6-base\n\n";

    SparseMatrix sheet;
    sheet.insert(0, 0, 10.0);
    sheet.insert(0, 1, 20.0);
    sheet.insert(1, 0, 5.0);
    sheet.insert(1, 1, std::string("texto"));
    sheet.printAll();
    std::cout << "sumRow(0) = " << sumRow(sheet, 0) << "\n";
    return 0;
}
