#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    private slots:
        void openHistogramWindow();
    void openSubsequenceWindow();

private:
    QPushButton *histogramButton;
    QPushButton *subsequenceButton;
};

#endif // MAINWINDOW_H
