#ifndef CREATEHOMEWORK_H
#define CREATEHOMEWORK_H
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include "afterloginprofessor.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <QMainWindow>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


namespace Ui {
class createhomework;
}

class createhomework : public QMainWindow
{
    Q_OBJECT

public:
    SOCKET ConnectSocket;
    explicit createhomework(QWidget *parent = nullptr);
    void setParent(afterloginprofessor* newparent) {ptr = newparent;}
    void setSocket(SOCKET newsocket) {this->ConnectSocket = newsocket;}
    ~createhomework();

private slots:
    void on_backButton_clicked();
    void on_dateButton_clicked();
    void on_calendarWidget_selectionChanged();
    void on_createButton_clicked();

private:
    afterloginprofessor* ptr;
    Ui::createhomework *ui;
    QString date;

};

#endif // CREATEHOMEWORK_H
