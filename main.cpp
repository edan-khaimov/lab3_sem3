#include <iostream>
#include <string>

#include <unordered_map>
#include "headers/Histogram.h"
#include "headers/MostFrequentSubsequences.h"
#include <QApplication>
#include "UI/headers/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

