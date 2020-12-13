#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "manager.hpp"
#include "user.hpp"
#include "train.hpp"
using namespace std;

int main(){
    int choice,op,ch;
	int opt,ret;
    char u[20],pass[20];
    char n[20]="sahal";
    char p[20]="123";
    Manager m1(n,p);
    User ul,us;
    printf("\t\t\t1:User\t\t2:Admin:\n\t\t\tEnter the option: ");
    scanf("%d",&op);
	 if(op==1){
                 while(true){
                cout<<endl;
                 cout<<"\t\t1.Registration\t\t2.User Login\t\t0.exit"<<endl<<"Enter your choice: ";cin>>ch;
                switch(ch){
                case 1:	ul.user_reg();break;
                case 2:us=ul.user_login();
                        while(true){
                         cout<<"\n1.Modify Record\n2.Add money to wallet\n3:view wallet amount\n4:view trains\n5.book ticket\n6:View seat and fare details of trains\n7:cancel ticket\n0.exit"<<endl;
			cout<<"\nEnter your choice: ";cin>>opt;
			system("clear");
                        if(opt==1)
                                ul.modify(us);
                        else if(opt==2)
                                ul.setwallet(us);
			else if(opt==3)ul.viewWallet(us);
			else if(opt==4)ul.viewTrain();
                        else if(opt==5)ul.bookTrain(us);
			else if(opt==6){char dbname[20]="QUOTA.db";m1.viewDatabase(dbname);}
            //    else if(opt==7){char dbname[20]="SEAT.db";m1.viewDatabase(dbname);}
                        else if(opt==7)ul.cancel(us);
			else exit(0);
		}
		break;
                case 0:exit(1);
                }
	     }
        }
      if(op==2){
	printf("\n\t\t\tEnter username: ");
	scanf("%s",u);
	printf("\n\t\t\tEnter password: ");
	scanf("%s",pass);
	printf("\n______________________________________________________________________");
	if((strcmp(u,n)!=0) || (strcmp(pass,p)!=0)){ printf("\n invalid username or password\n");exit(0);}
	else printf("\nLogin successfull");
    }
    else exit(0);
    while(true){
	int choice1=1;
    	printf("\nPress 1: add train\t2: modify train\t\t3: delete train\t\t4:set quota for a train\t\t5:Report generation\t\t0: quit\n\nEnter the option: ");
        scanf("%d",&choice);
	system("clear");
        if(choice==1)m1.addTrain(); 
        else if(choice==2)m1.modifyTrain();
        else if(choice==3)m1.deleteTrain();
        else if(choice==4)m1.setQuota();
	else if(choice==5){
	  while(choice1){
		cout<<"\n1:Display all trains running\n2:View full details of Quota\n3:Total wallet amount\n4:View current seat availability\n5:View all bookings(no:of ticket sold)\n6:no:of ticket cancelled\n7:View all waiting list\n0:Previous Menu";
        	cout<<"\n\nEnter your option:";
		cin>>choice1;
		system("clear");
		if(choice1==1){char dbname[20]="TRMS.db";m1.viewDatabase(dbname);}
        	else if(choice1==2){char dbname[20]="QUOTA.db";m1.viewDatabase(dbname);}
        	else if(choice1==3)m1.viewbalance();
        	else if(choice1==4){char dbname[20]="SEAT.db";m1.viewDatabase(dbname);}
        	else if(choice1==5){char dbname[20]="BOOKING.db";m1.viewDatabase(dbname);}
        	else if(choice1==6){char dbname[20]="CANCELREPORT.db";m1.viewDatabase(dbname);}
        	else if(choice1==7){char dbname[20]="WAITINGLIST.db";m1.viewDatabase(dbname);}
        	else break;
	  }
        }
        else exit(0);
    }
    return 0;
}
