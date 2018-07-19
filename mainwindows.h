#ifndef MAINWINDOWS_H
#define MAINWINDOWS_H

#include <QWidget>

namespace Ui {
class MainWindows;
}

class MainWindows : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindows(QWidget *parent = 0);
    ~MainWindows();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();

private:
    Ui::MainWindows *ui;
};

#endif // MAINWINDOWS_H
