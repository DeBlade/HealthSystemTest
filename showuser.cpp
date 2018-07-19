#include "showuser.h"
#include "ui_showuser.h"
#include <QDebug>
#include <QSqlQuery>
#include <healthreaddata.h>

ShowUser::ShowUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowUser)
{
    ui->setupUi(this);
    this->setParent(parent);
    ui->widgetLabel->show();
    ui->widget_edit->hide();
    ui->surepushButton->hide();
}
void ShowUser::hideorshowButton(bool type){
    if(type){
        ui->labelname->hide();
        ui->labelage->hide();
        ui->labelheight->hide();
        ui->labelshebao->hide();
    }else{
        ui->labelname->show();
        ui->labelage->show();
        ui->labelheight->show();
        ui->labelshebao->show();
    }
}
void ShowUser::hideorshowedit(bool type){
    if(type){
        ui->lineEdit->hide();
        ui->lineEdit_2->hide();
        ui->lineEdit_3->hide();
        ui->lineEdit_4->hide();
    }else{
        ui->lineEdit->show();
        ui->lineEdit_2->show();
        ui->lineEdit_3->show();
        ui->lineEdit_4->show();
    }
}

void ShowUser::receiveDataAndShowMessage(QString msg, HealthReadData *readData)
{
      connect(this,&ShowUser::sendmessageFromShowUser,readData,&HealthReadData::getShowUserMessageFromDatabase);
      connect(readData,&HealthReadData::sendQueryToShowUser,this,&ShowUser::showTextInUi);
      emit sendmessageFromShowUser(msg);
}
void ShowUser::showTextInUi(QStringList list){

    for(int i =0;i<list.size();i++)
    {
        switch (i) {
        case 0:
            if(list[i]!=NULL){
               ui->labelname->setText(list[i]);
            }else{
               ui->labelname->setText("");
            }
            break;
        case 1:
            if(list[i]!=NULL){
               ui->labelage->setText(list[i]);
            }else{
               ui->labelage->setText("");
            }
            break;
        case 2:
            if(list[i]!=NULL){
               ui->labelheight->setText(list[i]);
            }else{
               ui->labelheight->setText("");
            }
            break;
        case 3:
            if(list[i]!=NULL){
               ui->labelshebao->setText(list[i]);
            }else{
               ui->labelshebao->setText("");
            }
            break;
        default:
            break;
        }
    }
}
ShowUser::~ShowUser()
{
    delete ui;
}

void ShowUser::on_cancelbutton_clicked()
{
     emit sendDeleteMsgTohealthSystem();
}



void ShowUser::on_changebutton_clicked()
{
    ui->changebutton->hide();
    ui->surepushButton->show();
    ui->widgetLabel->hide();
    ui->widget_edit->show();

}

void ShowUser::on_surepushButton_clicked()
{

}

