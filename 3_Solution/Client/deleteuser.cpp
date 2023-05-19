#include "deleteuser.h"
#include "ui_deleteuser.h"
#include <QDialog>
#include <QListWidgetItem>
#define DEFAULT_BUFFLEN 512

deleteuser::deleteuser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deleteuser)
{
    ui->setupUi(this);
    ui->listWidget->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->deleteWidget->hide();

    connect(ui->listWidget, &QListWidget::itemClicked, this, &deleteuser::on_item_clicked);
}

deleteuser::~deleteuser()
{
    delete ui;
}

void deleteuser::on_pushButton_clicked()
{
    ptr->show();
    this->deleteLater();
     send(ConnectSocket, "back", DEFAULT_BUFFLEN, 0);
}

void deleteuser::on_item_clicked(QListWidgetItem *item)
{
    ui->deleteWidget->hide();
    ui->listWidget->move(40,150);

    QStringList myStringList = (item->text()).split(' '); //
    this->stringul = myStringList.at(0);


    ui->deleteWidget->show();
}

void deleteuser::on_deleteButton_clicked()
{
    send(ConnectSocket, this->stringul.toLocal8Bit(), DEFAULT_BUFFLEN, 0);

    ptr->show();
    this->deleteLater();
}

void deleteuser::setWidgetItems(QList<QString>& newqwidgetitems)
{
   for(auto& x : newqwidgetitems)
   {
        ui->listWidget->addItem(x);
   }


}





