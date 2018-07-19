#ifndef SHOWUSER_H
#define SHOWUSER_H

#include <QWidget>
#include <QSqlQuery>
#include <healthreaddata.h>
namespace Ui {
class ShowUser;
}

class ShowUser : public QWidget
{
    Q_OBJECT

public:
    explicit ShowUser(QWidget *parent = 0);
    ~ShowUser();
    void hideorshowButton(bool);
    void hideorshowedit(bool);
signals:
    void sendDeleteMsgTohealthSystem();
    void sendmessageFromShowUser(QString);
public slots:
    void receiveDataAndShowMessage(QString,HealthReadData *);
private slots:
    void on_cancelbutton_clicked();
    void showTextInUi(QStringList);
    void on_changebutton_clicked();
    void on_surepushButton_clicked();

private:
    Ui::ShowUser *ui;
};

#endif // SHOWUSER_H
