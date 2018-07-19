#ifndef ADDNEWUSER_H
#define ADDNEWUSER_H

#include <QWidget>

namespace Ui {
class AddNewUser;
}

class AddNewUser : public QWidget
{
    Q_OBJECT

public:
    explicit AddNewUser(QWidget *parent = 0 );
    ~AddNewUser();
private:
    void closeUserWindow();
    void clearAllLineText();
signals:
    void sendDataToHealth(QString,QString);
private slots:
    void on_sureButton_clicked();

private:
    Ui::AddNewUser *ui;
};

#endif // ADDNEWUSER_H
