#ifndef UPDATEUSER_H
#define UPDATEUSER_H
#include "afterloginadmin.h"
#include <QMainWindow>
#include <qlistwidget.h>
#include <vector>


#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace Ui {
class updateuser;
}

class updateuser : public QMainWindow
{
    Q_OBJECT

public:
    explicit updateuser(QWidget *parent = nullptr);
    ~updateuser();
    void setWidgetItems(QList<QString>& newqwidgetitems);

    void setParent(afterloginadmin* newptr) {ptr = newptr;}

    SOCKET ConnectSocket;

private slots:
    void on_item_clicked(QListWidgetItem *item);


    void on_updatebutton_clicked();

    void on_pushButton_clicked();

private:
    afterloginadmin* ptr;
    Ui::updateuser *ui;
    QString stringul;

};

#endif // UPDATEUSER_H
