#pragma once
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include "User.h"
#include "Homework.h"
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

    bool createUser(const User& user);
    vector<User> getUserByUsername(const string& username);
    bool verifyCredentials(const char* username, const char* password);
    bool deleteUser(const string& username);
    //bool updateUser(const User& user);

    bool createHomework(const Homework& homework);
};

