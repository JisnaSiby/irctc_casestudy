#pragma once
#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <string>
using namespace std;
struct date{
	int day;
	int month;
	int year;
};
struct time{
	int hour;
	int minute;
};
struct seat{
	char tname[25];
	int quota;
	char start[25];
	char stop[25];
	int fcs;
	int scs;
	int fcf;
	int scf;
};
struct booking{
	int id;
	char uname[20];
	date dt;
	char trname[25];
	int quotanum;
	char start[25];
	char stop[25];
	int fcsbooked;
	int scsbooked;
	int fcf;
	int scf;
};
class Manager{
private:
	char name[20];
	char password[20];
public:
Manager();
Manager(char *,char *);
void addTrain();
void modifyTrain();
void deleteTrain();
void setQuota();
void viewbalance();
void viewDatabase(char *);
};
