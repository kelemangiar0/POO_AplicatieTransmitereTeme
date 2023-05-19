#ifndef DELETEUSER_H
#define DELETEUSER_H

#include <QMainWindow>
#include "afterloginadmin.h"
#include <qlistwidget.h>
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



namespace Ui {
class deleteuser;
}

class deleteuser : public QMainWindow
{
    Q_OBJECT

public:
    explicit deleteuser(QWidget *parent = nullptr);
    ~deleteuser();
    SOCKET ConnectSocket;
     void setWidgetItems(QList<QString>& newqwidgetitems);
    void setParent(afterloginadmin* newptr) {ptr = newptr;}

private slots:
    void on_pushButton_clicked();
    void on_item_clicked(QListWidgetItem *item);




    void on_deleteButton_clicked();

private:
    Ui::deleteuser *ui;
    afterloginadmin* ptr;
     QString stringul;
};

#endif // DELETEUSER_H
