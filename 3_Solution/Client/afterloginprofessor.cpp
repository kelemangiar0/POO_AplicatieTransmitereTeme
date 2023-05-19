#include "afterloginprofessor.h"
#include "ui_afterloginprofessor.h"
#include <QDateTime>
#include "createhomework.h"
#include "viewhomework.h"
#define DEFAULT_BUFFLEN 512

afterloginprofessor::afterloginprofessor(QWidget *parent, const QString& _username) :
    QMainWindow(parent),
    ui(new Ui::afterloginprofessor), username(_username)
{     ui->setupUi(this);

    QString message("Welcome back, ");
    message += username;
    ui->welcomeLabel->setText(message);


}

afterloginprofessor::~afterloginprofessor()
{
    delete ui;
}

void afterloginprofessor::on_logoutButton_clicked()
{
    send(this->ConnectSocket, "logout",50, 0);
    parinte->show();
    this->deleteLater();
}


void afterloginprofessor::on_createButton_clicked()
{
    createhomework* createhomeworkobj = new createhomework(this);
    createhomeworkobj->setParent(this);
    createhomeworkobj->ConnectSocket = ConnectSocket;
    send(ConnectSocket, "create", 50, 0);
    createhomeworkobj ->show();


    this->hide();
}


void afterloginprofessor::on_viewButton_clicked()
{

    send(ConnectSocket, "viewStudents", 50, 0);
    QList<QString> homeworkslist;
    char numberHomeworks[DEFAULT_BUFFLEN];
    recv(ConnectSocket, numberHomeworks, DEFAULT_BUFFLEN, 0);
    for(int i = 0; i < atoi(numberHomeworks); i++)
    {
        char idbuffer[DEFAULT_BUFFLEN];
        char groupbuffer[DEFAULT_BUFFLEN];
        char termbuffer[DEFAULT_BUFFLEN];
        char titlebuffer[DEFAULT_BUFFLEN];

        recv(ConnectSocket, idbuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, groupbuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, termbuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);
        recv(ConnectSocket, titlebuffer, DEFAULT_BUFFLEN, 0);
        Sleep(1);

        QString idstring(idbuffer);
        QString groupstring(groupbuffer);
        QString termbufferstring(termbuffer);
        QString titlestring(titlebuffer);


        QDateTime futureDateTime = QDateTime::fromString(termbufferstring, "yyyy-MM-dd");

        QDate currentDate = QDate::currentDate();

        QDate futureDate = futureDateTime.date();

        qint64 daysDifference = currentDate.daysTo(futureDate);

        QString daysDifferenceString = QString::number(daysDifference) + " days left";
        QString finalstring = idstring + ' ' + groupstring +  ' ' + titlestring + ' ' + daysDifferenceString;
        homeworkslist.push_back(finalstring);
    }
    viewhomework* viewhomeworkobj = new viewhomework(this,homeworkslist);
    viewhomeworkobj->setParent(this);
    viewhomeworkobj->ConnectSocket = ConnectSocket;


    viewhomeworkobj ->show();
    this->hide();
}

