#include "../headers/HistogramWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <cstdlib> // Для std::rand() и std::srand()
#include <ctime>   // Для std::time()

HistogramWindow::HistogramWindow(QWidget *parent)
    : QWidget(parent),
      filePathEdit(new QLineEdit(this)),
      splitParameterComboBox(new QComboBox(this)),
      rangeStartEdit(new QLineEdit(this)),
      rangeEndEdit(new QLineEdit(this)),
      resultTable(new QTableWidget(this)),
      rangeList(new QListWidget(this)),
      addRangeButton(new QPushButton("Добавить разбиение", this)),
      selectFileButton(new QPushButton("Выбрать файл", this)),
      generateTableButton(new QPushButton("Получить данные", this)),
      mainLayout(new QVBoxLayout(this)) {

    std::srand(std::time(nullptr)); // Инициализация генератора случайных чисел
    setWindowTitle("Гистограммы");
    resize(800, 600);

    // Элементы для выбора файла
    auto *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(selectFileButton);
    fileLayout->addWidget(filePathEdit);

    // Настройка параметров разбиения
    splitParameterComboBox->addItems({"Возраст", "Рост", "Вес", "Зарплата"});

    auto *rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(splitParameterComboBox);
    rangeLayout->addWidget(rangeStartEdit);
    rangeLayout->addWidget(rangeEndEdit);
    rangeLayout->addWidget(addRangeButton);

    // Список разбиений
    auto *rangeListLayout = new QVBoxLayout();
    rangeListLayout->addWidget(rangeList);

    // Таблица результатов
    createTableHeaders();

    // Кнопка генерации данных
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(rangeLayout);
    mainLayout->addLayout(rangeListLayout);
    mainLayout->addWidget(resultTable);
    mainLayout->addWidget(generateTableButton);

    setLayout(mainLayout);
    applyStyles();

    // Сигналы и слоты
    connect(selectFileButton, &QPushButton::clicked, this, &HistogramWindow::openFileDialog);
    connect(addRangeButton, &QPushButton::clicked, this, &HistogramWindow::addRange);
    connect(generateTableButton, &QPushButton::clicked, this, &HistogramWindow::generateTable);
}

HistogramWindow::~HistogramWindow() = default;

void HistogramWindow::applyStyles() {
    setStyleSheet(R"(
        QWidget {
            background-color: #f9f9f9;
        }
        QLineEdit, QComboBox, QPushButton, QListWidget, QTableWidget {
            background-color: #ffffff;
            color: #000000; /* Черный текст */
            border: 1px solid #cccccc;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            color: #000000; /* Черный текст в выпадающем списке */
        }
        QLineEdit, QComboBox {
            padding: 5px;
        }
        QPushButton {
            border-radius: 5px;
            padding: 8px;
        }
        QPushButton:hover {
            background-color: #e6e6e6;
        }
        QPushButton:pressed {
            background-color: #d9d9d9;
        }
        QTableWidget {
            gridline-color: #cccccc;
        }
        QHeaderView::section {
            background-color: #f0f0f0;
            color: #000000; /* Черный текст */
            font-weight: bold;
            border: 1px solid #dcdcdc;
        }
    )");
}

void HistogramWindow::createTableHeaders() {
    QStringList headers = {
        "Разбиение", "Медиана (Возраст)", "Дисперсия (Возраст)", "Среднее (Возраст)",
        "Медиана (Рост)", "Дисперсия (Рост)", "Среднее (Рост)",
        "Медиана (Вес)", "Дисперсия (Вес)", "Среднее (Вес)",
        "Медиана (Зарплата)", "Дисперсия (Зарплата)", "Среднее (Зарплата)",
        "Мужской", "Женский",
        "В браке", "Не в браке", "Разведён", "Вдовец/Вдова",
        "Основное общее", "Среднее общее", "Бакалавриат", "Магистратура", "Аспирантура", "Среднее профессиональное"
    };

    resultTable->setColumnCount(headers.size());
    resultTable->setHorizontalHeaderLabels(headers);
    resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    resultTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

QStringList HistogramWindow::getRanges() const {
    QStringList ranges;
    for (int i = 0; i < rangeList->count(); ++i) {
        ranges.append(rangeList->item(i)->text());
    }
    return ranges;
}

void HistogramWindow::addRange() {
    bool okStart, okEnd;
    int start = rangeStartEdit->text().toInt(&okStart);
    int end = rangeEndEdit->text().toInt(&okEnd);

    if (!okStart || !okEnd || start < 0 || end < 0 || start >= end) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения для разбиения.");
        return;
    }

    QString rangeText = QString("От %1 до %2").arg(start).arg(end);
    rangeList->addItem(rangeText);
}

void HistogramWindow::openFileDialog() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите CSV файл", "", "CSV файлы (*.csv)");
    if (!fileName.isEmpty()) {
        filePathEdit->setText(fileName);
    }
}

void HistogramWindow::generateTable() {
    QStringList ranges = getRanges();
    QString filePath = filePathEdit->text();
    if (ranges.isEmpty() || filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите файл и добавьте хотя бы одно разбиение.");
        return;
    }

    for (int i = 0; i < ranges.size(); ++i) {
        resultTable->insertRow(i);
        resultTable->setItem(i, 0, new QTableWidgetItem(ranges[i]));
        for (int j = 1; j < resultTable->columnCount(); ++j) {
            resultTable->setItem(i, j, new QTableWidgetItem(QString::number(std::rand() % 100)));
        }
    }
}

void HistogramWindow::openTableInNewWindow() {
    auto *tableWindow = new QWidget();
    tableWindow->setWindowTitle("Результаты");
    tableWindow->resize(1200, 800);

    auto *tableCopy = new QTableWidget(resultTable->rowCount(), resultTable->columnCount());
    QStringList headers;
    for (int col = 0; col < resultTable->columnCount(); ++col) {
        headers.append(resultTable->horizontalHeaderItem(col)->text());
    }
    tableCopy->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < resultTable->rowCount(); ++i) {
        for (int j = 0; j < resultTable->columnCount(); ++j) {
            auto *item = resultTable->item(i, j);
            if (item) {
                tableCopy->setItem(i, j, new QTableWidgetItem(item->text()));
            }
        }
    }

    tableCopy->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableCopy->setStyleSheet(resultTable->styleSheet());

    auto *layout = new QVBoxLayout(tableWindow);
    layout->addWidget(tableCopy);
    tableWindow->setLayout(layout);
    tableWindow->show();
}

