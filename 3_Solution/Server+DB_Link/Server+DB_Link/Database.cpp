#include "Database.h"
//int Database::getNumberOfTableEntries(const string& table)
//{
//    nanodbc::statement stmt(conn);
//    nanodbc::prepare(stmt, R"(
//        SELECT COUNT(*) FROM ?;
//    )");
//
//    stmt.statement::bind(0, table.c_str());
//    nanodbc::result result = stmt.execute();
//
//    int nr = 0;
//    while (result.next())
//        nr = atoi(result.get<std::string>(0).c_str());
//
//    return nr;
//}
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
            INSERT INTO Homework (grupaStudii, deadline, titlu) VALUES (?, ?, ?);
        )");


    stmt.statement::bind(0, homework.grupa.c_str());
    stmt.statement::bind(1, homework.termenLimita.c_str());
    stmt.statement::bind(2, homework.titlu.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

vector<User> Database::getStudentsByHomework(int homeworkID)
{
    vector<User> students;

    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT Accounts.username, Accounts.parola, Accounts.tipCont, Accounts.grupaStudii FROM Accounts WHERE grupaStudii = ( SELECT grupaStudii FROM Homework where id = ? );
        )");

    stmt.statement::bind(0, &homeworkID);


    nanodbc::result result = stmt.execute();

    while (result.next()) {
        User user;
        user.username = result.get<std::string>(0);
        user.parola = result.get<std::string>(1);
        user.tipCont = result.get<std::string>(2);
        user.grupaStudii = result.get<std::string>(3);
        students.push_back(user);
    }
    return students;
}

int Database::getNumberOfHomework()
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
        SELECT COUNT(*) FROM Homework;
    )");

    nanodbc::result result = stmt.execute();

    int nr = 0;
    while (result.next())
        nr = atoi(result.get<std::string>(0).c_str());

    return nr;
}

Homework Database::getHomeworkFromIndex(int index)
{
    Homework hw;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT id, grupaStudii, deadline, titlu FROM Homework ORDER BY id OFFSET ? ROWS FETCH NEXT 1 ROWS ONLY;
        )");

    stmt.statement::bind(0, &index);

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        hw.id = atoi(result.get<std::string>(0).c_str());
        hw.grupa = result.get<std::string>(1);
        hw.termenLimita = result.get<std::string>(2);
        hw.titlu = result.get<std::string>(3);
    }

    return hw;
}

bool Database::generateHomeworkReport(int homeworkID)
{

    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            EXEC VerificaPosibilitateGenerareRaport ?;
        )");

 
    stmt.statement::bind(0, &homeworkID);

    nanodbc::result result = stmt.execute();

    int x = 0;
    while (result.next())
         x = atoi(result.get<std::string>(0).c_str());

    //trebuie dezalocata memoria inainte de a folosi alt statement in acelasi stack frame
    stmt.close();

    if (x == 0)
    {
        return false;
    }
    else
    {
        nanodbc::statement stmt(conn);
        nanodbc::prepare(stmt, R"(
            exec CalculMedieNota ?;
            )");

        int hID = homeworkID;
        stmt.statement::bind(0, &hID);

        nanodbc::result result = stmt.execute();
        return true;
    }
    
}

void Database::markHomework(int homeworkID, const char* user, float mark)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            UPDATE HomeworkStatus SET nota = ? WHERE temaID = ? AND student = ?;
        )");


    stmt.statement::bind(1, &homeworkID);
    stmt.statement::bind(2, user);
    stmt.statement::bind(0, &mark);

    nanodbc::result result = stmt.execute();

}

float Database::getMark(int homeworkID, const User& user)
{
    float mark = (float)-1.1;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT CAST(nota AS varchar(10)) FROM HomeworkStatus WHERE student = ? AND temaID = ?;
        )");

    stmt.statement::bind(0, user.username.c_str());
    stmt.statement::bind(1, &homeworkID);

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        mark = stof(result.get<std::string>(0));
    }

    return mark;
}

float Database::getMedia(int homeworkID)
{
    float mark = (float) -1.1;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT CAST(medie AS varchar(10)) FROM Homework WHERE id = ?;
        )");

    
    stmt.statement::bind(0, &homeworkID);

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        mark = stof(result.get<std::string>(0));
    }

    return mark;
}

bool Database::updateHomeworkStatus(const User& user, const Homework& homework)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            UPDATE HomeworkStatus SET stare = 'Complet' WHERE student = ? AND temaID = ?;
        )");
    
    stmt.statement::bind(0, user.username.c_str());
    stmt.statement::bind(1, &homework.id);

    nanodbc::result result = stmt.execute();

    return result.has_affected_rows();
}

bool Database::getHomeworkStatus(const User& user, int homeworkID)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT stare FROM HomeworkStatus WHERE temaID = ? AND student = ?;
        )");

    stmt.statement::bind(0, &homeworkID);
    stmt.statement::bind(1, user.username.c_str());

    nanodbc::result result = stmt.execute();

    string status;

    while (result.next())
    status = result.get<std::string>(0);

    if (status == "Incomplet")
        return false;

    return true;
}

vector<Homework> Database::getAssignedHomeworkForStudent(const User& user)
{
    vector<Homework> hw;

    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT Homework.id, Homework.grupaStudii, Homework.deadline, Homework.titlu FROM Homework INNER JOIN HomeworkStatus ON Homework.id = HomeworkStatus.temaID WHERE HomeworkStatus.student = ?;
        )");

    stmt.statement::bind(0, user.username.c_str());

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        Homework hwl;
        hwl.id = atoi(result.get<std::string>(0).c_str());
        hwl.grupa = result.get<std::string>(1);
        hwl.termenLimita = result.get<std::string>(2);
        hwl.titlu = result.get<std::string>(3);
        hw.push_back(hwl);
    }

    return hw;
}

int Database::getNumberOfAssignedHomework(const User& user)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
        SELECT COUNT(*) FROM Homework INNER JOIN HomeworkStatus on Homework.id = HomeworkStatus.temaID WHERE HomeworkStatus.student = ?;
    )");

    stmt.statement::bind(0, user.username.c_str());

    nanodbc::result result = stmt.execute();

    int nr = 0;
    while (result.next())
        nr = atoi(result.get<std::string>(0).c_str());

    return nr;
}

bool Database::generateFilePath(const File& file)
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            INSERT INTO Files (temaID, student, cale) VALUES (?, ?, ?);
        )");

    stmt.statement::bind(0, &file.temaID);
    stmt.statement::bind(1, file.student.c_str());
    stmt.statement::bind(2, file.cale.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

File Database::getFileAssociated(const string& username, int homeworkID)
{
    File file;

    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT id, cale FROM Files WHERE temaID = ? AND student = ?;
        )");

    stmt.statement::bind(1, username.c_str());
    stmt.statement::bind(0, &homeworkID);

    nanodbc::result result = stmt.execute();

    while (result.next()) {
      
        file.id = atoi(result.get<std::string>(0).c_str());
        file.temaID = homeworkID;
        file.student = username;
        file.cale = result.get<std::string>(1);
    }

    return file;
}

Homework Database::getHomeworkByID(int homeworkID)
{
    Homework hw;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT id, grupaStudii, deadline, titlu FROM Homework WHERE id = ?;
        )");

    //std::string user = username;
    stmt.statement::bind(0, &homeworkID);

    nanodbc::result result = stmt.execute();

    while (result.next()) {
        //User user;
        hw.id = atoi(result.get<std::string>(0).c_str());
        hw.grupa = result.get<std::string>(1);
        hw.termenLimita = result.get<std::string>(2);
        hw.titlu = result.get<std::string>(3);
        //users.push_back(user);
    }

    return hw;
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

bool Database::updateUserDB(const string& username, string parolanoua="no",  string grupanoua = "no") {

    User fetchedUser;
    vector<User> users = this->getUserByUsername(username);
    for (const auto& user : users)
    {
        fetchedUser = user;
    }
    //am gasit userul pt care vreau sa fac update

    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            UPDATE Accounts SET parola = ?, grupaStudii = ? WHERE username = ?;
        )");

    stmt.statement::bind(0, parolanoua.c_str());
    stmt.statement::bind(1, grupanoua.c_str());
    stmt.statement::bind(2, username.c_str());

    nanodbc::result rez = stmt.execute();

    return rez.has_affected_rows();
}

int Database::getNumberOfAccounts()
{
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
        SELECT COUNT(*) FROM Accounts;
    )");

    nanodbc::result result = stmt.execute();

    int nr=0;
    while (result.next())
        nr = atoi(result.get<std::string>(0).c_str());

    return nr;
}

User Database::getUsernameFromIndex(int index)
{
    User users;
    nanodbc::statement stmt(conn);
    nanodbc::prepare(stmt, R"(
            SELECT username FROM Accounts ORDER BY username OFFSET ? ROWS FETCH NEXT 1 ROWS ONLY;
        )");

    stmt.statement::bind(0, &index);

    nanodbc::result result = stmt.execute();

    while (result.next())
        users.username = result.get<std::string>(0);

    return users;
}
