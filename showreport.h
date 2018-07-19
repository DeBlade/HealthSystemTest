#ifndef SHOWREPORT_H
#define SHOWREPORT_H

#include <QWidget>
#include <QSqlQuery>
#include <QTreeWidgetItem>
#include "healthreaddata.h"
#include <QChartView>
#include <QChart>
#include <QSplineSeries>
#include <QLabel>
namespace Ui {
class ShowReport;
}

class ShowReport : public QWidget
{
    Q_OBJECT

public:
    explicit ShowReport(QWidget *parent = 0, QString id = NULL,HealthReadData *readData = NULL);
    ~ShowReport();
signals :
    void getUserInformation(QString id);
    void sentWidgetSizeToChart(QSize);

private slots:
    void updateReportUi(QStringList list);

private :
    void reportShowChartView(QtCharts::QChartView *chartview,QString str,int type);
    QStringList analysisTheString(QString str,int num);
    void resizeEvent(QResizeEvent *event);
    void setTextInReport(QLabel *label,QLabel *resultlabel,QString str,float min,float max);

private:
    Ui::ShowReport *ui;
    QtCharts::QChartView *sagreportview;
    QtCharts::QChartView *fronreporttview;
};

#endif // SHOWREPORT_H
