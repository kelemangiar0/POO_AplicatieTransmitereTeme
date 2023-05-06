#include "Database.h"

bool Database::createUser(const User& user) {
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            INSERT INTO Accounts (username, parola, tipCont, grupaStudii) VALUES (?, ?, ?, ?);
        )");


    stmt.statement::bind(0, user.username.c_str());
    stmt.statement::bind(1, user.parola.c_str());
    stmt.statement::bind(2, user.tipCont.c_str());
    stmt.statement::bind(3, user.grupaStudii.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

bool Database::deleteUser(const string& username) 
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            DELETE FROM Accounts WHERE username = ?;
        )");

    stmt.statement::bind(0, username.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

bool Database::createHomework(const Homework& homework)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            INSERT INTO Homework (grupaStudii, deadline) VALUES (?, ?);
        )");


    stmt.statement::bind(0, homework.grupa.c_str());
    stmt.statement::bind(1, homework.termenLimita.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

vector<User> Database::getUserByUsername(const std::string& username)
{
    std::vector<User> users;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT username, parola, tipCont, grupaStudii FROM Accounts WHERE username = ?;
        )");

    //std::string user = username;
    stmt.statement::bind(0, username.c_str());

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        User user;
        user.username = result.get<std::string>(0);
        user.parola = result.get<std::string>(1);
        user.tipCont = result.get<std::string>(2);
        user.grupaStudii = result.get<std::string>(3);
        users.push_back(user);
    }

    return users;
}

bool Database::verifyCredentials(const char* insertedUsername, const char* insertedPassword)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
        SELECT parola FROM Accounts WHERE username = ?;
    )");


    stmt.statement::bind(0, insertedUsername);
    nanodbc::result result = stmt.execute();

    string databasePassword;

    while (result.next())
        databasePassword = result.get<std::string>(0);

    if (strcmp(insertedPassword, databasePassword.c_str()) != 0)
        return false;

    else return true;
}


void Database::disconnect() {
    if (conn.connected()) {
        conn.disconnect();
        conn.deallocate();
    }
}

//bool Database::updateUser(const User& user) {
//    nanodbc::statement stmt(conn);
//    nanodbc::prepare(stmt, R"(
//            UPDATE Accounts SET username = ?, parola = ?, tipCont= ?, grupaStudii = ? WHERE username = ?;
//        )");
//
//    stmt.statement::bind(0, user.username.c_str());
//    stmt.statement::bind(1, user.parola.c_str());
//    stmt.statement::bind(2, user.tipCont.c_str());
//    stmt.statement::bind(3, user.grupaStudii.c_str());
//    //stmt.statement::bind(4, &user.username);
//
//    nanodbc::result rez = stmt.execute();
//
//    return rez.has_affected_rows();
//}