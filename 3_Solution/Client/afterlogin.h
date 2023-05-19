#ifndef AFTERLOGIN_H
#define AFTERLOGIN_H

#include <QMainWindow>
#include <QLabel>
#include "mainwindow.h"
#include <QListWidget>
#include <QList>
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
class afterlogin;
}

class afterlogin : public QMainWindow
{
    Q_OBJECT
private:
    Ui::afterlogin *ui;
    QMainWindow* parinte;
    QString m_username;
    QString m_filePath;
    QList<QString> homeworksdetails;


  public:
    SOCKET ConnectSocket;
    explicit afterlogin(QWidget *parent = nullptr);
    ~afterlogin();
    void setUsername(QString newusername);
    void setParent(QMainWindow* newparent) {parinte = newparent;}
    void setSocket(SOCKET newsocket) {this->ConnectSocket = newsocket;}

    void addTitlesToWidget(QList<QString>& obj);
    void addDetails(QString detail) {homeworksdetails.push_back(detail);}
private slots:


    void on_toolButton_clicked();


    void on_homeworksWidget_itemClicked(QListWidgetItem *item);
    void on_chooseButton_clicked();
    void on_uploadButton_clicked();
};

#endif // AFTERLOGIN_H
