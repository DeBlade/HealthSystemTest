#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <playvideo.h>
#include <QPushButton>
#include <QResizeEvent>
#include "frontalshowchart.h"
#include <saggitalshowchart.h>
#include "bydGaitAnalysis.h"
#include "healthreaddata.h"
namespace Ui {
class Analysis;
}

class Analysis : public QWidget
{
	Q_OBJECT

public:
	explicit Analysis(QWidget *parent = 0,HealthReadData *readData = NULL);
	~Analysis();
private:
	void resizeEvent(QResizeEvent *event);
	void initButton();
	void initStackButton(QPushButton *button,QString text);
	void setButtonDisable();
	void getData(QString);
	void initTheUi();
	bool isItemHasParent(QTreeWidgetItem *);
	void initTheChartView();
	QStringList analysisTheString(QString str,int num);
	void analysisShowChartView(QChartView *chartview,QString strx,QString stry,int num);
    void getfilePath(QString );
signals :
	void sendMessageToShowReport(QString id,bool tag);
	void sendmesstoOpen(QString);
	void sentVideoPath(QString);
	void sentGroupBoxSizeToSag(QSize);
	void sentGroupBoxSizeToFron(QSize);
	void sendIdtoGetFilePath(QString);
	void sendKneeAngleTodataBase(QString,QString,QString);
    void getFilePathName(QString,QString );
	void sagButtoClick(QString);
	void getOriginalDataFromDatabase(QString ,bool ,bool);
	void FornButtoClick(QString);
	void sendProntalData(QString,QString,QString);
	void sendOriginalData(QString,QString,int,QString,QString);
	void sendSaggitalData(QString,QString,QString,QString,QString);
    void resetChartView();
public slots:
	void receiveMessaeFromhealth(QTreeWidgetItem *item);
	void resetGroupBoxSize(QSize size);
    void restShowChartView();
private slots:
	void on_getbutton_clicked();

	void on_getreportbutton_clicked();

	void showSaggitalAlterWindow();

	void showFrontalAlterWindow();

	void getVideoPathNext(QString sagpath,QString fronpath);

    void receiveFromopen(QString type,QString file);

	void on_sagbutton_clicked();

	void on_fornButton_clicked();

	void resetTheUi(QString id);

	void sagVideoButtonClick();

	void fronVideoButtonClick();

	void receiverOriginalData(QStringList saglist,QStringList fronlist);

	void on_sagFileButton_clicked();

	void on_fronFileButton_clicked();

private:
	Ui::Analysis *ui;
	QPushButton* sagPushButton;
	QPushButton* fronPushButton;
	QPushButton* sagVideoPushButton;
	QPushButton* fronVideoPushButton;
	QString userId;
	SaggitalShowChart *sagShowchart;
	FrontalShowChart *fronShowChart;
	GaitParams params;
	QTreeWidgetItem *aItem;
	HealthReadData *aReadData;
	QString sagfilepath;
	QString fronfilepath;
	QChartView *sagreportviewsagchartview;
	QChartView *sagchartview;
	QChartView *fronchartview;
};

#endif // ANALYSIS_H
