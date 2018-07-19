#include "addnewuser.h"
#include "ui_addnewuser.h"
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <healthsystem.h>

AddNewUser::AddNewUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddNewUser)
{
    ui->setupUi(this);
    setWindowFlags (Qt::SubWindow);
    this->setParent(parent);
}

AddNewUser::~AddNewUser()
{
    delete ui;
}
void AddNewUser::closeUserWindow(){
    close();
    qDebug()<<"closeUserWindow";

}
void AddNewUser::clearAllLineText(){
    ui->namelineEdit->clear();
    ui->agelineEdit->clear();
    ui->heightlineEdit->clear();
    ui->shebaolineEdit->clear();
}
void AddNewUser::on_sureButton_clicked()
{   if(ui->namelineEdit->text()!=NULL){
        QString sql;
        sql.append( QString("insert into health(name,age,hight,shebao) values('%1','%2','%3','%4')")
                    .arg(ui->namelineEdit->text())
                    .arg(ui->agelineEdit->text())
                    .arg(ui->heightlineEdit->text())
                    .arg(ui->shebaolineEdit->text()));
        qDebug()<<sql;
        emit sendDataToHealth(sql,ui->namelineEdit->text());
        //QMessageBox::information(this,tr("提示"),tr("添加成功"));
        clearAllLineText();
    }else{
        //QMessageBox::information(this,tr("提示"),tr("名字不能为空"));
    }
}
