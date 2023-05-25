#include "createhomework.h"
#include "ui_createhomework.h"

#define DEFAULT_BUFFLEN 512
#include <QDate>

createhomework::createhomework(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::createhomework)
{
    ui->setupUi(this);

    ui->calendarWidget->hide();
}

createhomework::~createhomework()
{
    delete ui;
}

void createhomework::on_backButton_clicked()
{
    ptr->show();
    this->deleteLater();
    send(ConnectSocket, "back", DEFAULT_BUFFLEN, 0);
}


void createhomework::on_dateButton_clicked()
{
    ui->calendarWidget->show();
}

void createhomework::on_calendarWidget_selectionChanged()
{
    QDate data = ui->calendarWidget->selectedDate();
    ui->dateEdit->setDate(data);
    ui->calendarWidget->hide();

    date =  data.toString("yyyy-MM-dd");
}


void createhomework::on_createButton_clicked()
{
    QString title = ui->titlelineedit->text();
    QString group  = ui->grouplineedit ->text();


    send(ConnectSocket, group.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, date.toLocal8Bit(), DEFAULT_BUFFLEN, 0);
    send(ConnectSocket, title.toLocal8Bit(), DEFAULT_BUFFLEN, 0);

    ptr->show();
    this->deleteLater();
}

