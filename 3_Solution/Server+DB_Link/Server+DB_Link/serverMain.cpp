#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
//#include <stdlib.h>

//#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include "Database.h"

using namespace std;

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

string connection_string = "Driver={ODBC Driver 17 for SQL Server};Server=localhost;Database=CampusConnectDB;Trusted_Connection=yes;";
Database db(connection_string);

void createUser(SOCKET* AcceptSocket)
{
    char username[DEFAULT_BUFLEN], password[DEFAULT_BUFLEN], type[DEFAULT_BUFLEN], grupa[DEFAULT_BUFLEN];
    recv(*AcceptSocket, username, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, password, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, type, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, grupa, DEFAULT_BUFLEN, 0);


    string standardGrupa = grupa;
    for (auto& c : standardGrupa) {
        c = std::toupper(c);
    }
  
    User newUser{ username,password,type,standardGrupa.c_str()};
    if (db.createUser(newUser)) {
        cout << "Admin created user..." << endl;
    }
    else {
        std::cout << "Error creating user.\n";
    }
}
void deleteUser(SOCKET* AcceptSocket)
{
    char username[DEFAULT_BUFLEN];
    recv(*AcceptSocket, username, DEFAULT_BUFLEN, 0);
    if (db.deleteUser(username)) {
        std::cout << "User deleted successfully.\n";
    }
    else {
        std::cout << "Error deleting user.\n";
    }
}

void createHomework(SOCKET* AcceptSocket)
{
    char grupa[DEFAULT_BUFLEN], data[DEFAULT_BUFLEN];
    recv(*AcceptSocket, grupa, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, data, DEFAULT_BUFLEN, 0);

    string standardGrupa = grupa;
    for (auto& c : standardGrupa)
        c = std::toupper(c);

    Homework newHomework{0, standardGrupa.c_str(), data };
    db.createHomework(newHomework);
}
DWORD WINAPI ProcessClient(LPVOID lpParameter)
{
    SOCKET AcceptSocket = (SOCKET)lpParameter;

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    getpeername(AcceptSocket, (SOCKADDR*)&addr, &addrlen);
    char* ip = inet_ntoa(addr.sin_addr);
    cout << ip << " (" << AcceptSocket << ") connected..." << endl;

    char serverUser[50];
    char serverPassword[50];
    int iResult;

again:
    iResult = recv(AcceptSocket, serverUser, 50, 0);
    cout << ip << " (" << AcceptSocket << ") User recieved: " << serverUser << endl;
    iResult = recv(AcceptSocket, serverPassword, 50, 0);
    cout << ip << " (" << AcceptSocket << ") Password recieved: " << serverPassword << endl;

    bool userFound = false;
    vector<User> users = db.getUserByUsername(serverUser);
    for (const auto& user : users)
    {
        if (db.verifyCredentials(serverUser, serverPassword))
        {
            string userType = user.tipCont;

            if (userType == "student")
            {
                cout << "Logged in as student..." << endl;
                userFound = true;
                iResult = send(AcceptSocket, "studentLogin", DEFAULT_BUFLEN, 0);

                char command[DEFAULT_BUFLEN];

                while (1)
                {

                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0);

                    if (strcmp(command, "quit") == 0)
                        break;
                }
                break;
            }
            if (userType == "profesor")
            {
                cout << "Logged in as profesor..." << endl;
                userFound = true;
                iResult = send(AcceptSocket, "profesorLogin", DEFAULT_BUFLEN, 0);
                
                char command[DEFAULT_BUFLEN];

                while (1)
                {

                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0);

                    if (strcmp(command, "quit") == 0)
                        break;

                    if (strcmp(command, "create") == 0)
                        createHomework(&AcceptSocket);
                }
                break;
            }
            if (userType == "admin")
            {
                cout << "Logged in as admin..." << endl;
                userFound = true;
                iResult = send(AcceptSocket, "adminLogin", DEFAULT_BUFLEN, 0);

                char command[DEFAULT_BUFLEN];
                
                while (1)
                {
                    
                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0);

                    if (strcmp(command, "quit") == 0)
                        break;
                   
                    if (strcmp(command, "create") == 0)
                        createUser(&AcceptSocket);

                    if (strcmp(command, "delete") == 0)
                        deleteUser(&AcceptSocket);
                }

                //daca am primit comanda de logout, ies din clientul curent de tot
                //->
                //(de ce am pus break aici: cand puteam introduce mai multi username identici, ma loga in toti) 
                break;
            }
        }
        else
        {
            cout << "Inserted incorrect password..." << endl;
            iResult = send(AcceptSocket, "incorrectPassword", 50, 0);
            goto again;
        }
    }

    if (!userFound)
    {
        cout << "User not found!" << endl;
        iResult = send(AcceptSocket, "userNotFound", 50, 0);
        goto again;
    }
    //<-
    //adica am iesit aici

    cout << ip << " (" << AcceptSocket << ") disconnected..." << endl;

    return 0;
}
SOCKET initialiseServer()
{
    SOCKET ListenSocket = INVALID_SOCKET;


    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for the server to listen for client connections.
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = ::bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    return ListenSocket;
}

int __cdecl main(void)
{
    cout << "SERVER" << endl << endl << endl;

    int iResult;

    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = initialiseServer();

    do
    {
        ClientSocket = SOCKET_ERROR;

        while (ClientSocket == SOCKET_ERROR)
        {
            ClientSocket = accept(ListenSocket, NULL, NULL);
        }

        DWORD dwThreadId;
        CreateThread(NULL, 0, ProcessClient, (LPVOID)ClientSocket, 0, &dwThreadId);

    } while (1);

    // No longer need server socket
    closesocket(ListenSocket);


    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}