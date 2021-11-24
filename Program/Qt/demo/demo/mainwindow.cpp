#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QPointerDemo.h"

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


void MainWindow::on_QPointerTest_clicked()
{
    CPointerDemoWidget::test();
}
