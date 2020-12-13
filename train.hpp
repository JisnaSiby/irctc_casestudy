#pragma once
#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "manager.hpp"
using namespace std;
/*struct date{
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
};*/
class Train{
    private:
        char name[25], start[25], dest[25];
	struct date dat;
	struct time tim;
	vector<char*> inter;
    public:
        Train();
        Train(char *,struct date dt,struct time ti,char *, char *,vector<char*> );
	void setName(char *);
    void setStart(char *);
    void setDest(char *);
    void setInter(vector<char*> );
    char *getName();
    char *getStart();
    char *getDest();
    vector<char*> getInter();
	friend class Manager;
};
