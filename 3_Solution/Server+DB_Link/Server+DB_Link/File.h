#pragma once
#include<string>
using namespace std;
class File
{
private:
	int id = -1;
	int temaID = -1;
	string student;
	string cale;
	friend class Database;
public:
	File() {}
	File(int id, int tID, string stud, string cale)
	{
		this->id = id;
		this->temaID = tID;
		this->student = stud;
		this->cale = cale;
	}
	int getID() { return this->id; }
	int getTemaID() { return this->temaID; }
	string getStudent() { return this->student; }
	string getCale() { return this->cale; }
};

