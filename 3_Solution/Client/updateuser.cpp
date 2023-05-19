#include "updateuser.h"
#include "ui_updateuser.h"
#include <QDialog>
#include <QListWidgetItem>
#include <vector>

#define DEFAULT_BUFFLEN 512

updateuser::updateuser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::updateuser)
{
    ui->setupUi(this);

    ui->listWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->updatewidget->hide();
  connect(ui->listWidget, &QListWidget::itemClicked, this, &updateuser::on_item_clicked);
}

void updateuser::on_item_clicked(QListWidgetItem *item)
{
    ui->updatewidget->hide();
    ui->listWidget->move(40,150);

    ui->grouplineedit->clear();
    ui->passwordlineedit->clear();

    QStringList myStringList = (item->text()).split(' '); //
    this->stringul = myStringList.at(0);
    QString type = myStringList.at(2);

    ui->updatewidget->show();
    if(type == "profesor")
    {
        ui->grouplabel->hide();
        ui->grouplineedit->hide();
    }
    if(type == "student")
    {
        ui->grouplabel->show();
        ui->grouplineedit->show();
    }

}

updateuser::~updateuser()
{
   delete ui;
}

void updateuser::setWidgetItems(QList<QString>& newqwidgetitems)
{
   for(auto& x : newqwidgetitems)
   {
      ui->listWidget->addItem(x);
   }

}



void updateuser::on_updatebutton_clicked()
{


    QString password = ui->passwordlineedit->text();
    QString group    = ui->grouplineedit->text();

    if(group == " ")
    {
      send(ConnectSocket, this->stringul.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
          send(ConnectSocket, password.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
          send(ConnectSocket, NULL, DEFAULT_BUFFLEN, 0);

    }
    else
    {

          send(ConnectSocket, this->stringul.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
          send(ConnectSocket, password.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
          send(ConnectSocket, group.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
    }
    ptr->show();
    this->deleteLater();
}


void updateuser::on_pushButton_clicked()
{
    ptr->show();
    this->deleteLater();
    send(ConnectSocket, "back", DEFAULT_BUFFLEN, 0);

}

