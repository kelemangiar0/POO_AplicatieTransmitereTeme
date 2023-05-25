#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <ui_mainwindow.h>
#include <QEvent>
#include <QLineEdit>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "updateuser.h"
#include "viewhomework.h"
#include <iostream>


using namespace std;



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFFLEN 512
#define DEFAULT_PORT "27015"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLineEdit *userlineEdit = ui->userlineEdit;
    QLineEdit *passlineEdit = ui->passlineEdit;

    connect(userlineEdit, &QLineEdit::selectionChanged, userlineEdit, &QLineEdit::clear);
    connect(passlineEdit, &QLineEdit::selectionChanged, passlineEdit, &QLineEdit::clear);


}

MainWindow::~MainWindow()
{
    delete ui;
}
SOCKET initialiseConnection(int argc, char** argv)
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL, * ptr = NULL, hints;
    int iResult;


    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        exit(1);
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            exit(1);
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }

    return ConnectSocket;
}



int MainWindow::on_loginButton_clicked()
{
    afterlogin *studentafter = nullptr;
    afterloginadmin *adminafter = nullptr;
    afterloginprofessor *professorafter = nullptr;


    bool option = false; //pt testare
    if (option == true)
    {
        afterlogin* obj = new afterlogin(this);

        obj->setParent(this);
        this->hide();
        obj->show();

        return 0;
    }



    printf("CLIENT\n\n\n");

    SOCKET ConnectSocket = INVALID_SOCKET;


    char** vec = new char*[2];
    for(int i = 0 ; i<2; i++)
     vec[i] = new char[20];
    strcpy(vec[0], "0");
    strcpy(vec[1], "192.168.8.103");
    ConnectSocket = initialiseConnection(2, vec);

    for(int i  = 0; i<2; i++)
     delete[] vec[i];
    delete[] vec;
    int iResult = 0;
    char serverResponse[DEFAULT_BUFFLEN];


    QString username1 = ui->userlineEdit->text();
    QString password1 = ui->passlineEdit->text();

    send(ConnectSocket, username1.toLocal8Bit(), username1.size()+1, 0);
    send(ConnectSocket, password1.toLocal8Bit(), password1.size()+1, 0);


    recv(ConnectSocket, serverResponse, DEFAULT_BUFFLEN, 0);

    if (strcmp(serverResponse, "studentLogin") == 0)
    {
        cout << "Welcome, student!" << endl;
        studentafter = new afterlogin(this);
        studentafter->setParent(this);
        // numar studenti, iar pt fiecare idtema, titlu, grupa, termen limita, status (Complet sau Incomplet)
        QList<QString> titleslist;
        char numberStudents[DEFAULT_BUFFLEN];
        recv(ConnectSocket, numberStudents, DEFAULT_BUFFLEN, 0);

        char homeworkid_buffer[DEFAULT_BUFFLEN];
        char title_buffer[DEFAULT_BUFFLEN];
        char group_buffer[DEFAULT_BUFFLEN];
        char term_buffer[DEFAULT_BUFFLEN];
        char status_buffer[DEFAULT_BUFFLEN];
        char grade_buffer[DEFAULT_BUFFLEN];
        for(int i = 0; i < atoi(numberStudents); i++)
        {

              recv(ConnectSocket, homeworkid_buffer, DEFAULT_BUFFLEN, 0);
             Sleep(1);
              recv(ConnectSocket, title_buffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, group_buffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, term_buffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, status_buffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              recv(ConnectSocket, grade_buffer, DEFAULT_BUFFLEN, 0);
              Sleep(1);
              QString homeworkid_string(homeworkid_buffer);
              QString title_string(title_buffer);
              QString group_string(group_buffer);
              QString term_string(term_buffer);
              QString status_string(status_buffer);
              titleslist.push_back(title_string);
              QString grade_string(grade_buffer);

              QString finalstring = homeworkid_string + ' ' + title_string + ' ' + group_string + ' ' + term_string + ' ' + status_string + ' ' + grade_string;
              studentafter->addDetails(finalstring);

        }


        studentafter->addTitlesToWidget(titleslist);
        studentafter->setUsername(ui->userlineEdit->text());
        studentafter->ConnectSocket = ConnectSocket;
        this->hide();

        studentafter->show();
    }
    if (strcmp(serverResponse, "profesorLogin") == 0)
    {

        cout << "Welcome, profesor!" << endl;

        professorafter = new afterloginprofessor(this,username1);
        professorafter->setParent(this);
        professorafter->ConnectSocket = ConnectSocket;
        this->hide();

        professorafter->show();


    }
    if (strcmp(serverResponse, "adminLogin") == 0)
    {
        cout << "Welcome, admin!" << endl;

        adminafter = new afterloginadmin(this);
        adminafter->setParent(this);
        adminafter->ConnectSocket = ConnectSocket;
        this->hide();

        adminafter->show();
    }

    if (strcmp(serverResponse, "incorrectPassword") == 0)
    {
        cout << "Incorrect password! Try again..." << endl;

    }
    if (strcmp(serverResponse, "userNotFound") == 0)
    {
        cout << "User not into database. Contact admin, then try again..." << endl;
    }



    return 0;
}



