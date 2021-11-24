#ifndef QPOINTERDEMO_H
#define QPOINTERDEMO_H

#include <QWidget>
#include <QDebug>
#include <QPointer>
#include <QException>

class CPointerDemoWidget : public QWidget
{
public:
    explicit CPointerDemoWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QWidget(parent, f)
    {
        qDebug() << __FUNCTION__ ;
    }

    ~CPointerDemoWidget()
    {
        qDebug() << __FUNCTION__ ;
    }

    static void test()
    {
        QPointer<CPointerDemoWidget> pQPointerDemoWidget = new CPointerDemoWidget;
        delete pQPointerDemoWidget;

        if(pQPointerDemoWidget)
            pQPointerDemoWidget->show();

        CPointerDemoWidget * pDemoWidget = new CPointerDemoWidget;
        delete pDemoWidget;


        if(pDemoWidget)
            pDemoWidget->show();
    }
};


#endif // QPOINTERDEMO_H
