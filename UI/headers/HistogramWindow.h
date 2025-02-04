#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include "../../headers/Histogram.h"

#include "../../../sequences/arraySequence.h"

class HistogramWindow : public QWidget {
    Q_OBJECT

public:
    explicit HistogramWindow(QWidget *parent = nullptr);
    ~HistogramWindow() override;

    private slots:
        void openFileDialog();
    void addRange();
    void generateTable();
    void plotHistograms();
    QMap<QString, int> calculateHistogramData(const ArraySequence<int>& data, const QString& param);
    std::pair<int, int> getRange(int field, const ArraySequence<std::pair<int, int>>& ranges);

private:
    void applyStyles();
    void createTableHeaders() const;
    QStringList getRanges() const;

    QLineEdit *filePathEdit;
    QComboBox *splitParameterComboBox;
    QLineEdit *rangeStartEdit;
    QLineEdit *rangeEndEdit;
    QTableWidget *resultTable;
    QListWidget *rangeList;
    QPushButton *addRangeButton;
    QPushButton *selectFileButton;
    QPushButton *generateTableButton;
    QVBoxLayout *mainLayout;
    ArraySequence<std::pair<int, int>> rangesArray;
    QPushButton *plotButton;
    IDictionary<std::pair<int, int>, PartitionStatistics> cachedStats;
    QScrollArea *scrollArea;
    QWidget *chartsContainer;
};

#endif // HISTOGRAMWINDOW_H
