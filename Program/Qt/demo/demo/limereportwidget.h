#ifndef LIMEREPORTWIDGET_H
#define LIMEREPORTWIDGET_H

#include <QDialog>
//#include "LimeReport"

#include "lrreportengine.h"             // to add report engine
#include "lrcallbackdatasourceintf.h"   // if you want use callback datasources

namespace Ui {
class LimeReportWidget;
}

class LimeReportWidget : public QDialog
{
    Q_OBJECT

public:
    explicit LimeReportWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~LimeReportWidget();

private slots:
    void on_btnReportDesigner_clicked();
    void on_btnCallbackDatasource_clicked();

    void slotGetCallbackData(const LimeReport::CallbackInfo& info, QVariant& data);

    void on_btnGroup_clicked();

    void on_btnTableWidget_clicked();

private:
    Ui::LimeReportWidget *ui;

    // callback datasource test
    void InitCallbackReport();
    void prepareCallbackDs();
    QVariant getColumnData(const QString& key);
    LimeReport::ReportEngine m_CallbackDsReport;
    QStringList m_CallbackDs;
    int m_nCallbackDsRowCount;
    LimeReport::ICallbackDatasource * m_pCallbackDatasource;
    LimeReport::PreviewReportWidget * m_pCbdsPreviewReportWidget;

    // group test
    void InitGroupReport();
    LimeReport::ReportEngine m_GroupReport;

    // table widget
    void InitTableWidgetReport();
    LimeReport::ReportEngine m_TableWidgetReport;
};

#endif // LIMEREPORTWIDGET_H
