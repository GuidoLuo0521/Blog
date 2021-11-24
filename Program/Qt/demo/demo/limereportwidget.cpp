#include "limereportwidget.h"
#include "ui_limereportwidget.h"
#include <QHBoxLayout>


LimeReportWidget::LimeReportWidget(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f),
    ui(new Ui::LimeReportWidget),
    m_pCallbackDatasource(nullptr),
    m_pCbdsPreviewReportWidget(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("LimeReport Demo");

    InitCallbackReport();

    InitGroupReport();

    InitTableWidgetReport();
}

LimeReportWidget::~LimeReportWidget()
{
    delete ui;
}

void LimeReportWidget::on_btnReportDesigner_clicked()
{
    LimeReport::ReportEngine report;
    report.designReport();
}

void LimeReportWidget::on_btnCallbackDatasource_clicked()
{
    m_CallbackDsReport.designReport();
    //m_pCbdsPreviewReportWidget->refreshPages();
}

void LimeReportWidget::on_btnGroup_clicked()
{
    m_GroupReport.designReport();
}

void LimeReportWidget::slotGetCallbackData(const LimeReport::CallbackInfo &info, QVariant &data)
{
    switch (info.dataType)
    {
    case LimeReport::CallbackInfo::IsEmpty:
        break;
    case LimeReport::CallbackInfo::HasNext:
        break;
    case LimeReport::CallbackInfo::ColumnHeaderData:
        data = m_CallbackDs.at(info.index);
        break;
    case LimeReport::CallbackInfo::ColumnData:
        data = getColumnData(info.columnName);
        break;
    case LimeReport::CallbackInfo::ColumnCount:
        data = m_CallbackDs.count();
        break;
    case LimeReport::CallbackInfo::RowCount:
        data = m_nCallbackDsRowCount;
        break;
    default:
        break;
    }
}

void LimeReportWidget::prepareCallbackDs()
{
    if(m_CallbackDs.count() <= 0)
    {
        m_CallbackDs << "UID" << "Name" << "Sex" << "Birthday" << "Age"
                     << "AgeUnit" << "Telephone" << "PatientID" << "PatientType" << "MedicalRecordNo";
    }

    m_nCallbackDsRowCount = 20;
}

void LimeReportWidget::InitCallbackReport()
{
    m_pCallbackDatasource = m_CallbackDsReport.dataManager()->createCallbackDatasource("CallBackDatasource");
    prepareCallbackDs();

    connect(m_pCallbackDatasource, SIGNAL(getCallbackData(const LimeReport::CallbackInfo&, QVariant&)),
            this, SLOT(slotGetCallbackData(const LimeReport::CallbackInfo&, QVariant&)));

    m_CallbackDsReport.loadFromFile("test.lrxml", true);
    //m_pCbdsPreviewReportWidget = m_CallbackDsReport.createPreviewWidget();
    //this->layout()->addWidget(m_pCbdsPreviewReportWidget);
}

QVariant LimeReportWidget::getColumnData(const QString &key)
{
    if(key == "UID")
    {
        return "UID";
    }
    else if(key == "Name")
    {
        return "Name";
    }
    else if(key == "Sex")
    {
        return "Sex";
    }
    else if(key == "Birthday")
    {
        return "Birthday";
    }
    else if(key == "Age")
    {
        return "Age";
    }
    else if(key == "AgeUnit")
    {
        return "AgeUnit";
    }
    else if(key == "Telephone")
    {
        return "Telephone";
    }
    else if(key == "PatientID")
    {
        return "PatientID";
    }
    else if(key == "PatientType")
    {
        return "PatientType";
    }
    else if(key == "MedicalRecordNo")
    {
        return "MedicalRecordNo";
    }
    else
        return "";
}

void LimeReportWidget::InitGroupReport()
{

}

void LimeReportWidget::InitTableWidgetReport()
{
    const int knColumnCount = 4;
    const int knRowCount = 5;

    ui->tableWidget->setColumnCount(knColumnCount);
    ui->tableWidget->setRowCount(knRowCount);

    for(int i = 0; i < knColumnCount; ++i)
    {
        for(int j = 0; j < knRowCount; ++j)
        {
            QString strItem = QString("item %1 %2").arg(i).arg(j);
            ui->tableWidget->setItem(j, i, new QTableWidgetItem(strItem));
        }
    }

    m_TableWidgetReport.dataManager()->addModel("TableWidget", ui->tableWidget->model(), false);
}


void LimeReportWidget::on_btnTableWidget_clicked()
{
    m_TableWidgetReport.designReport();
}
