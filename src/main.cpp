// ═══════════════════════════════════════════════════════════════════
//  ARCHIVO : main.cpp  — Entry point con GUI Qt6
//  AUTOR   : Miembro 3
// ═══════════════════════════════════════════════════════════════════
#include <QApplication>
#include <QFont>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("SpreadsheetDS");
    app.setOrganizationName("AED-UTEC");

    // Fuente global de la app
    QFont font("Segoe UI", 9);
    app.setFont(font);

    MainWindow window;
    window.show();

    return app.exec();
}
