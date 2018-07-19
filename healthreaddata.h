#ifndef HEALTHREADDATA_H
#define HEALTHREADDATA_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <hdata.h>
class HealthReadData : public QObject
{
	Q_OBJECT
public:
	explicit HealthReadData(QObject *parent = 0);

signals:
	void sendAllUserFromSql(struct IDNAMELIST);
	void sendNewUseMesgToAddItem(struct IDNAMELIST);
	void sendQueryToShowUser(QStringList list);
	void sendQueryToShowReport(QStringList list);
	void sendVideoPathToanaliysis(QString saglist,QString fronlist);
	void sendVideoPathWhichInsert(QString list);
	void sendJoinParamKneeAngleToSag(QString);
	void sendJoinParamKneeAngleToFron(QString);
    void sendOriginalDataToAna(QStringList saglist,QStringList fronlist);
    void sendErrorInformation(QString);
public slots:
	void deleteFromDatabase(QString);
	void getVideoPath(QString);
	void insertVideoPathToDatabase(QString,QString,QString);
    void updateRecoder(QString type,QString recoder,QString id);
	void getAllUserName();
    void createTable();
	void insertNewUser(QString,QString);
	void getShowUserMessageFromDatabase(QString);
	void getShowReportMessage(QString msg);
	void insertJoinKneeAngle(QString filed,QString joinkneeangle, QString id);
	void updateOriginalData(QString x,QString y,int type ,QString id,QString num);
    void queryOriginalData(QString ,bool ,bool);
	void getDataToChart(QString tag,QString id,int chartId);
	void updateProntalData(QString footAngle,QString stepWidth,QString id);
	void updateSaggitalData(QString cadence,QString srideLength,QString stepLength,QString walkingSpeed,QString id);
private:
	QStringList analysisTheString(QString q,int num);
private :
	QSqlDatabase db;
};

#endif // HEALTHREADDATA_H
