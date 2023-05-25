#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<fstream>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

//#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include "Database.h"



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

string connection_string = "Driver={ODBC Driver 17 for SQL Server};Server=localhost;Database=CampusConnectDB;Trusted_Connection=yes;";
Database db(connection_string);

//functii admin
void createUser(SOCKET* AcceptSocket)
{

    char username[DEFAULT_BUFLEN], password[DEFAULT_BUFLEN], type[DEFAULT_BUFLEN], grupa[DEFAULT_BUFLEN];
    recv(*AcceptSocket, username, DEFAULT_BUFLEN, 0);
    if (strcmp(username, "back") == 0) return;
    recv(*AcceptSocket, password, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, type, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, grupa, DEFAULT_BUFLEN, 0);


    string standardGrupa = grupa;
    for (auto& c : standardGrupa) {
        c = std::toupper(c);
    }
  
    User newUser( username,password,type,standardGrupa.c_str());
    if (db.createUser(newUser)) {
        std::cout << "Admin created user..." << endl;
    }
    else {
        std::cout << "Error creating user.\n";
    }
}
void deleteUser(SOCKET* AcceptSocket)
{

    //db.getNumberOfTableEntries("Accounts");
    char buf[DEFAULT_BUFLEN];
    send(*AcceptSocket, _itoa(db.getNumberOfAccounts(), buf, 10), DEFAULT_BUFLEN, 0);

    for (int i = 0; i < db.getNumberOfAccounts(); i++)
    {
        User user = db.getUsernameFromIndex(i);
        vector<User> toSend = db.getUserByUsername(user.getUsername());

        send(*AcceptSocket, toSend[0].getUsername().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getParola().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getTipCont().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getGrupaStudii().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
    }



    char username[DEFAULT_BUFLEN];
    recv(*AcceptSocket, username, DEFAULT_BUFLEN, 0);
    if (strcmp(username, "back") == 0) return;
    if (db.deleteUser(username)) {
        std::cout << "User deleted successfully.\n";
    }
    else {
        std::cout << "Error deleting user.\n";
    }
}
void updateUser(SOCKET* AcceptSocket)
{
   
    char buf[DEFAULT_BUFLEN];
    send(*AcceptSocket, _itoa(db.getNumberOfAccounts(), buf, 10), DEFAULT_BUFLEN, 0);

    //trimit aplicatiei client a adminului cate conturi trebuie sa afiseze, iar apoi ii trimit toate detaliile tuturor conturilor
    for (int i = 0; i < db.getNumberOfAccounts(); i++)
    {
        User user = db.getUsernameFromIndex(i);
        vector<User> toSend = db.getUserByUsername(user.getUsername());
        
        send(*AcceptSocket, toSend[0].getUsername().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getParola().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getTipCont().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, toSend[0].getGrupaStudii().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
    }
    //
    
    char username[DEFAULT_BUFLEN];
    recv(*AcceptSocket, username, DEFAULT_BUFLEN, 0);
    if (strcmp(username, "back") == 0) return;
    char parolaNoua[DEFAULT_BUFLEN];
    recv(*AcceptSocket, parolaNoua, DEFAULT_BUFLEN, 0);

    char grupaNoua[DEFAULT_BUFLEN];
    recv(*AcceptSocket, grupaNoua, DEFAULT_BUFLEN, 0);

    string standardGrupa = grupaNoua;
    for (auto& c : standardGrupa) {
        c = std::toupper(c);
    }

    db.updateUserDB(username,parolaNoua, standardGrupa);
    std::cout << "Admin updated user..." << endl;
}   
//gata functii admin


//functii profesor
void createHomework(SOCKET* AcceptSocket)
{
    char grupa[DEFAULT_BUFLEN], data[DEFAULT_BUFLEN],titlu[DEFAULT_BUFLEN];
    recv(*AcceptSocket, grupa, DEFAULT_BUFLEN, 0);
    if (strcmp(grupa, "back") == 0) return;
    recv(*AcceptSocket, data, DEFAULT_BUFLEN, 0);
    recv(*AcceptSocket, titlu, DEFAULT_BUFLEN, 0);

    string standardGrupa = grupa;
    for (auto& c : standardGrupa)
        c = std::toupper(c);

    Homework newHomework(0, standardGrupa.c_str(), data, titlu);
    db.createHomework(newHomework);
}
void viewStudentsAssignedToHomework(SOCKET* AcceptSocket)
{
    char buf[DEFAULT_BUFLEN];

    //am trimis numarul de widgetul care trebuie afisate
    send(*AcceptSocket, _itoa(db.getNumberOfHomework(), buf, 10), DEFAULT_BUFLEN, 0);

    for (int i = 0; i < db.getNumberOfHomework(); i++)
    {
        Homework hw = db.getHomeworkFromIndex(i);
       
        strcpy(buf, " ");
        send(*AcceptSocket, _itoa(hw.getID(), buf, 10), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw.getGrupa().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw.getTermenLimita().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw.getTitlu().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
    }

   //dupa ce serverul a primit toate temele, astept aici ca clientul sa imi raspunda cu id-ul temei la care vrea sa vada statusurile
    char homeworkID[DEFAULT_BUFLEN];
    recv(*AcceptSocket, homeworkID, DEFAULT_BUFLEN, 0);


showAgain:

    vector<User> students = db.getStudentsByHomework(atoi(homeworkID));

    string sizeStr = to_string(students.size());
    char studentNumber[10]=" ";
    strcpy(studentNumber, sizeStr.c_str());


    send(*AcceptSocket, studentNumber, 10, 0);

    for ( auto& user : students) 
    {
        bool status = db.getHomeworkStatus(user, atoi(homeworkID));

        send(*AcceptSocket, user.getUsername().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, user.getGrupaStudii().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);

        //send their status
        if (status)
            send(*AcceptSocket, "Complet", DEFAULT_BUFLEN, 0);
        else
            send(*AcceptSocket, "Incomplet", DEFAULT_BUFLEN, 0);
        Sleep(1);
    }


    //trimit verificarea daca se pot genera rapoarte
    if (!db.generateHomeworkReport(atoi(homeworkID)))
        send(*AcceptSocket, "cannotGenerate", 15, 0);
    else 
        //generarea se face automat in acea metoda
        send(*AcceptSocket, "can123Generate", 15, 0);

    Sleep(10);




   
recieveAnotherCommand:
    recv(*AcceptSocket, buf, DEFAULT_BUFLEN, 0);
    if (strcmp(buf, "downloadFromServer") == 0)
    {
        std::cout << "Requested download..." << endl;
        
        recv(*AcceptSocket, buf, DEFAULT_BUFLEN, 0);
        int homeworkID = atoi(buf);

        recv(*AcceptSocket, buf, DEFAULT_BUFLEN, 0);
        string username = buf;
        
        //acum aflu fisierul asociat utilizatorului si temei primite
        File fisierAsociat = db.getFileAssociated(username, homeworkID);
        send(*AcceptSocket, fisierAsociat.getCale().c_str(), DEFAULT_BUFLEN, 0);

        
        //incep trimiterea fisierului
        std::ifstream inFile(fisierAsociat.getCale().c_str(), std::ios::binary);
        if (!inFile) {  return; }

        char buffer[65536];
        int bytesSent;
        while (inFile.read(buffer, sizeof(buffer)) || inFile.gcount()) 
        {
            int bytesToWrite = (int)inFile.gcount();
            if (inFile.eof() && bytesToWrite < sizeof(buffer)) 
            {
                buffer[bytesToWrite] = '\0';
                bytesToWrite++;
            }
            bytesSent = send(*AcceptSocket, buffer, bytesToWrite, 0);
            
            if (bytesSent == SOCKET_ERROR) { return; }
            Sleep(1);
        }
        Sleep(1000);
        send(*AcceptSocket, "gata", 5, 0);
        
        inFile.close();
        //am terminat trimiterea fisierului catre profesor


        std::cout << "Un profesor a descarcat un fisier..." << endl;
        goto recieveAnotherCommand;
    }
    if (strcmp(buf, "marked") == 0)
    {
        char user[DEFAULT_BUFLEN], nota[DEFAULT_BUFLEN];

        recv(*AcceptSocket, homeworkID, DEFAULT_BUFLEN, 0);
        recv(*AcceptSocket, user, DEFAULT_BUFLEN, 0);
        recv(*AcceptSocket, nota, DEFAULT_BUFLEN, 0);
        
        db.markHomework(atoi(homeworkID), user, atof(nota));
        goto recieveAnotherCommand;
    }
    if (strcmp(buf, "generateReport") == 0)
    {
        
        send(*AcceptSocket, studentNumber, 10, 0);
        Sleep(1);
        for ( auto& user : students)
        {
            send(*AcceptSocket, user.getUsername().c_str(), DEFAULT_BUFLEN, 0);
            Sleep(1);

            char buffer[DEFAULT_BUFLEN];
            std::sprintf(buffer, "%.2f", db.getMark(atoi(homeworkID), user));
            send(*AcceptSocket, buffer, DEFAULT_BUFLEN, 0);
            Sleep(1);  
        }

        char buffer[DEFAULT_BUFLEN];
        std::sprintf(buffer, "%.2f", db.getMedia(atoi(homeworkID)));
        send(*AcceptSocket, buffer, 10, 0);
        Sleep(1);

        goto recieveAnotherCommand;
    }
    if (strcmp(buf, "back") == 0)
    {
        return;
    }
    else
    {
        strcpy(homeworkID, buf);
        goto showAgain;
    }

}
//gata functii profesor


//functii student
void seeHomework(SOCKET* AcceptSocket,const User& user)
{
    char buf[DEFAULT_BUFLEN];

    vector<Homework> hw = db.getAssignedHomeworkForStudent(user);
    
    //trebuie sa mai aflu numarul de teme, il bag in loc de 3
    //trebuie sa si trimit numarul de teme
    send(*AcceptSocket, _itoa(db.getNumberOfAssignedHomework(user), buf, 10), DEFAULT_BUFLEN, 0);
    //Sleep(1);
    
    for (int i = 0; i < db.getNumberOfAssignedHomework(user); i++)
    {
        
        strcpy(buf, " ");
        send(*AcceptSocket, _itoa(hw[i].getID(), buf, 10), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw[i].getTitlu().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw[i].getGrupa().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        send(*AcceptSocket, hw[i].getTermenLimita().c_str(), DEFAULT_BUFLEN, 0);
        Sleep(1);
        
        //sa trimit catre afisare si nota primita din database
        char buffer[DEFAULT_BUFLEN];
        std::sprintf(buffer, "%.2f", db.getMark(hw[i].getID(), user));
        send(*AcceptSocket, buffer, DEFAULT_BUFLEN, 0);
        Sleep(1);
        
        //aici enat mai trebuie sa faca un recieve de nota si vede el ce face cu ea
        //face o verificare la nota si in functie de rezultatul primit de el afiseaza waiting sau afiseaza nota
        
        //char status[DEFAULT_BUFLEN];
        if (db.getHomeworkStatus(user, hw[i].getID()) == 1)
            send(*AcceptSocket, "Complet", DEFAULT_BUFLEN, 0);
        else 
            send(*AcceptSocket, "Incomplet", DEFAULT_BUFLEN, 0);
        Sleep(1);
    }
}
//gata fucntii student

DWORD WINAPI ProcessClient(LPVOID lpParameter)
{
   
    SOCKET AcceptSocket = (SOCKET)lpParameter;

    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
    getpeername(AcceptSocket, (SOCKADDR*)&addr, &addrlen);
    char* ip = inet_ntoa(addr.sin_addr);
    std::cout << ip << " (" << AcceptSocket << ") initialised a connection..." << endl;

    char serverUser[50];
    char serverPassword[50];
    int iResult;

    iResult = recv(AcceptSocket, serverUser, 50, 0);
    //cout << ip << " (" << AcceptSocket << ") User recieved: " << serverUser << endl;
    iResult = recv(AcceptSocket, serverPassword, 50, 0);
    //cout << ip << " (" << AcceptSocket << ") Password recieved: " << serverPassword << endl;
        

    bool userFound = false;
    vector<User> users = db.getUserByUsername(serverUser);
    for ( auto& user : users)
    {
        userFound = true;
        if (db.verifyCredentials(serverUser, serverPassword))
        {
            string userType = user.getTipCont();
            std::cout << ip << " (" << AcceptSocket << ") User logged in: " << user.getUsername() << endl;
            if (userType == "student")
            {
                userFound = true;
                iResult = send(AcceptSocket, "studentLogin", 13, 0);

                char command[DEFAULT_BUFLEN];

                while (1)
                {
                    seeHomework(&AcceptSocket,user);
                    
                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0); //eu primesc tema pe care a selectat - o, se expandeaza in dreapta, si acolo am optiunea de upload file

                    if (strcmp(command, "uploadFile") == 0) //aici trebuie sa stiu deja catre care tema vreau sa trimit fisierul
                    {
                        char buf[DEFAULT_BUFLEN];
                        recv(AcceptSocket, buf, DEFAULT_BUFLEN, 0);
                        int nr = atoi(buf);
                        Homework hw = db.getHomeworkByID(nr);


                        //sa verific calea am un % in plus!!!!
                        string cale = "C:\\POO\\" + to_string(hw.getID()) + "_" + hw.getTitlu();
                        //sa verific calea aici in plm!!!!

                        if (!fs::is_directory(cale.c_str()) || !fs::exists(cale.c_str())) {
                            fs::create_directory(cale.c_str());
                        }

                        char extensie[50] = "n";
                        recv(AcceptSocket, extensie, 4, 0);
                        cale = cale + "\\" + user.getUsername() + '.' + string(extensie);


                        //aici e send-ul fisierului efectiv, in functie de cale
                        char buffer[65536];
                        int bytesReceived;
                        ofstream outFile(cale, std::ios::binary);
                        while ((bytesReceived = recv(AcceptSocket, buffer, sizeof(buffer), 0)) > 0)
                        {
                            if (strcmp(buffer, "gata") == 0)
                                break;
          
                            outFile.write(buffer, bytesReceived);
                            std::cout << "Se primeste..." << endl;
                            Sleep(1);
                        }

                        outFile.close();
                        std::cout << "S-a primit." << endl;

                        File file( 0,hw.getID(), user.getUsername(), cale);
                        db.generateFilePath(file);
                        db.updateHomeworkStatus(user, hw);
                    }
                    if(strcmp(command,"logout") == 0) 
                        break;
                   
                }
                break;
            }
            if (userType == "profesor")
            {
                userFound = true;
                iResult = send(AcceptSocket, "profesorLogin", 14, 0);
                
                char command[DEFAULT_BUFLEN];

                while (1)
                {

                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0);

                    if (strcmp(command, "logout") == 0)
                        break;

                    if (strcmp(command, "create") == 0)
                        createHomework(&AcceptSocket);

                    if (strcmp(command, "viewStudents") == 0)
                        viewStudentsAssignedToHomework(&AcceptSocket);

                }
                break;
            }
            if (userType == "admin")
            {
                userFound = true;
                iResult = send(AcceptSocket, "adminLogin", 11, 0);

                char command[DEFAULT_BUFLEN];
                
                while (1)
                {
                    
                    recv(AcceptSocket, command, DEFAULT_BUFLEN, 0);

                    if (strcmp(command, "logout") == 0)
                        break;
                   
                    if (strcmp(command, "create") == 0)
                        createUser(&AcceptSocket);

                    if (strcmp(command, "delete") == 0)
                        deleteUser(&AcceptSocket);

                    if (strcmp(command, "update") == 0)
                        updateUser(&AcceptSocket);
                }

                //daca am primit comanda de logout, ies din clientul curent de tot
                //->
                //(de ce am pus break aici: cand puteam introduce mai multi username identici, ma loga in toti) 
                break;
            }
        }
        else
        {
            std::cout << "Inserted incorrect password..." << endl;
            iResult = send(AcceptSocket, "incorrectPassword", 18, 0);
        }
    }

    if (!userFound)
    {
        std::cout << "User not found!" << endl;
        iResult = send(AcceptSocket, "userNotFound", 13, 0);
    }
    //<-
    //adica am iesit aici

    std::cout << ip << " (" << AcceptSocket << ") disconnected..." << endl;

    DWORD test=0;
    return test;
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
    cout << "================================" << endl;
    cout << "-----Campus Connect Server------" << endl;
    cout << "================================" << endl<<endl<<endl;

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