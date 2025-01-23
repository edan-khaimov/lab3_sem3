#ifndef SUBSEQUENCEWINDOW_H
#define SUBSEQUENCEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>

class SubsequenceWindow : public QWidget {
    Q_OBJECT

public:
    explicit SubsequenceWindow(QWidget *parent = nullptr);
    ~SubsequenceWindow() override;

    private slots:
        void loadFile();
    void chooseSaveLocation();
    void processSubsequences();

private:
    QLineEdit *filePathField;
    QSpinBox *lminSpinBox;
    QSpinBox *lmaxSpinBox;
    QLineEdit *saveLocationField;
    QPushButton *loadFileButton;
    QPushButton *chooseSaveButton;
    QPushButton *processButton;
    QLabel *statusLabel;
};

#endif // SUBSEQUENCEWINDOW_H
