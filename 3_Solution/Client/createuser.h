#ifndef CREATEUSER_H
#define CREATEUSER_H
#include "afterloginadmin.h"
#include <QMainWindow>
#include <QLineEdit>

#include <QLabel>
#include "mainwindow.h"
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
class createuser;
}

class createuser : public QMainWindow
{
    Q_OBJECT

public:
    explicit createuser(QWidget *parent = nullptr);
    void setParent(afterloginadmin* newptr) {ptr = newptr;}
    ~createuser();
    SOCKET ConnectSocket;
private slots:
    void on_pushButton_clicked();


    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_registerButton_clicked();

private:
    Ui::createuser *ui;
    afterloginadmin* ptr;

    QLabel* m_groupLabel;
    QLineEdit* m_groupLineEdit;
    QLabel* m_nameLabel;
    QLineEdit* m_nameLineEdit;
    QLabel* m_passwordLabel;
    QLineEdit* m_passwordLineEdit;

    QString* studentname;
    QString* studentgroup;
    QString* studentpassword;

    QString* professorname;
    QString* professorpassword;

};

#endif // CREATEUSER_H
