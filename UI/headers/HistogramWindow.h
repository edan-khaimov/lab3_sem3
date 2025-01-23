#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

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
};

#endif // HISTOGRAMWINDOW_H
