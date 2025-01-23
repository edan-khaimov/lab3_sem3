#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

class HistogramWindow : public QWidget {
    Q_OBJECT

public:
    explicit HistogramWindow(QWidget *parent = nullptr);
    ~HistogramWindow() override;

    private slots:
        void openFileDialog();
    void addRange();
    void generateTable();
    void openTableInNewWindow();

private:
    void applyStyles();
    void createTableHeaders();
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
};

#endif // HISTOGRAMWINDOW_H
