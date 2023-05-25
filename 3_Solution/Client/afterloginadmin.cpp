#include "afterloginadmin.h"
#include "ui_afterloginadmin.h"

#include <QDialog>
#include <QPixmap>
#include "createuser.h"
#include "updateuser.h"
#include "deleteuser.h"
#include <QDebug>
#include <list>
#define DEFAULT_BUFFLEN 512

afterloginadmin::afterloginadmin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::afterloginadmin)
{
    ui->setupUi(this);


}

afterloginadmin::~afterloginadmin()
{
    delete ui;
}


void afterloginadmin::on_toolButton_clicked()
{
        send(this->ConnectSocket, "logout",50, 0);
        parinte->show();
        this->deleteLater();
}



void afterloginadmin::on_creationButton_clicked()
{
        createuser* createuserobj = new createuser(this);
        createuserobj->setParent(this);
        createuserobj->ConnectSocket = ConnectSocket;
        send(ConnectSocket, "create", 50, 0);
        createuserobj ->show();


        this->hide();
}


void afterloginadmin::on_modifyButton_clicked()
{
        updateuser* updateuserobj = new updateuser(this);

        updateuserobj->setParent(this);
        updateuserobj->ConnectSocket = ConnectSocket;
        send(ConnectSocket, "update", 50, 0);

        QList<QString> stringlist;
        char numberAccounts[DEFAULT_BUFFLEN];
        recv(ConnectSocket, numberAccounts, DEFAULT_BUFFLEN, 0);

        for(int i=0; i<atoi(numberAccounts); i++)
        {
            char usernamebuffer[DEFAULT_BUFFLEN];
            char passwordbuffer[DEFAULT_BUFFLEN];
            char typebuffer[DEFAULT_BUFFLEN];
            char groupbuffer[DEFAULT_BUFFLEN];

            recv(ConnectSocket, usernamebuffer, DEFAULT_BUFFLEN, 0);
            Sleep(1);
             recv(ConnectSocket, passwordbuffer, DEFAULT_BUFFLEN, 0);
            Sleep(1);
             recv(ConnectSocket, typebuffer, DEFAULT_BUFFLEN, 0);
            Sleep(1);
              recv(ConnectSocket, groupbuffer, DEFAULT_BUFFLEN, 0);
            Sleep(1);
             QString usernamestring(usernamebuffer);
            QString passwordstring(passwordbuffer);
              QString typestring(typebuffer);
               QString groupstring(groupbuffer);

              QString finalstring = usernamestring+ ' ' + passwordstring+' '+typestring+' '+groupstring + ' ';
              stringlist.push_back(finalstring);

        }


        updateuserobj->setWidgetItems(stringlist);
        updateuserobj->setParent(this);
        updateuserobj ->show();
        this->hide();
}


void afterloginadmin::on_deleteButton_clicked()
{
        deleteuser* deleteuserobj = new deleteuser(this);

        deleteuserobj->setParent(this);
        deleteuserobj->ConnectSocket = ConnectSocket;
        send(ConnectSocket, "delete", 50, 0);

        QList<QString> stringlist;
        char numberAccounts[DEFAULT_BUFFLEN];
        recv(ConnectSocket, numberAccounts, DEFAULT_BUFFLEN, 0);
        qDebug() << atoi(numberAccounts);
        for(int i=0; i<atoi(numberAccounts); i++)
        {
              char usernamebuffer[DEFAULT_BUFFLEN];
              char passwordbuffer[DEFAULT_BUFFLEN];
              char typebuffer[DEFAULT_BUFFLEN];
              char groupbuffer[DEFAULT_BUFFLEN];

              recv(ConnectSocket, usernamebuffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, passwordbuffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, typebuffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, groupbuffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              QString usernamestring(usernamebuffer);
              QString passwordstring(passwordbuffer);
              QString typestring(typebuffer);
              QString groupstring(groupbuffer);

              QString finalstring = usernamestring+ ' ' + passwordstring+' '+typestring+' '+groupstring + ' ';
              stringlist.push_back(finalstring);

        }


        deleteuserobj->setWidgetItems(stringlist);
        deleteuserobj->setParent(this);
        deleteuserobj ->show();
        this->hide();
}

