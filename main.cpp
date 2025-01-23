#include <QApplication>
#include <iostream>
#include "UI/headers/MainWindow.h"
#include "headers/Histogram.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

