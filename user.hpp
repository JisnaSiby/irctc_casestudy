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
class User{
        private:
		 char uname[40],pwd[40],fname[50],lname[50],email[50],gender[20];
                 int age;
                long int mobno;
		int wallet;
        public:
		User();
		User(char *,char *);
		User(char *,char *,char *,char *,int ,char *,char *,long int);
        void user_reg();
        User user_login();
        void modify(User);
        void setwallet(User);
        void viewTrain();
        void viewWallet(User);
        void bookTrain(User);
        void book(int ,int ,User,GDBM_FILE,datum,datum,booking,seat);
        void cancel(User us);
};

