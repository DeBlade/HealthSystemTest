#include "healthsystem.h"
#include "ui_healthsystem.h"
#include <QTreeWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QSqlError>
#include <addnewuser.h>
#include <showreport.h>
#include <showuser.h>
#include <QFile>
#include <QMenu>
#include <QThread>
#include <QMetaType>
#include <QDateTime>


HealthSystem::HealthSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HealthSystem)
{
    ui->setupUi(this);
    setWindowTitle(tr("\345\247\277\346\200\201\345\210\206\346\236\220\347\263\273\347\273\237 v1.0"));
    qRegisterMetaType <IDNAMELIST>("IDNAMELIST");
    //初始化界面上的button
    initStyle();
    //创建新的widget
    createNewWidget();

    ui->stackedWidget->setParent(this);
//	//关闭窗口 销毁线程
    connect(this,&HealthSystem::destroyed,this,&HealthSystem::onWindowDestory);

    connect(this,&HealthSystem::createDatabase,healthReaddata,&HealthReadData::createTable);
    connect(healthReaddata,&HealthReadData::sendErrorInformation,this,&HealthSystem::showErrorInformation);
    emit createDatabase();

    //查询数据库中所有用户
    connect(healthReaddata,&HealthReadData::sendAllUserFromSql,this,&HealthSystem::AddTreeRoot);

    connect(this, &HealthSystem::sendSignalToHealth, healthReaddata, &HealthReadData::getAllUserName);
    //将用户数据插入到数据库中
    connect(this,&HealthSystem::insertUserMesageToDatabase,healthReaddata,&HealthReadData::insertNewUser);

    //数据库返回的刚插入的ID 和 name 在item 中增加
    connect(healthReaddata,&HealthReadData::sendNewUseMesgToAddItem,this,&HealthSystem::addnewUserItem);

    //用户添加界面返回的sql语句
    connect(addnewuser,&AddNewUser::sendDataToHealth,this,&HealthSystem::addnewUserSql);

    //refresh new recoder
    connect(this,&HealthSystem::updateNewRecoder,healthReaddata,&HealthReadData::updateRecoder);
    //数据库删除用户
    connect(this,&HealthSystem::deleteUserFromDatabase,healthReaddata,&HealthReadData::deleteFromDatabase);

    connect(showuesr,&ShowUser::sendDeleteMsgTohealthSystem,this,&HealthSystem::deletefromshowuser);

    connect(analysis,&Analysis::sendMessageToShowReport,this,&HealthSystem::ShowHealthReport);

    connect(this,&HealthSystem::sentMsgToShowUser,showuesr,&ShowUser::receiveDataAndShowMessage);

    connect(this,&HealthSystem::sendMessageToAnalysis,analysis,&Analysis::receiveMessaeFromhealth);

    connect(ui->treeWidget,&QTreeWidget::itemClicked, this, &HealthSystem::receiveSignalItem);

    connect(this,&HealthSystem::toResetAnalysisBoxsize,analysis,&Analysis::resetGroupBoxSize);

    connect(this,&HealthSystem::currentChangeToresetChart,analysis,&Analysis::restShowChartView);
    // 发信号读取数据库中所有的用户并显示到treewidget
    emit sendSignalToHealth();
}

//创建读取数据库的线程 增加新用户 预览用户和 分析数据的界面
void HealthSystem::createNewWidget()
{
    healthThread = new QThread(this);

    healthReaddata = new HealthReadData;
    healthReaddata->moveToThread(healthThread);
    healthThread->start();

    newUser = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("\346\267\273\345\212\240\346\226\260\347\224\250\346\210\267")));
    QVariant var0(0);
    newUser->setData(0,Qt::UserRole,var0);
    newUser->setIcon(0,QIcon(addIcon));

    addnewuser = new AddNewUser(ui->stackedWidget);
    showuesr = new ShowUser(ui->stackedWidget);
    analysis = new Analysis(ui->stackedWidget,healthReaddata);

    welcome = new WelcomeWidget;

    ui->stackedWidget->addWidget(addnewuser);
    ui->stackedWidget->addWidget(analysis);
    ui->stackedWidget->addWidget(showuesr);
    ui->stackedWidget->addWidget(welcome);
    ui->stackedWidget->setCurrentWidget(welcome);
}

//初始化各个widget button 的风格
void HealthSystem::initStyle()
{
    ui->widget->setStyleSheet("background-color:#334960;");
    ui->treeWidget->setStyleSheet("font: 10pt;background-color:#334960;color: rgb(255, 255, 255);");
    ui->leftwidget->setStyleSheet("background-color:#334960;");
    ui->leftwidget->hide();

    QPixmap pixmap0;
    pixmap0.load("://image/guardicon.png");
    ui->gorightbutton->setIcon(pixmap0);
    ui->gorightbutton->hide();

    QPixmap pixmap;
    pixmap.load("://image/guidicon.png");
    ui->pushButton->setIcon(pixmap);

    fileIcon = "://image/file.png";
    openfileIcon = "://image/openfile.png";
    addIcon = "://image/addicon.png";
    reportfileIcon = "://image/report.png";
    ui->treeWidget->setHeaderHidden(true);

}

//从treewidget上删除显示的用户
void HealthSystem::deletefromshowuser(){
    qDebug()<<"deletefromshowuser";
    deletefromtreewidget();
}

void HealthSystem::resizeEvent(QResizeEvent *event)
{
    qDebug()<<"HealthSystem resizeEvent"<<ui->stackedWidget->size();

    //emit toResetAnalysisBoxsize(ui->stackedWidget->size());
}

//界面最大化后 重设控件大小
void HealthSystem::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange) return;
    if(this->windowState()==Qt::WindowMaximized)
    {
        qDebug()<<"HealthSystem changeEvent WindowMaximized"<<ui->stackedWidget->size();
        emit toResetAnalysisBoxsize(ui->stackedWidget->size());

    }
}

//点击item 响应的函数
void HealthSystem::receiveSignalItem(QTreeWidgetItem * item,int x){
        deleteuserItem = item;
        qDebug()<<"receiveSignalItem "<<x;
        addView(item);
}

void HealthSystem::ShowHealthReport(QString id,bool tag)
{

    ShowReport *report = new ShowReport(ui->stackedWidget,id,healthReaddata);
    report->show();
    QTreeWidgetItem *currentitem = deleteuserItem;
    while(1)
    {
        QTreeWidgetItem *swapitem = currentitem->parent();
        if(!swapitem)
        {
            break;
        }else{
           currentitem = swapitem->parent();
        }
    }
    setItemExpandOrNot(currentitem);
    if (tag){
        modifyItemName("result",currentitem->child(1)->child(0));
    }
    ui->stackedWidget->addWidget(report);
    ui->stackedWidget->setCurrentWidget(report);

}

void HealthSystem::setItemExpandOrNot(QTreeWidgetItem *currentitem)
{
    if(currentitem->child(0)->isExpanded())
    {
        currentitem->child(0)->child(0)->setSelected(false);
    }
    currentitem->child(1)->setExpanded(true);
    currentitem->child(1)->child(0)->setSelected(true);
}


void HealthSystem::addView(QTreeWidgetItem * item){
     QVariant var = item->data(0,Qt::UserRole);
     QVariant var1 = item->data(2,Qt::UserRole);
     if(var==0)
     {
         ui->stackedWidget->setCurrentWidget(addnewuser);
         analysis->hide();
         showuesr->hide();
         welcome->hide();

     }
     else if(var==3)
     {
          qDebug()<<"sendMessageToAnalysis click";
          modifyItemName("newrecoder",item);
          emit sendMessageToAnalysis(item);
          ui->stackedWidget->setCurrentWidget(analysis);

     }
     else if(var==1)
     {
         emit sentMsgToShowUser(var1.toString(), healthReaddata);
         ui->stackedWidget->setCurrentWidget(showuesr);
     }
     else if(var==5)
     {
        QVariant var2 = item->parent()->parent()->data(2,Qt::UserRole);
        ShowHealthReport(var2.toString(),false);
     }
}

QString HealthSystem::getNowTime(){
    QDateTime time = QDateTime::currentDateTime();

    QString str = time.toString("yyyyMMddhh");
    qDebug()<<"getNowTime"<<str;
    return str;
}

QString HealthSystem::getParentItemVar(QTreeWidgetItem * item)
{
     QTreeWidgetItem * parentItem =item->parent();
    if(parentItem)
    {
     QVariant var = parentItem->data(0,Qt::UserRole);
     while(var!=1)
     {
         item = parentItem->parent();
         parentItem = item;
         var = parentItem->data(0,Qt::UserRole);
         qDebug()<<"getParentItem"<<var;
     }
     qDebug()<<"parentItem 3 "<<parentItem->data(2,Qt::UserRole);
     return parentItem->data(2,Qt::UserRole).toString();
    }
    else
    {
        return item->data(2,Qt::UserRole).toString();
    }
}

void HealthSystem::modifyItemName(QString type,QTreeWidgetItem * item)
{
     QVariant var = item->data(2,Qt::UserRole);
     if (item == Q_NULLPTR)
     {
             return;
     }
     if(var.toString()=="999999")
     {   QString time = getNowTime();
         for(int i=0;i<item->columnCount();i++)
         {
             item->setText(i,time);
         }
         emit updateNewRecoder(type,time,getParentItemVar(item));
     }
}
void HealthSystem::addnewUserSql(QString msg,QString name)
{
    emit insertUserMesageToDatabase(msg,name);
}

void HealthSystem::addnewUserItem(IDNAMELIST msg)
{
    AddTreeRoot(msg);
}

void HealthSystem::AddTreeRoot(struct IDNAMELIST list)
{
    for(int i=0;i<list.nameList.size();i++){
        QTreeWidgetItem * item=new QTreeWidgetItem(ui->treeWidget,QStringList(QString(list.nameList[i])));
        item->setIcon(0,QIcon(fileIcon));
        ui->treeWidget->addTopLevelItem(item);
        QVariant var1(1),var11(list.idList[i].toInt());
        item->setData(0,Qt::UserRole,var1);
        item->setData(2,Qt::UserRole,var11);

        QTreeWidgetItem * item1=new QTreeWidgetItem(QStringList(QString("\346\243\200\346\237\245\350\256\260\345\275\225")));
        item1->setIcon(0,QIcon(fileIcon));
        QVariant var2(2);
        item1->setData(0,Qt::UserRole,var2);
        QString newRecoder;
        if(list.newRecoderList[i]=="999999")
        {
           newRecoder = "\346\226\260\345\273\272\350\256\260\345\275\225";
        }
        else
        {
           newRecoder=list.newRecoderList[i];
        }
        QTreeWidgetItem * item1_1=new QTreeWidgetItem(QStringList(newRecoder));
        item1_1->setIcon(0,QIcon(reportfileIcon));
        QVariant var3(3),var3_1(list.newRecoderList[i].toInt());
        item1_1->setData(0,Qt::UserRole,var3);
        item1_1->setData(2,Qt::UserRole,var3_1);

        QTreeWidgetItem * item2=new QTreeWidgetItem(QStringList(QString("\346\243\200\346\265\213\346\212\245\345\221\212")));
        item2->setIcon(0,QIcon(fileIcon));
        QVariant var4(4);
        item2->setData(0,Qt::UserRole,var4);
        QString result;
        if(list.resultList[i]=="999999")
        {
           result = "\346\243\200\346\265\213\347\273\223\346\236\234";
        }
        else
        {
           result=list.resultList[i];
        }
        QTreeWidgetItem * item2_1=new QTreeWidgetItem(QStringList(result));
        item2_1->setIcon(0,QIcon(reportfileIcon));
        QVariant var5(5),var5_1(list.resultList[i].toInt());
        item2_1->setData(0,Qt::UserRole,var5);
        item2_1->setData(2,Qt::UserRole,var5_1);

        item->addChild(item1);
        item1->addChild(item1_1);
        item->addChild(item2);
        item2->addChild(item2_1);
    }
}

HealthSystem::~HealthSystem()
{
    delete ui;
}

void HealthSystem::on_pushButton_clicked()
{
    ui->label->hide();
    ui->treeWidget->hide();
    ui->pushButton->hide();
    ui->widget->hide();
    ui->leftwidget->show();
    ui->gorightbutton->show();
}

void HealthSystem::on_gorightbutton_clicked()
{
    ui->label->show();
    ui->treeWidget->show();
    ui->pushButton->show();
    ui->widget->show();
    ui->leftwidget->hide();
    ui->gorightbutton->hide();
}

void HealthSystem::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    QVariant var = item->data(0,Qt::UserRole);
    if(var ==1){
        item->setIcon(0,QIcon(openfileIcon));
    }
}

void HealthSystem::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{
    QVariant var = item->data(0,Qt::UserRole);
    if(var ==1){
        item->setIcon(0,QIcon(fileIcon));
    }

}

void HealthSystem::deletefromtreewidget(){
   if (deleteuserItem == NULL) return;

    QVariant var1 = deleteuserItem->data(2,Qt::UserRole);
    emit deleteUserFromDatabase(var1.toString());
    deletefromwidget();
}
void HealthSystem::deletefromwidget(){
    if(deleteuserItem==Q_NULLPTR)
    {
        return;
    }
    if(deleteuserItem->parent()==Q_NULLPTR)
    {
        delete ui->treeWidget->takeTopLevelItem(ui->treeWidget->currentIndex().row());
    }
    else
    {
        delete deleteuserItem->parent()->takeChild(ui->treeWidget->currentIndex().row());
    }
    deleteuserItem =NULL;
}

void HealthSystem::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* curItem=ui->treeWidget->itemAt(pos);
        deleteuserItem = curItem;
        if(curItem==NULL)return;
        QVariant var = curItem->data(0,Qt::UserRole);
        if(1 == var)
        {
           QMenu *popMenu =new QMenu(this);
           popMenu->addAction("删除此条记录",this,&HealthSystem::deletefromtreewidget);
           popMenu->setStyleSheet("background-color:#334960;font: 10pt;color: rgb(255, 255, 255);");
           popMenu->exec(QCursor::pos());
        }
}

void HealthSystem::onWindowDestory()
{
   healthThread->quit();
   healthThread->wait();
   delete healthThread;
   healthThread = NULL;
}

void HealthSystem::showErrorInformation(QString info){
   QMessageBox::information(this,tr("提示"),info);
}

void HealthSystem::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
     QTreeWidgetItem *mprevious = previous;
     if(!mprevious)
     {
         previous = current;
     }

    if(getParentItemVar(current)!=getParentItemVar(previous)){
        emit currentChangeToresetChart();
    }
}
