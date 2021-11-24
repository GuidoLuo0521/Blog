#ifndef QSHAREDPOINTERDEMO_H
#define QSHAREDPOINTERDEMO_H

#include <QWidget>
#include <QDebug>
#include <QSharedPointer>
#include <QApplication>

class CQSharedPointerDemoWidget : public QWidget
{
public:
    explicit CQSharedPointerDemoWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
        : QWidget(parent, f)
    {
        qDebug() << __FUNCTION__ ;
    }

    ~CQSharedPointerDemoWidget()
    {
        qDebug() << __FUNCTION__ ;
    }

#define TEST_QSHAREDPOINTER 0

    static void test()
    {
        qDebug() << __FUNCTION__ ;
#if TEST_QSHAREDPOINTER
        qDebug() << "TEST_QSHAREDPOINTER" ;
        QSharedPointer<CQSharedPointerDemoWidget> pWidget(new CQSharedPointerDemoWidget);
#else
        CQSharedPointerDemoWidget * pWidget = new CQSharedPointerDemoWidget;
#endif
        pWidget->show();
    }
};


#endif // QSHAREDPOINTER_H
