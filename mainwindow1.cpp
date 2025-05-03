#include "mainwindow1.h"
#include "ui_mainwindow1.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_released()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);

}


void MainWindow::on_pushButton_released()
{
    if(gamewindow) {
        gamewindow->close();
        gamewindow->deleteLater();
    }

    gamewindow = new GameWindow();  // 不传递parent使其独立
    gamewindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(gamewindow, &GameWindow::destroyed, this, [this](){
        gamewindow = nullptr;
        this->show();
    });

    this->hide();
    gamewindow->show();

}


void MainWindow::on_pushButton_3_released()//退出按钮
{
    close();
}


void MainWindow::on_pushButton_4_released()//返回
{
     ui->stackedWidget->setCurrentWidget(ui->page);
}

