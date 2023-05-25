#ifndef AFTERLOGINPROFESSOR_H
#define AFTERLOGINPROFESSOR_H
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <QMainWindow>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
using namespace std;

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


namespace Ui {
class afterloginprofessor;
}

class afterloginprofessor : public QMainWindow
{
    Q_OBJECT

public:
    SOCKET ConnectSocket;
    explicit afterloginprofessor(QWidget *parent = nullptr, const QString& _username= " ");
    ~afterloginprofessor();
    void setParent(QMainWindow* newparent) {parinte = newparent;}
    void setSocket(SOCKET newsocket) {this->ConnectSocket = newsocket;}


private slots:
    void on_logoutButton_clicked();

    void on_createButton_clicked();

    void on_viewButton_clicked();

private:
    QMainWindow* parinte;
    QString username;
    Ui::afterloginprofessor *ui;
};

#endif // AFTERLOGINPROFESSOR_H
