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
    QLineEdit *filePathField;          // Поле для ввода пути к файлу
    QSpinBox *lminSpinBox;             // Поле для ввода значения lmin
    QSpinBox *lmaxSpinBox;             // Поле для ввода значения lmax
    QLineEdit *saveLocationField;      // Поле для выбора пути сохранения
    QPushButton *loadFileButton;       // Кнопка для загрузки файла
    QPushButton *chooseSaveButton;     // Кнопка для выбора папки сохранения
    QPushButton *processButton;        // Кнопка для обработки данных
    QLabel *statusLabel;               // Метка для отображения статуса
};

#endif // SUBSEQUENCEWINDOW_H
