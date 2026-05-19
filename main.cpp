#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    QFont defaultFont("Microsoft YaHei", 9);
    app.setFont(defaultFont);

    MainWindow window;
    window.show();

    return app.exec();
}
