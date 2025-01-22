#include "..//headers/SubsequenceWindow.h"
#include "../../headers/MostFrequentSubsequences.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

SubsequenceWindow::SubsequenceWindow(QWidget *parent)
    : QWidget(parent), filePathField(new QLineEdit(this)),
      lminSpinBox(new QSpinBox(this)), lmaxSpinBox(new QSpinBox(this)),
      saveLocationField(new QLineEdit(this)), loadFileButton(new QPushButton("Загрузить файл", this)),
      chooseSaveButton(new QPushButton("Выбрать папку сохранения", this)),
      processButton(new QPushButton("Получить данные", this)),
      statusLabel(new QLabel("", this)) {
    setWindowTitle("Поиск наиболее частых подпоследовательностей");
    resize(800, 600);

    // Установка диапазонов
    lminSpinBox->setRange(1, 1000);
    lmaxSpinBox->setRange(1, 1000);
    lminSpinBox->setValue(1);
    lmaxSpinBox->setValue(10);

    // Убираем кнопки ручного сдвига
    lminSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    lmaxSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // Применение стилей
    setStyleSheet(R"(
        QWidget {
            background-color: #f9f9f9;
        }
        QLineEdit, QSpinBox {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
        }
        QPushButton {
            background-color: #ffffff;
            color: #333333;
            border: 1px solid #cccccc;
            border-radius: 5px;
            padding: 10px;
            font-size: 14px;
        }
        QPushButton:hover {
            background-color: #e6e6e6;
        }
        QPushButton:pressed {
            background-color: #d9d9d9;
        }
        QLabel {
            color: #555555;
            font-size: 14px;
        }
    )");

    auto *mainLayout = new QVBoxLayout(this);

    auto *fileInputLayout = new QHBoxLayout();
    fileInputLayout->addWidget(loadFileButton);
    fileInputLayout->addWidget(filePathField);

    auto *rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(new QLabel("Минимальная длина подстроки:", this));
    rangeLayout->addWidget(lminSpinBox);
    rangeLayout->addWidget(new QLabel("Максимальная длина подстроки:", this));
    rangeLayout->addWidget(lmaxSpinBox);

    auto *saveLayout = new QHBoxLayout();
    saveLayout->addWidget(chooseSaveButton);
    saveLayout->addWidget(saveLocationField);

    mainLayout->addLayout(fileInputLayout);
    mainLayout->addLayout(rangeLayout);
    mainLayout->addLayout(saveLayout);
    mainLayout->addWidget(processButton);
    mainLayout->addWidget(statusLabel);

    // Сигналы и слоты
    connect(loadFileButton, &QPushButton::clicked, this, &SubsequenceWindow::loadFile);
    connect(chooseSaveButton, &QPushButton::clicked, this, &SubsequenceWindow::chooseSaveLocation);
    connect(processButton, &QPushButton::clicked, this, &SubsequenceWindow::processSubsequences);
}

SubsequenceWindow::~SubsequenceWindow() = default;

void SubsequenceWindow::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "Текстовые файлы (*.txt)");
    if (!fileName.isEmpty()) {
        filePathField->setText(fileName);
    }
}

void SubsequenceWindow::chooseSaveLocation() {
    QString savePath = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения");
    if (!savePath.isEmpty()) {
        saveLocationField->setText(savePath);
    }
}

void SubsequenceWindow::processSubsequences() {
    QString filePath = filePathField->text();
    int lmin = lminSpinBox->value();
    int lmax = lmaxSpinBox->value();
    QString saveLocation = saveLocationField->text();

    if (filePath.isEmpty()) {
        statusLabel->setText("Ошибка: Укажите путь к файлу.");
        return;
    }

    if (saveLocation.isEmpty()) {
        statusLabel->setText("Ошибка: Укажите папку для сохранения.");
        return;
    }

    // Здесь вызывается ваша логика анализа подпоследовательностей
    QMessageBox::information(this, "Готово", "Данные успешно обработаны и сохранены!");
}
