#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include"gamewindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_2_released();

    void on_pushButton_released();

    void on_pushButton_3_released();

    void on_pushButton_4_released();

private:
    Ui::MainWindow *ui;
    GameWindow*gamewindow=nullptr;
};

#endif // MAINWINDOW1_H
