#pragma once
#include <string>
using namespace std;

class User {
private:
    string username;
    string parola;
    string tipCont;
    string grupaStudii;
    friend class Database;
public:
    User(){}
    User(const char* user, const char* pass, const char* tip, const char* grupa)
    {
        this->username = user;
        this->parola = pass;
        this->tipCont = tip;
        this->grupaStudii = grupa;
    }
    string getUsername() { return this->username; }
    string getParola() { return this->parola; }
    string getTipCont() { return this->tipCont; }
    string getGrupaStudii() { return this->grupaStudii; }
};