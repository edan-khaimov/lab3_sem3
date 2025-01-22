#include "..//headers/MainWindow.h"
#include "../headers/SubsequenceWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), histogramButton(new QPushButton("Гистограммы", this)),
      subsequenceButton(new QPushButton("Поиск подпоследовательностей", this)) {
    setWindowTitle("Главное окно");
    resize(800, 600);

    // Применяем стиль
    setStyleSheet(R"(
        QMainWindow {
            background-color: #f9f9f9;
        }
        QPushButton {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 10px;
            font-size: 14px; /* Уменьшен шрифт */
        }
        QPushButton:hover {
            background-color: #e6e6e6;
        }
        QPushButton:pressed {
            background-color: #d9d9d9;
        }
    )");

    // Размещение кнопок
    histogramButton->setGeometry(300, 200, 250, 50); // Увеличена ширина
    subsequenceButton->setGeometry(300, 300, 250, 50); // Увеличена ширина

    // Сигналы и слоты
    connect(histogramButton, &QPushButton::clicked, this, &MainWindow::openHistogramWindow);
    connect(subsequenceButton, &QPushButton::clicked, this, &MainWindow::openSubsequenceWindow);
}

MainWindow::~MainWindow() = default;

void MainWindow::openHistogramWindow() {
    // Здесь может быть переход к окну гистограмм
}

void MainWindow::openSubsequenceWindow() {
    histogramButton->hide();
    subsequenceButton->hide();

    auto *window = new SubsequenceWindow(this);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}
