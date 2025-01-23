#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

#include "../headers/HistogramWindow.h"
#include "../../headers/Histogram.h"
#include "../../../sorting/Person.h"

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

    // Список разбиений с уменьшенным размером
    rangeList->setMaximumHeight(100); // Ограничение высоты
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
        QMessageBox QLabel {
            color: #000000; /* Черный текст ошибок */
        }
    )");
}

void HistogramWindow::createTableHeaders() const {
    QStringList headers = {
        "Разбиение", "Медиана (Возраст)", "Дисперсия (Возраст)", "Среднее (Возраст)",
        "Медиана (Рост)", "Дисперсия (Рост)", "Среднее (Рост)",
        "Медиана (Вес)", "Дисперсия (Вес)", "Среднее (Вес)",
        "Медиана (Зарплата)", "Дисперсия (Зарплата)", "Среднее (Зарплата)",
        "Мужчина", "Женщина",
        "В браке", "Не в браке", "В разводе", "Вдовец/Вдова",
        "Основное общее", "Среднее общее", "Среднее профессиональное", "Бакалавриат", "Магистратура", "Аспирантура"
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
    rangesArray.Append({start, end});
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
    QString parameter = splitParameterComboBox->currentText();

    if (ranges.isEmpty() || filePath.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите файл и добавьте хотя бы одно разбиение.");
        return;
    }

    ArraySequence<Person> persons;
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Person person;
    std::ifstream file(filePath.toStdString());

    if (!file.is_open()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream stream(line);
        std::string tmp;

        std::getline(stream, tmp, ',');
        person.setSurname(tmp);
        std::getline(stream, tmp, ',');
        person.setName(tmp);
        std::getline(stream, tmp, ',');
        person.setPatronymic(tmp);
        std::getline(stream, tmp, ',');
        person.setGender(tmp);
        std::getline(stream, tmp, ',');
        person.setAge(std::stoi(tmp));
        std::getline(stream, tmp, ',');
        person.setWeight(std::stoi(tmp));
        std::getline(stream, tmp, ',');
        person.setHeight(std::stoi(tmp));
        std::getline(stream, tmp, ',');
        person.setEducation(tmp);
        std::getline(stream, tmp, ',');
        person.setMaritalStatus(tmp);
        std::getline(stream, tmp, ',');
        person.setPassportSeries(std::stoi(tmp));
        std::getline(stream, tmp, ',');
        person.setPassportNumber(std::stoi(tmp));
        std::getline(stream, tmp, ',');
        person.setSalary(std::stoi(tmp));

        persons.Append(person);
    }

    file.close();

    Histogram histogram;

    if (parameter == "Возраст") {
        histogram.Build(persons, rangesArray, [](const Person& p) { return p.getAge(); });
    } else if (parameter == "Вес") {
        histogram.Build(persons, rangesArray, [](const Person& p) { return p.getWeight(); });
    } else if (parameter == "Рост") {
        histogram.Build(persons, rangesArray, [](const Person& p) { return p.getHeight(); });
    } else if (parameter == "Зарплата") {
        histogram.Build(persons, rangesArray, [](const Person& p) { return p.getSalary(); });
    }

    auto data = std::move(histogram.GetStatistics());

    for (int i = 0; i < ranges.size(); ++i) {
        auto& partition = data[rangesArray[i]];
        resultTable->insertRow(i);
        resultTable->setItem(i, 0, new QTableWidgetItem(ranges[i]));
        resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(partition.ages.median)));
        resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(partition.ages.variance)));
        resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(partition.ages.mean)));
        resultTable->setItem(i, 4, new QTableWidgetItem(QString::number(partition.heights.median)));
        resultTable->setItem(i, 5, new QTableWidgetItem(QString::number(partition.heights.variance)));
        resultTable->setItem(i, 6, new QTableWidgetItem(QString::number(partition.heights.mean)));
        resultTable->setItem(i, 7, new QTableWidgetItem(QString::number(partition.weights.median)));
        resultTable->setItem(i, 8, new QTableWidgetItem(QString::number(partition.weights.variance)));
        resultTable->setItem(i, 9, new QTableWidgetItem(QString::number(partition.weights.mean)));
        resultTable->setItem(i, 10, new QTableWidgetItem(QString::number(partition.salaries.median)));
        resultTable->setItem(i, 11, new QTableWidgetItem(QString::number(partition.salaries.variance)));
        resultTable->setItem(i, 12, new QTableWidgetItem(QString::number(partition.salaries.mean)));
        resultTable->setItem(i, 13, new QTableWidgetItem(QString::number(partition.genders["Мужчина"])));
        resultTable->setItem(i, 14, new QTableWidgetItem(QString::number(partition.genders["Женщина"])));
        resultTable->setItem(i, 15, new QTableWidgetItem(QString::number(partition.maritalStatuses["В браке"])));
        resultTable->setItem(i, 16, new QTableWidgetItem(QString::number(partition.maritalStatuses["Не в браке"])));
        resultTable->setItem(i, 17, new QTableWidgetItem(QString::number(partition.maritalStatuses["В разводе"])));
        resultTable->setItem(i, 18, new QTableWidgetItem(QString::number(partition.maritalStatuses["Вдовец/Вдова"])));
        resultTable->setItem(i, 19, new QTableWidgetItem(QString::number(partition.educations["Основное общее"])));
        resultTable->setItem(i, 20, new QTableWidgetItem(QString::number(partition.educations["Среднее общее"])));
        resultTable->setItem(i, 21, new QTableWidgetItem(QString::number(partition.educations["Среднее профессиональное"])));
        resultTable->setItem(i, 22, new QTableWidgetItem(QString::number(partition.educations["Бакалавриат"])));
        resultTable->setItem(i, 23, new QTableWidgetItem(QString::number(partition.educations["Магистратура"])));
        resultTable->setItem(i, 24, new QTableWidgetItem(QString::number(partition.educations["Аспирантура"])));
    }
}
