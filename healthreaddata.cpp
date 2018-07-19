#include "healthreaddata.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QJsonArray>
#include "hdata.h"
HealthReadData::HealthReadData(QObject *parent) : QObject(parent)
{

}
void HealthReadData::createTable()
{
	qDebug() <<QSqlDatabase::drivers();
	//添加数据库
	db = QSqlDatabase::addDatabase("QMYSQL","healthdb");
	//链接数据库
	db.setHostName("127.0.0.1");
	db.setUserName("root");
	db.setPassword("123456");
	db.setDatabaseName("health");
	if(!db.open())
	{
		qDebug()<<"Fail to open db";
		
		emit sendErrorInformation("数据库打开失败 lastError " + db.lastError().databaseText());
		db.close();
		return;
	}
	else
	{
		qDebug()<<"Successed to open db"<<db.lastError();
	}
	QSqlQuery query;
	query = QSqlQuery(db);
	query.exec("create table if not exists health (id int auto_increment primary key,"
			   "name varchar(20),age varchar(10),hight varchar(10),shebao varchar(20),"
			   "footAngle varchar(20),stepWidth varchar(20),cadence varchar(20),srideLength varchar(20),"
			   "stepLength varchar(20),walkingSpeed varchar(20),sagfilepath varchar(225),fronfilepath varchar(225),"
			   "recoder varchar(20),newrecoder varchar(225),result varchar(225),frontalkneeangle TEXT,paramskneeangle TEXT,"
               "originalfronx TEXT,originalfrony TEXT,originalfronsize varchar(20),originalsagx TEXT,originalsagy TEXT,originalsagsize varchar(20))");
}

void  HealthReadData::getAllUserName()
{   //QStringList nameList;
	struct IDNAMELIST list;
	QSqlQuery query;
	query = QSqlQuery(db);
	query.exec("select * from health");
	while(query.next())
	{
	   list.nameList<<query.value("name").toString();
	   list.idList<<query.value("id").toString();
	   if(query.value("newrecoder").toString()==NULL)
	   {
		   list.newRecoderList<<"999999";
	   }else
	   {
		   list.newRecoderList<<query.value("newrecoder").toString();
	   }

	   if(query.value("result").toString()==NULL)
	   {
		   list.resultList<<"999999";
	   }else
	   {
		   list.resultList<<query.value("result").toString();
	   }
	}
	emit sendAllUserFromSql(list);
}

void  HealthReadData::insertNewUser(QString sql,QString name)
{
   qDebug()<<"insertNewUser "<<sql<<"name "<<name;
   QSqlQuery query;
   query = QSqlQuery(db);

   IDNAMELIST list;
   list.nameList<<name;
   list.newRecoderList<<"999999";
   list.resultList<<"999999";
   query.prepare(sql);
   query.exec();

   QString maxId = "select last_insert_id()";
   bool tag = query.exec(maxId);

   if(!tag)
   {
	   emit sendErrorInformation("插入失败 请检查数据库");
	   return;
   }
   else{
		emit sendErrorInformation("数据插入成功");
   }
   while(query.next())
   {
	   list.idList<<query.value(0).toString();
   }
   emit sendNewUseMesgToAddItem(list);
}
void HealthReadData::getShowUserMessageFromDatabase(QString msg)
{

	qDebug()<<"getShowUserMessageFromDatabase "<<msg;
	QSqlQuery query;

	query = QSqlQuery(db);

	QString sql = "select * from health";

	sql =sql.append(" where id = ").append("\"").append(msg).append("\"");

	query.exec(sql);
	QStringList list;
	while(query.next())
	{
		list<<query.value("name").toString()
			<<query.value("age").toString()
			<<query.value("hight").toString()
			<<query.value("shebao").toString();
	}
	emit sendQueryToShowUser(list);
}

void HealthReadData::getShowReportMessage(QString msg)
{

	qDebug()<<"getShowReportMessage "<<msg;
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "select * from health";
	sql =sql.append(" where id = ").append("\"").append(msg).append("\"");
	query.exec(sql);
	QStringList list;

	while(query.next())
	{
	list<<query.value("name").toString()
		<<query.value("age").toString()
		<<query.value("footAngle").toString()
		<<query.value("stepWidth").toString()
		<<query.value("cadence").toString()
		<<query.value("srideLength").toString()
		<<query.value("stepLength").toString()
		<<query.value("walkingSpeed").toString()
		<<query.value("frontalkneeangle").toString()
		<<query.value("paramskneeangle").toString();
	}
	emit sendQueryToShowReport(list);
}

void HealthReadData::deleteFromDatabase(QString msg)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "delete from health where id = '"+msg+"'";
	qDebug()<<"deleteUserInSql "<<sql<<"  "<<msg;
	query.exec(sql);
}

void HealthReadData::getVideoPath(QString id)
{
	qDebug()<<"getVideoPath id"<<id;
	QString sql = "select sagfilepath,fronfilepath from health where id = '"+id+"'";
	QSqlQuery query;
	query = QSqlQuery(db);
	query.exec(sql);
	query.next();
	qDebug()<<"getVideoPath "<<query.value(0);
	emit sendVideoPathToanaliysis(query.value(0).toString(),query.value(1).toString());
}

void HealthReadData::insertVideoPathToDatabase(QString type,QString filepath,QString id)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "update health set "+type+" = '"+filepath+" 'where id = '"+id+"'";
	qDebug()<<"update fileName"<<sql;
	query.exec(sql);
	emit sendVideoPathWhichInsert(id);
}

void HealthReadData::updateRecoder(QString type,QString recoder,QString id)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "update health set "+type+" = '"+recoder+" 'where id = '"+id+"'";
	//qDebug()<<"update newrecoder"<<sql;
	query.exec(sql);
}

void HealthReadData::updateProntalData(QString footAngle,QString stepWidth,QString id)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "update health set footAngle = '"+footAngle+
			"',stepWidth = '"+stepWidth+" 'where id = '"+id+"'";
	//qDebug()<<"updateprontaldata"<<sql;
	query.exec(sql);
}

void HealthReadData::updateSaggitalData(QString cadence,QString srideLength,QString stepLength,QString walkingSpeed,QString id)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "update health set cadence = '"+cadence+
			"',srideLength = '"+srideLength+
			"',stepLength = '"+stepLength+
			"',walkingSpeed = '"+walkingSpeed+" 'where id = '"+id+"'";
	//qDebug()<<"updateSaggitalData"<<sql;
	query.exec(sql);
}
void HealthReadData::updateOriginalData(QString x,QString y,int type ,QString id,QString num)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql;
	if(type == 0){
		 sql = "update health set originalsagx = '"+x+
				"',originalsagy = '"+y+"',originalsagsize = '"+num+" 'where id = '"+id+"'";
	}
	else if(type == 1)
	{
		sql = "update health set originalfronx = '"+x+
			   "',originalfrony = '"+y+"',originalfronsize = '"+num+" 'where id = '"+id+"'";
	}

	//qDebug()<<"updateprontaldata"<<sql;
	query.exec(sql);
}

void HealthReadData::queryOriginalData(QString id,bool sagdata,bool frondata)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "select originalfronx,originalfrony,originalfronsize,originalsagx,"
				  "originalsagy,originalsagsize from health where id = '"+id+"'";
    //qDebug()<<"queryOriginalData "<<sql;
	query.exec(sql);
	QStringList saglist;
    saglist.clear();
	QStringList fronlist;
    fronlist.clear();
	while(query.next())
	{
		if(sagdata)
        {
            if(query.value("originalsagx").toString()!=NULL)
            {
               saglist<<query.value("originalsagx").toString();
            }
            if(query.value("originalsagy").toString()!=NULL)
            {
                saglist<<query.value("originalsagy").toString();
            }
             if(query.value("originalsagsize").toString()!=NULL)
             {
                saglist<<query.value("originalsagsize").toString();
             }
		}
		if (frondata)
        {
            if(query.value("originalfronx").toString()!=NULL)
            {
                fronlist<<query.value("originalfronx").toString();
            }
            if(query.value("originalfrony").toString()!=NULL)
            {
                fronlist<<query.value("originalfrony").toString();
            }
            if(query.value("originalfronsize").toString()!=NULL)
            {
                fronlist<<query.value("originalfronsize").toString();
            }
		}
	}
    if(sagdata||frondata)
    {
        emit sendOriginalDataToAna(saglist,fronlist);
    }
}
void HealthReadData::insertJoinKneeAngle(QString filed, QString joinkneeangle, QString id)
{
	QSqlQuery query;
	query = QSqlQuery(db);
	QString sql = "update health set "+filed+" = '"+joinkneeangle+" 'where id = '"+id+"'";
	//qDebug()<<"insertJoinKneeAngle"<<sql;
	query.exec(sql);
}
void HealthReadData::getDataToChart(QString tag,QString id,int chartId)
{
	QString sql = "select "+tag+" from health where id = '"+id+"'";
	qDebug()<<"getDataToChart"<<sql<<" "<<chartId;
	QSqlQuery query;
	query = QSqlQuery(db);
	query.exec(sql);
	query.next();
    //qDebug()<<query.value(0).toString();
	if(chartId == 1){
		emit sendJoinParamKneeAngleToSag(query.value(0).toString());
	}
	else if (chartId == 2)
	{
		emit sendJoinParamKneeAngleToFron(query.value(0).toString());
	}
}
