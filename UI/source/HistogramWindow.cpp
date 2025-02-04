#include <QtCharts>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "../../../sorting/Person.h"
#include "../../headers/Histogram.h"
#include "../headers/HistogramWindow.h"

HistogramWindow::HistogramWindow(QWidget* parent) :
    QWidget(parent), filePathEdit(new QLineEdit(this)), splitParameterComboBox(new QComboBox(this)),
    rangeStartEdit(new QLineEdit(this)), rangeEndEdit(new QLineEdit(this)), resultTable(new QTableWidget(this)),
    rangeList(new QListWidget(this)), addRangeButton(new QPushButton("Добавить разбиение", this)),
    selectFileButton(new QPushButton("Выбрать файл", this)),
    generateTableButton(new QPushButton("Получить данные", this)), mainLayout(new QVBoxLayout(this)),
    plotButton(new QPushButton("Построить графики", this)) {

    setWindowTitle("Гистограммы");
    resize(800, 600);

    auto* fileLayout = new QHBoxLayout();
    fileLayout->addWidget(selectFileButton);
    fileLayout->addWidget(filePathEdit);

    splitParameterComboBox->addItems({"Возраст", "Рост", "Вес", "Зарплата"});

    auto* rangeLayout = new QHBoxLayout();
    rangeLayout->addWidget(splitParameterComboBox);
    rangeLayout->addWidget(rangeStartEdit);
    rangeLayout->addWidget(rangeEndEdit);
    rangeLayout->addWidget(addRangeButton);

    rangeList->setMaximumHeight(100);
    auto* rangeListLayout = new QVBoxLayout();
    rangeListLayout->addWidget(rangeList);

    createTableHeaders();

    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(rangeLayout);
    mainLayout->addLayout(rangeListLayout);
    mainLayout->addWidget(resultTable);
    mainLayout->addWidget(generateTableButton);
    mainLayout->addWidget(plotButton);

    setLayout(mainLayout);
    applyStyles();

    connect(selectFileButton, &QPushButton::clicked, this, &HistogramWindow::openFileDialog);
    connect(addRangeButton, &QPushButton::clicked, this, &HistogramWindow::addRange);
    connect(generateTableButton, &QPushButton::clicked, this, &HistogramWindow::generateTable);
    connect(plotButton, &QPushButton::clicked, this, &HistogramWindow::plotHistograms);
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
    QStringList headers = {"Разбиение",
                           "Медиана (Возраст)",
                           "Дисперсия (Возраст)",
                           "Среднее (Возраст)",
                           "Медиана (Рост)",
                           "Дисперсия (Рост)",
                           "Среднее (Рост)",
                           "Медиана (Вес)",
                           "Дисперсия (Вес)",
                           "Среднее (Вес)",
                           "Медиана (Зарплата)",
                           "Дисперсия (Зарплата)",
                           "Среднее (Зарплата)",
                           "Мужчина",
                           "Женщина",
                           "В браке",
                           "Не в браке",
                           "В разводе",
                           "Вдовец/Вдова",
                           "Основное общее",
                           "Среднее общее",
                           "Среднее профессиональное",
                           "Бакалавриат",
                           "Магистратура",
                           "Аспирантура"};

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

    cachedStats = std::move(histogram.GetStatistics());

    for (int i = 0; i < ranges.size(); ++i) {
        auto& partition = cachedStats[rangesArray[i]];
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
        resultTable->setItem(i, 21,
                             new QTableWidgetItem(QString::number(partition.educations["Среднее профессиональное"])));
        resultTable->setItem(i, 22, new QTableWidgetItem(QString::number(partition.educations["Бакалавриат"])));
        resultTable->setItem(i, 23, new QTableWidgetItem(QString::number(partition.educations["Магистратура"])));
        resultTable->setItem(i, 24, new QTableWidgetItem(QString::number(partition.educations["Аспирантура"])));
    }
}

void HistogramWindow::plotHistograms() {
    if (cachedStats.GetCount() == 0) {
        QMessageBox::warning(this, "Ошибка", "Сначала получите данные.");
        return;
    }

    auto *window = new QWidget();
    window->setWindowTitle("Графики разбиений");
    window->setStyleSheet("background-color: #f5f5f5;");

    QTabWidget *tabWidget = new QTabWidget(window);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setStyleSheet(R"(
        QTabWidget::pane { border: 1px solid #cccccc; }
        QTabBar::tab {
            background: #e0e0e0;
            color: #333333;
            padding: 8px 12px;
            border: 1px solid #cccccc;
            border-bottom: none;
        }
        QTabBar::tab:selected {
            background: #ffffff;
            color: #000000;
        }
    )");

    for (auto& [range, stats] : cachedStats) {
        auto *tab = new QWidget();
        tab->setStyleSheet("background-color: #ffffff;");

        auto *scrollArea = new QScrollArea();
        scrollArea->setWidgetResizable(true);
        auto *scrollContent = new QWidget();
        auto *mainLayout = new QVBoxLayout(scrollContent);
        mainLayout->setSpacing(20);

        QStringList numericParams = {"Возраст", "Вес", "Рост", "Зарплата"};
        for (auto& param : numericParams) {
            QChart *chart = new QChart();
            chart->setBackgroundBrush(QBrush(Qt::white));
            chart->setTitle(param);

            QBarSeries *series = new QBarSeries();
            QBarSet *set = new QBarSet(param);
            set->setColor(QColor(70, 130, 180));

            ArraySequence<int>& data =
                (param == "Возраст") ? stats.agesData :
                (param == "Вес") ? stats.weightsData :
                (param == "Рост") ? stats.heightsData : stats.salariesData;

            auto bins = calculateHistogramData(data, param);
            QStringList categories = bins.keys();

            std::sort(categories.begin(), categories.end(), [](const QString& a, const QString& b) {
                int aStart = a.split("-")[0].toInt();
                int bStart = b.split("-")[0].toInt();
                return aStart < bStart;
            });

            for (auto& category : categories) {
                *set << bins[category];
            }

            series->append(set);
            chart->addSeries(series);

            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            QValueAxis *axisY = new QValueAxis();

            axisX->append(categories);
            axisX->setTitleText("Диапазон значений");
            axisY->setTitleText("Количество человек");
            axisY->setTickCount(10);
            axisY->setLabelFormat("%d");

            chart->addAxis(axisX, Qt::AlignBottom);
            chart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisX);
            series->attachAxis(axisY);

            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);
            chartView->setMinimumSize(600, 400);
            mainLayout->addWidget(chartView);
        }

        QStringList categoricalParams = {"Пол", "Образование", "Семейный статус"};
        auto *pieLayout = new QHBoxLayout();
        for (auto& param : categoricalParams) {
            QPieSeries *series = new QPieSeries();
            auto& data =
                (param == "Пол") ? stats.genders :
                (param == "Образование") ? stats.educations : stats.maritalStatuses;

            for (auto& [key, count] : data) {
                QString label = QString::fromStdString(key);
                QStringList words = label.split(' ');
                QString wrappedLabel;
                int lineLength = 0;
                const int maxLineLength = 15;

                for (QString& word : words) {
                    if (lineLength + word.length() + 1 > maxLineLength) {
                        wrappedLabel += "\n";
                        lineLength = 0;
                    }
                    wrappedLabel += word + " ";
                    lineLength += word.length() + 1;
                }

                wrappedLabel = wrappedLabel.trimmed();
                series->append(QString("%1\n(%2)").arg(wrappedLabel).arg(count), count);
            }

            QChart *pieChart = new QChart();
            pieChart->setBackgroundBrush(QBrush(Qt::white));
            pieChart->addSeries(series);
            pieChart->setTitle(param);

            QLegend *legend = pieChart->legend();
            legend->setAlignment(Qt::AlignBottom);
            legend->setMaximumWidth(500); // Увеличиваем ширину
            legend->setFont(QFont("Arial", 10, QFont::Normal));
            legend->setMarkerShape(QLegend::MarkerShapeRectangle);
            legend->setLabelBrush(QBrush(Qt::black));

            QChartView *pieView = new QChartView(pieChart);
            pieView->setRenderHint(QPainter::Antialiasing);
            pieView->setFixedSize(600, 400); // Увеличиваем размер диаграммы
            pieLayout->addWidget(pieView);
        }

        mainLayout->addLayout(pieLayout);

        scrollArea->setWidget(scrollContent);
        tabWidget->addTab(scrollArea, QString("%1-%2").arg(range.first).arg(range.second));
    }

    window->setLayout(new QVBoxLayout());
    window->layout()->addWidget(tabWidget);
    window->resize(1280, 720);
    window->show();
}

QMap<QString, int> HistogramWindow::calculateHistogramData(const ArraySequence<int>& data, const QString& param) {
    QMap<QString, int> bins;
    if (data.GetLength() == 0) return bins;

    int minVal, maxVal, step;
    if (param == "Возраст") {
        minVal = 0;
        maxVal = 100;
        step = 10;
    } else if (param == "Вес") {
        minVal = 3;
        maxVal = 203;
        step = 20;
    } else if (param == "Зарплата") {
        minVal = 0;
        maxVal = 1000000;
        step = 100000;
    } else if (param == "Рост") {
        minVal = 45;
        maxVal = 245;
        step = 20;
    } else {
        return bins;
    }

    for (int i = minVal; i < maxVal; i += step) {
        int end = i + step;
        if (end > maxVal) end = maxVal;
        bins[QString("%1-%2").arg(i).arg(end)] = 0;
    }

    for (auto& value : data) {
        int binIndex = (value - minVal) / step;
        if (binIndex < 0) binIndex = 0;
        if (binIndex >= bins.size()) binIndex = bins.size() - 1;
        int start = minVal + binIndex * step;
        int end = start + step;
        if (end > maxVal) end = maxVal;
        bins[QString("%1-%2").arg(start).arg(end)]++;
    }

    return bins;
}


std::pair<int, int> HistogramWindow::getRange(const int field, const ArraySequence<std::pair<int, int>>& ranges) {
    for (const auto& range : ranges) {
        if (field >= range.first && field < range.second) {
            return range;
        }
    }
    return {-1, -1};
}