#pragma once
#include<iostream>
using namespace std;
class Homework
{
private:
	int id = -1;
	string grupa;
	string termenLimita;
	string titlu;
	friend class Database;
public:
	int getID() { return this->id; }
	string getGrupa() { return this->grupa; }
	string getTermenLimita() { return this->termenLimita; }
	string getTitlu() { return this->titlu; }

	Homework(){}
	Homework(int id, const char* grupa, const char* data, const char* titlu)
	{
		this->id = id;
		this->grupa = grupa;
		this->termenLimita = data;
		this->titlu = titlu;
	}
};

