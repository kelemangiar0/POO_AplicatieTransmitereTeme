#ifndef AFTERLOGINADMIN_H
#define AFTERLOGINADMIN_H

#include <QMainWindow>
#include <QLabel>
#include "mainwindow.h"

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
using namespace std;

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


namespace Ui {
class afterloginadmin;
}

class afterloginadmin : public QMainWindow
{
    Q_OBJECT

public:
    explicit afterloginadmin(QWidget *parent = nullptr);
    void setParent(QMainWindow* newparent) {parinte = newparent;}
    void setSocket(SOCKET newsocket) {this->ConnectSocket = newsocket;}
    ~afterloginadmin();
    SOCKET ConnectSocket;
private:
    Ui::afterloginadmin *ui;
    QMainWindow* parinte;
private slots:
    void on_toolButton_clicked();

    void on_creationButton_clicked();
    void on_modifyButton_clicked();
    void on_deleteButton_clicked();
};

#endif // AFTERLOGINADMIN_H
