#ifndef VIEWHOMEWORK_H
#define VIEWHOMEWORK_H

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <QMainWindow>
#include "QListWidget"
#include "afterloginprofessor.h"
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
class viewhomework;
}

class viewhomework : public QMainWindow
{
    Q_OBJECT

public:
    explicit viewhomework(QWidget *parent, QList<QString>& list);
    ~viewhomework();
    void setWidgetItems(QList<QString>& newqwidgetitems, QListWidget*& listwidgetptr);
    void setParent(afterloginprofessor* newptr) {ptr = newptr;}
    SOCKET ConnectSocket;

private slots:
    void on_homeworksWidget_itemClicked(QListWidgetItem *item);
    void on_studentsWidget_itemClicked(QListWidgetItem *item);
    void on_backButton_clicked();
    void on_downloadButton_clicked();
    void on_markButton_clicked();
    void on_reportButton_clicked();

private:
        void setHomeworkList(QList<QString>& list);
    Ui::viewhomework *ui;
    QList<QString> homeworkslist;
    afterloginprofessor* ptr;
    QList<QString>* studentsptr;
    QString homeworkID;
    QString username;
};

#endif // VIEWHOMEWORK_H
