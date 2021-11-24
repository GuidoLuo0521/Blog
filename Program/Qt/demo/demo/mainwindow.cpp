#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPointer>

#include "QPointerDemo.h"
#include "QSharedPointerDemo.h"
#include "limereportwidget.h"


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

void MainWindow::on_QSharedPointerTest_clicked()
{
    CQSharedPointerDemoWidget::test();
}

void MainWindow::on_btnLimeReport_clicked()
{
#if 1
    QPointer<LimeReportWidget> pWidget = new LimeReportWidget;
    pWidget->exec();

    pWidget->deleteLater();
    //delete pWidget;
#else
   LimeReportWidget widget;
   widget.exec();
#endif
}
