#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "afterlogin.h"
#include "afterloginadmin.h"
#include "afterloginprofessor.h"
#include <QMainWindow>

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


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    int on_loginButton_clicked();


private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H



