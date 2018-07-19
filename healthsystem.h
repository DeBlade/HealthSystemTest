#ifndef HEALTHSYSTEM_H
#define HEALTHSYSTEM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <addnewuser.h>
#include <showuser.h>
#include <analysis.h>
#include <QStringList>
#include <QThread>
#include "healthreaddata.h"
#include "bydGaitAnalysis.h"
#include "hdata.h"
#include "welcomewidget.h"

namespace Ui {
class HealthSystem;
}

class HealthSystem : public QWidget
{
	Q_OBJECT

public:
	explicit HealthSystem(QWidget *parent = 0);
	~HealthSystem();
signals:
	void closeAddNewUser();
	void closeShowUser();
	void sentMsgToShowUser(QString,HealthReadData *);
	void sendMessageToAnalysis(QTreeWidgetItem *);
	void sendSignalToHealth();
	void insertUserMesageToDatabase(QString,QString);
	void deleteUserFromDatabase(QString);
	void updateNewRecoder(QString,QString,QString);
	void toResetAnalysisBoxsize(QSize size);
    void createDatabase();
    void currentChangeToresetChart();
private slots:
	void AddTreeRoot(struct IDNAMELIST);

	void receiveSignalItem(QTreeWidgetItem *,int x);

	void addnewUserItem(IDNAMELIST);

	void addnewUserSql(QString,QString);

	void deletefromtreewidget();

	void deletefromwidget();

	void on_pushButton_clicked();

	void on_gorightbutton_clicked();

	void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

	void on_treeWidget_itemCollapsed(QTreeWidgetItem *item);

	void on_treeWidget_customContextMenuRequested(const QPoint &pos);

	void deletefromshowuser();

	void ShowHealthReport(QString id,bool tag);

	void onWindowDestory();
    void showErrorInformation(QString info);
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    void changeEvent(QEvent *event);
	void addView(QTreeWidgetItem *);
	//void receiveDataFromAddUser(QString msg);
	void initStyle();
	QString getNowTime();
	void createNewWidget();
	void modifyItemName(QString ,QTreeWidgetItem * item);
	QString getParentItemVar(QTreeWidgetItem * item);
	void setItemExpandOrNot(QTreeWidgetItem *currentitem);
	void resizeEvent(QResizeEvent *event);
private:
	Ui::HealthSystem *ui;
	bool hide_head;
	QTreeWidgetItem *newUser;
	QTreeWidgetItem *temp;
	QTreeWidgetItem *deleteuserItem;
	QThread *healthThread;
	HealthReadData *healthReaddata;

	AddNewUser *addnewuser;
	ShowUser *showuesr;
	Analysis *analysis;
	WelcomeWidget *welcome;
	QStringList nameList;
	int  userCount;
	bool buttonhide;
	QHash<QString,QWidget *> hashHealth;
	QString fileIcon;
	QString openfileIcon;
	QString addIcon;
    QString reportfileIcon;
	QIcon icon;

	float mwidth;
	float mheight;
    bool tag;
};

#endif // HEALTHSYSTEM_H
