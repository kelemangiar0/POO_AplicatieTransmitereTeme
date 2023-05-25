#pragma once
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "User.h"
#include "Homework.h"
#include"File.h"
#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <nanodbc/nanodbc.h>


using namespace std;

class Database {
private:
    nanodbc::connection conn;

    void disconnect();

public:
    Database(const string connectionString) : conn(connectionString) {}
    ~Database() {}

    //toti
    vector<User> getUserByUsername(const string& username);
    bool verifyCredentials(const char* username, const char* password);
    //int getNumberOfTableEntries(const string& table);

    //admin
    bool createUser(const User& user);
    bool deleteUser(const string& username);
    bool updateUserDB(const string& user, string parolanoua,  string grupanoua);
    int getNumberOfAccounts();
    User getUsernameFromIndex(int index);

    //profesori
    Homework getHomeworkFromIndex(int index);
    bool createHomework(const Homework& homework);
    vector<User> getStudentsByHomework(int homeworkID);
    int getNumberOfHomework();  
    bool generateHomeworkReport(int homeworkID);
    void markHomework(int homeworkID, const char* user, float mark);
    float getMark(int homeworkID, const User& user);
    float getMedia(int homeworkID);


    //studenti
    bool updateHomeworkStatus(const User& user,const Homework& homework);
    bool getHomeworkStatus(const User& user, int homeworkID);
    vector<Homework> getAssignedHomeworkForStudent(const User& user);
    int getNumberOfAssignedHomework(const User& user);
    bool generateFilePath(const File& file);
    File getFileAssociated(const string& username, int homeworkID);
    Homework getHomeworkByID(int homeworkID);
};

