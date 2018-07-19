#include "mainwindows.h"
#include "ui_mainwindows.h"
#include  "healthsystem.h"
#include <QPixmap>
MainWindows::MainWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindows)
{
    ui->setupUi(this);
    setWindowTitle(tr("\345\247\277\346\200\201\345\210\206\346\236\220\347\263\273\347\273\237 v1.0"));
    ui->label->setScaledContents(true);
    ui->label_2->setScaledContents(true);
    ui->label_3->setScaledContents(true);
    ui->label_4->setScaledContents(true);
    ui->pushButton->setStyleSheet("QPushButton:!pressed{border-image: url(:/image/btfx_btn_before.png);}"
                                  "QPushButton:pressed{border-image: url(:/image/btfx_btn_after.png);}");
}

MainWindows::~MainWindows()
{
    delete ui;
}

void MainWindows::on_pushButton_clicked()
{/*       ui->pushButton->hide();
        ui->pushButton_2->hide();
        ui->pushButton_3->hide();
        ui->pushButton_4->hide();*/


}

void MainWindows::on_pushButton_pressed()
{
//    QPixmap pixmap;
//    pixmap.load(":/image/btfx_btn_after.png");
//    ui->pushButton->setIcon(pixmap);
}

void MainWindows::on_pushButton_released()
{
    HealthSystem *w = new HealthSystem;
    w->setMinimumSize(860,580);
    w->show();
    this->close();
}
