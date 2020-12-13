#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "user.hpp"
using namespace std;
		
User::User(){}
User::User(char *n,char *p){strcpy(uname,n);strcpy(pwd,p);}
User::User(char *n,char *p,char *fn,char *ln,int a,char *gen,char *em,long int mob){}
void User::user_reg(){
        GDBM_FILE dbf;
        datum key,data;
        char pass[50];
        char keybuf[100],databuf[200];
        dbf=gdbm_open("userlogin.db",0,GDBM_WRCREAT,0666,0);
        cout<<"\t\t-----Welcome to Railway Reservation System-----"<<endl;
        cout<<"\n\t\t******User Login******\t\t\n"<<endl;
        cout<<"\t\tUser Name: ";
        cin>>uname;
        cout<<"\t\tPassword : ";
        cin>>pass;
        cout<<"\t\tRe-enter Password: ";
        cin>>pwd;
        if(strcmp(pwd,pass)!=0){
                cout<<"Password does not match"<<endl;
                exit(0);
        }
	sprintf(keybuf,"%s %s",uname,pwd);
        key.dsize=strlen(keybuf)+1;
        key.dptr=keybuf;
         cout<<"\t\t******Personal Details****** \t\t"<<endl;
        cout<<"\n\t\tFirst Name   : ";
        cin>>fname;
        cout<<"\n\t\tLast Name    : ";
        cin>>lname;
        cout<<"\n\t\tAge          : ";
        cin>>age;
        cout<<"\n\t\tGender       : ";
        cin>>gender;
        cout<<"\n\t\tGmail        : ";
        cin>>email;
        cout<<"\n\t\tMobile Number: ";
        cin>>mobno;
        cout<<"Fname\t Lname\t Age\t Gender\t   Gmail \t    Mobile "<<endl;
        cout<<"-----\t -----\t --- \t------\t --------  \t  ------ "<<endl;
        sprintf(databuf,"%s\t %s\t %d\t %s\t %s\t %ld ",fname,lname,age,gender,email,mobno);
        data.dsize=strlen(databuf)+1;
        data.dptr=databuf;

        gdbm_store(dbf,key,data,GDBM_INSERT);
        data=gdbm_fetch(dbf,key);
        printf("%s",data.dptr);
        cout<<endl;
	    cout<<"\n\t\tThank you for Registration\n";
        cout<<"\n\t\t\t*****Welcome to IRCTC*****"<<endl;
        gdbm_close(dbf);
}
User User::user_login(){
        GDBM_FILE dbf;
        char keybuf[200],databuf[200];
        char pass[100],name[50];
        datum key, data;
        cout<<"\n\t\tUser Name: ";
        cin>>name;
        cout<<"\t\tPassword : ";
        cin>>pass;
	User us(name,pass);
	char u[50],p[50];
        dbf=gdbm_open("userlogin.db",0,GDBM_WRCREAT,0666,0);
	sprintf(keybuf,"%s %s",name,pass);
	key.dsize=strlen(keybuf)+1;
	key.dptr=keybuf;
	int ret=gdbm_exists(dbf,key);
        if(ret==1){
                cout<<"Login successfull"<<endl;
        	gdbm_close(dbf);
        	return us;
	}
        else{
                cout<<"Invalid Username or Password"<<endl;
        	gdbm_close(dbf);
		exit(0);
	}
}

void User::modify(User us){
        GDBM_FILE dbf;
        datum key, data;
        char keybuf[100],databuf[256];
        int flag=0;
        char pass[50];
        dbf=gdbm_open("userlogin.db",0,GDBM_WRCREAT,0666,0);
                cout<<"\t\t******Modify Personal Details****** \t\t"<<endl;
        cout<<"\n\t\tFirst Name   : ";
        cin>>fname;
        cout<<"\n\t\tLast Name    : ";
        cin>>lname;
        cout<<"\n\t\tAge          : ";
        cin>>age;
        cout<<"\n\t\tGender       : ";
        cin>>gender;
        cout<<"\n\t\tGmail        : ";
        cin>>email;
        cout<<"\n\t\tMobile Number: ";
        cin>>mobno;
        sprintf(keybuf,"%s %s",us.uname,us.pwd);
        key.dsize=strlen(keybuf)+1;
        key.dptr=keybuf;
        cout<<"Fname\t Lname\t  Age\t    Gender\t     Gmail \t    Mobile  "<<endl;
        cout<<"-----\t ----- \t --- \t  -------  \t ----------\t  -------   "<<endl;
        sprintf(databuf,"%s \t%s \t%d\t %s\t %s\t%ld ",fname,lname,age,gender,email,mobno);
   	data.dsize=strlen(databuf)+1;
   	data.dptr=databuf;
   	gdbm_store(dbf,key,data,GDBM_REPLACE);
   	data=gdbm_fetch(dbf,key);
   	printf("%s\n",data.dptr);
   	cout<<"\t\tRecord Updated"<<endl;
   	gdbm_close(dbf);
}
void User::setwallet(User us){
	int walletextra;
        GDBM_FILE dbf;
        datum key, data;
        char pass[25];
        char keybuf[50],databuf[256];
        cout<<"\n\t\t\tEnter the depositing amount: ";
        cin>>walletextra;
        dbf=gdbm_open("WALLET.db",0,GDBM_WRCREAT,0666,0);
	sprintf(keybuf,"%s",us.uname);
	cout<<"\n\t\t\tyour username:"<<us.uname;
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
	int ret=gdbm_exists(dbf,key);
	if(ret==1){//already exist.adding with previous wallet amt
		data=gdbm_fetch(dbf,key);
		sscanf(data.dptr,"%d",&wallet);
		cout<<"\n\t\t\tcurrent wallet amount:"<<wallet;
		wallet+=walletextra;
		sprintf(databuf,"%d",wallet);
		cout<<"\n\t\t\tNew wallet amount:"<<wallet;
  		data.dsize=strlen(databuf)+1;
  		data.dptr=databuf;
  		gdbm_store(dbf,key,data,GDBM_REPLACE);
	}
	else{
		sprintf(databuf," %d",walletextra);
		cout<<"\n\t\t\tNew wallet amount:"<<walletextra;
        	data.dsize=strlen(databuf)+1;
        	data.dptr=databuf;
   		gdbm_store(dbf,key,data,GDBM_INSERT);
	}
	cout<<"\n\t\t\tMoney added successfully";
	gdbm_close(dbf);
	return;
}

void User::viewTrain(){
	int i=1;
	GDBM_FILE db;
    	datum key, value;
    	db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
	key=gdbm_firstkey(db);
	printf("\n\t\t\tAll the train details are given below\n");
   	while(key.dptr){
    	   	 value=gdbm_fetch(db,key);
   		 printf("\n\t\t\t%d : %s %s",i++,key.dptr,value.dptr);
        	 datum nextkey;
       		 nextkey=gdbm_nextkey(db,key);
       		 free(key.dptr);
       		 key=nextkey;
	}
	gdbm_close(db);
}
void User::viewWallet(User us){
	GDBM_FILE db;
    	datum key,value;
	char keybuf[100];
    	db=gdbm_open("WALLET.db",0,GDBM_WRCREAT,0666,0);
	sprintf(keybuf,"%s",us.uname);
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
	printf("\n\t\t\tMoney in your wallet: ");
    	value=gdbm_fetch(db,key);
   	printf("%s ",value.dptr);
	gdbm_close(db);
}

void User::bookTrain(User us){
	struct date dt1;
	struct booking b1;
    	printf("\nEnter train name:");
	 cin.ignore();cin.get(b1.trname,25);
	//cin>>b1.trname;
    	printf("\n\t\t\tEnter the Date:");
    	printf("\n\t\t\t\tDay:");scanf("%d",&dt1.day);
   	printf("\n\t\t\t\tmonth:");scanf("%d",&dt1.month);
   	printf("\n\t\t\t\tyear:");scanf("%d",&dt1.year);	
    	GDBM_FILE db;
    	datum key,value;
    	char keybuf[100],valuebuf[256];
    	db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    	sprintf(keybuf,"%d %d %d %s",dt1.day,dt1.month,dt1.year,b1.trname);
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
    	int ret=gdbm_exists(db,key);
    	if(ret==0){printf("\n\t\t\tTrain doesnot exist:");gdbm_close(db);return;}
    	else{//train exist
	  int op;
   	  value=gdbm_fetch(db,key);
    	  printf("\nTrain Exist");
    	  printf("\n\t\t\t%s %s",key.dptr,value.dptr);
	  gdbm_close(db);
    	  printf("\npress 1 for booking the above train 0 to return to previous page");
    	  scanf("%d",&op);
	  if(op==1){
		struct booking b;
  		char stt[25],destn[25];
    		int type,seatcount;
		struct date dt;
		struct time ti;
    		cout<<"\n\t\tEnter the starting station: ";
    		cin>>b1.start;
    		cout<<"\n\t\tEnter the destination station: ";
    		cin>>b1.stop;
    		cout<<"\n\t\tEnter the class(1:first,2:second): ";
    		cin>>type;
    		cout<<"\n\t\tEnter the number of seats: ";
    		cin>>seatcount;
    		GDBM_FILE db1,db2,db3;
    		datum key1,value1,key2,value2,key3,value3;
    		char keybuf1[200],valuebuf1[256],keybuf2[200],valuebuf2[256],keybuf3[200],valuebuf3[256];
		int found=0;
    		db1=gdbm_open("SEAT.db",0,GDBM_WRCREAT,0666,0);
		//check corresponding quota with start station &destn exist or not
		for(int q=1;q<=10 && found!=1;q++){
    		 	 sprintf(keybuf1,"%d %d %d %s %d",dt1.day,dt1.month,dt1.year,b1.trname,q);
    		 	 key1.dsize=strlen(keybuf1)+1;
    		 	 key1.dptr=keybuf1;
		 	 int ret1=gdbm_exists(db1,key1);
		 	 if(ret1==1){//quota exist for the train.read all seat deatails
				struct seat s1;
				value1=gdbm_fetch(db1,key1);
   				sscanf(value1.dptr,"%s %s %d %d %d %d",s1.start,s1.stop,&s1.fcs,&s1.scs,&s1.fcf,&s1.scf);
				if(strcmp(b1.start,s1.start)==0 && strcmp(b1.stop,s1.stop)==0){
			  		found=1;
					b1.quotanum=q;
					b1.dt=dt1;
			  		printf("\nTrain with quota exist,quota number=%d",q);//1
			  		if(type==1)printf("\nyou selected 1st class seat");
					else printf("\nyou selected second class seat");
					book(type,seatcount,us,db1,key1,value1,b1,s1);//calling booking function
				}
		  	}
		}
  	   }
   	   else {return;}//return to previous page
  	}
  	return;
}
void User::book(int type,int seatcount,User us,GDBM_FILE db1,datum key1,datum value1,booking b1,seat s1){
		cout<<endl<<seatcount;
		int seatavailable,fare;
		if(type==1){//first class seat
			seatavailable=s1.fcs;
			fare=s1.fcf;
		}
		if(type==2){//2nd class seat
			seatavailable=s1.scs;
			fare=s1.scf;
		}
    		GDBM_FILE db2,db3;
    		datum key2,value2,key3,value3;
    		char keybuf1[200],valuebuf1[256],keybuf2[200],valuebuf2[256],keybuf3[200],valuebuf3[256];
		//check wallet amt
		db2=gdbm_open("WALLET.db",0,GDBM_WRCREAT,0666,0);
		sprintf(keybuf2,"%s",us.uname);
    	 	key2.dsize=strlen(keybuf2)+1;
    	 	key2.dptr=keybuf2;
	  	int ret1=gdbm_exists(db2,key2);
	  	if(ret1==0){printf("\n\t\tSorry.No money in your wallet...Add money to wallet&try again");gdbm_close(db2);gdbm_close(db1);return;}//user didnt add money to wallet
		else{//user has wallet ..now check user has enough wallet amt
		   value2=gdbm_fetch(db2,key2);
		   int curramt;
   		   sscanf(value2.dptr,"%d",&curramt);
		   if(curramt<(seatcount*fare)){
		 	printf("\n\t\tyou dont have enough money in your wallet.\n\t\tRequired amt=%d \n\t\tmoney in your wallet=%d \nAdd money and try back",seatcount*fare,curramt);
		  	gdbm_close(db2);
		  	gdbm_close(db1);
			return;
		   }
		   else printf("\nyou have enough amount in your wallet");
		   //checking available seats and store the count of seats to be in waiting list in remaining(rem)
		   int rem,booked;
		   if(seatavailable==0){
			rem=seatcount;
			booked=0;
		   }
		   else if(seatcount>seatavailable){
			if(type==1){b1.fcsbooked=seatavailable;b1.scsbooked=0;  rem=seatcount-b1.fcsbooked; booked=b1.fcsbooked;}
			if(type==2){b1.scsbooked=seatavailable;b1.fcsbooked=0;  rem=seatcount-b1.scsbooked; booked=b1.scsbooked;}
		   }
		   else{//seatcount<seatavailable
		    rem=0;
		    if(type==1){b1.fcsbooked=seatcount;b1.scsbooked=0; booked=b1.fcsbooked;}
		    if(type==2){b1.scsbooked=seatcount;b1.fcsbooked=0; booked=b1.scsbooked;}
		   }
		   cout<<endl<<"\nbooked tickets count="<<booked;
	           if(booked>0){
			//update seat in SEAT db
   		  	if(type==1)sprintf(valuebuf1,"%s %s %d %d %d %d",s1.start,s1.stop,seatavailable-booked,s1.scs,s1.fcf,s1.scf);
   		  	else sprintf(valuebuf1,"%s %s %d %d %d %d",s1.start,s1.stop,s1.fcs,seatavailable-booked,s1.fcf,s1.scf);
		  	value1.dsize=strlen(valuebuf1)+1;
    		  	value1.dptr=valuebuf1;
   		  	gdbm_store(db1,key1,value1, GDBM_REPLACE);
		  	gdbm_close(db1);
		  
			//substract money from user acc
			sprintf(valuebuf2,"%d",curramt-(booked*fare));
		  	value2.dsize=strlen(valuebuf2)+1;
    		  	value2.dptr=valuebuf2;
   		  	gdbm_store(db2,key2,value2, GDBM_REPLACE);
			printf("\n\t\t\tBILL\n\t\t_____________\n\tcost for 1 seat=%d",fare);	
			printf("\n\twithdrawn money from ur account is\n\tno:of seat booked*cost for 1 seat=%d",booked*fare);	
		  	gdbm_close(db2);
			//add money to admin wallet
			char ad[20]="admin";
			db2=gdbm_open("ADMINWALLET.db",0,GDBM_WRCREAT,0666,0);
			sprintf(keybuf2,"%s",ad);
	    	 	key2.dsize=strlen(keybuf2)+1;
    		 	key2.dptr=keybuf2;
	  		int ret1=gdbm_exists(db2,key2);
	  		if(ret1==0){//first time creation of wallet to admin (if wallet not yet created)
				int adamt=booked*fare;
				sprintf(valuebuf2,"%d",adamt);
			 	value2.dsize=strlen(valuebuf2)+1;
    			 	value2.dptr=valuebuf2;
   			 	gdbm_store(db2,key2,value2, GDBM_INSERT);
			 	gdbm_close(db2);
			}
			else{
				int amt;
				value2=gdbm_fetch(db2,key2);
				sscanf(value2.dptr,"%d",&amt);
				amt+=(booked*fare);
				sprintf(valuebuf2,"%d",amt);
			 	value2.dsize=strlen(valuebuf2)+1;
    			 	value2.dptr=valuebuf2;
   			 	gdbm_store(db2,key2,value2,GDBM_REPLACE);
			 	gdbm_close(db2);
			}
			printf("\nMoney added to admin wallet");
			//storing data to booking db
			struct booking b;
			int maxid=0;
    		  	db3=gdbm_open("BOOKING.db",0,GDBM_WRCREAT,0666,0);
			key3=gdbm_firstkey(db3);
			if(!key3.dptr)//no booking in db
				maxid=2000;
			else{//find last booking id
				while(key3.dptr){
    	   	 		  sscanf(key3.dptr,"%d",&b.id);
				  if(b.id>maxid)maxid=b.id;
        	 		  datum nextkey;
       		 		  nextkey=gdbm_nextkey(db3,key3);
       		 		  free(key3.dptr);
       		 		  key3=nextkey;
				}
			}
			b.id=++maxid;
			maxid=0;//resetting
    	  	  	sprintf(keybuf3,"%d",b.id);
    	 	  	key3.dsize=strlen(keybuf3)+1;
    	 	  	key3.dptr=keybuf3;
			value3=gdbm_fetch(db3,key3);
   		  	if(type==1)sprintf(valuebuf3,"%s %d %d %d %s %d %s %s %d %d %d %d",us.uname,b1.dt.day,b1.dt.month,b1.dt.year,b1.trname,b1.quotanum,b1.start,b1.stop,booked,0,s1.fcf,s1.scf);
   		  	else sprintf(valuebuf3,"%s %d %d %d %s %d %s %s %d %d %d %d",us.uname,b1.dt.day,b1.dt.month,b1.dt.year,b1.trname,b1.quotanum,b1.start,b1.stop,0,booked,s1.fcf,s1.scf);
		  	value3.dsize=strlen(valuebuf3)+1;
    		  	value3.dptr=valuebuf3;
			gdbm_store(db3,key3,value3,GDBM_INSERT);
		  	gdbm_close(db3);
			printf("\nBooking for %d seats is successful,Thank you\nyour booking id=%d ",booked,b.id);
		     }
			//storing to waitinglist db
			struct booking b;
			if(rem>0){
				cout<<"\nremaining seats to be in waiting list="<<rem<<endl;
				int maxid=0;
    		  		db3=gdbm_open("WAITINGLIST.db",0,GDBM_WRCREAT,0666,0);
				key3=gdbm_firstkey(db3);
				if(!key3.dptr)//no waiting list in db
					maxid=3000;
				else{//find last waitlist id
					while(key3.dptr){
    	   	 			  sscanf(key3.dptr,"%d",&b.id);
					  if(b.id>maxid)maxid=b.id;
        	 			  datum nextkey;
       		 			  nextkey=gdbm_nextkey(db3,key3);
       		 			  free(key3.dptr);
       		 			  key3=nextkey;
					}
				}
				b.id=++maxid;
    	  	  		sprintf(keybuf3,"%d",b.id);
    	 	  		key3.dsize=strlen(keybuf3)+1;
    	 	  		key3.dptr=keybuf3;
				value3=gdbm_fetch(db3,key3);
   		  		if(type==1) sprintf(valuebuf3,"%s %d %d %d %s %d %s %s %d %d %d %d",us.uname,b1.dt.day,b1.dt.month,b1.dt.year,b1.trname,b1.quotanum,b1.start,b1.stop,rem,0,s1.fcf,s1.scf);
   		  		else sprintf(valuebuf3,"%s %d %d %d %s %d %s %s %d %d %d %d",us.uname,b1.dt.day,b1.dt.month,b1.dt.year,b1.trname,b1.quotanum,b1.start,b1.stop,0,rem,s1.fcf,s1.scf);
		  		value3.dsize=strlen(valuebuf3)+1;
    		  		value3.dptr=valuebuf3;
				gdbm_store(db3,key3,value3,GDBM_INSERT);
		  		gdbm_close(db3);
				if(booked==0){
					//pgm not entered to if(booked>0) block then db1&db2 would not be closed
		  			gdbm_close(db2);
		  			gdbm_close(db1);
				}
				printf("\nyou are in waiting list for remaining %d tickets\nyour waitlist id=%d",rem,b.id);
			}	
		   
	 	}
		return;
}

void User::cancel(User us){
	struct booking b;
	int op;
	cout<<"\n1:booking id \t\t2:waitlist id:";
	cin>>op;
	if(op==1){
		cout<<"\nEnter your booking id: ";
		cin>>b.id;
		GDBM_FILE db1;
		datum key1,value1;
		char keybuf1[100],valuebuf1[200];
    		db1=gdbm_open("BOOKING.db",0,GDBM_WRCREAT,0666,0);
    		sprintf(keybuf1,"%d",b.id);
    		key1.dsize=strlen(keybuf1)+1;
    		key1.dptr=keybuf1;
		if(!gdbm_exists(db1,key1)){cout<<"No booking exist";gdbm_close(db1);return;}
		else{
			value1=gdbm_fetch(db1,key1);
   		  	sscanf(value1.dptr,"%s %d %d %d %s %d %s %s %d %d %d %d",b.uname,&b.dt.day,&b.dt.month,&b.dt.year,b.trname,&b.quotanum,b.start,b.stop,&b.fcsbooked,&b.scsbooked,&b.fcf,&b.scf);
			gdbm_close(db1);
			if(strcmp(b.uname,us.uname)!=0){cout<<"\nName doesnt match.Not your booking id";return;}
			cout<<"\nstarting station:"<<b.start<<"\ndestination station:"<<b.stop;
			//return money to user
			int retmoney=b.fcsbooked*b.fcf+b.scsbooked*b.scf;
			cout<<endl<<"returning money="<<retmoney;
			GDBM_FILE db2,db3;
			datum key2,value2,key3,value3;
			char keybuf2[100],valuebuf2[200],keybuf3[100],valuebuf3[200];
    			db2=gdbm_open("WALLET.db",0,GDBM_WRCREAT,0666,0);
			sprintf(keybuf2,"%s",us.uname);
    			key2.dsize=strlen(keybuf2)+1;
    			key2.dptr=keybuf2;
			value2=gdbm_fetch(db2,key2);
			int balance;
			sscanf(value2.dptr,"%d",&balance);
			sprintf(valuebuf2,"%d",balance+retmoney);
    			value2.dsize=strlen(valuebuf2)+1;
    			value2.dptr=valuebuf2;
			gdbm_store(db2,key2,value2,GDBM_REPLACE);
			gdbm_close(db2);
			//substract money from admin
			char ad[20]="admin";
			db2=gdbm_open("ADMINWALLET.db",0,GDBM_WRCREAT,0666,0);
			sprintf(keybuf2,"%s",ad);
    			key2.dsize=strlen(keybuf2)+1;
    			key2.dptr=keybuf2;
			value2=gdbm_fetch(db2,key2);
			sscanf(value2.dptr,"%d",&balance);
			sprintf(valuebuf2,"%d",balance-retmoney);
    			value2.dsize=strlen(valuebuf2)+1;
    			value2.dptr=valuebuf2;
			gdbm_store(db2,key2,value2,GDBM_REPLACE);
			gdbm_close(db2);
			//update seat db
    			db2=gdbm_open("SEAT.db",0,GDBM_WRCREAT,0666,0);
    		 	sprintf(keybuf2,"%d %d %d %s %d",b.dt.day,b.dt.month,b.dt.year,b.trname,b.quotanum);
    		 	key2.dsize=strlen(keybuf2)+1;
    		 	key2.dptr=keybuf2;
			struct seat s1;
			int flag;
			value2=gdbm_fetch(db2,key2);
   			sscanf(value2.dptr,"%s %s %d %d %d %d",s1.start,s1.stop,&s1.fcs,&s1.scs,&s1.fcf,&s1.scf);
			if(b.scsbooked==0)sprintf(valuebuf2,"%s %s %d %d %d %d",s1.start,s1.stop,s1.fcs+b.fcsbooked,s1.scs,s1.fcf,s1.scf);
			if(b.fcsbooked==0)sprintf(valuebuf2,"%s %s %d %d %d %d",s1.start,s1.stop,s1.fcs,s1.scs+b.scsbooked,s1.fcf,s1.scf);
    			value2.dsize=strlen(valuebuf2)+1;
    			value2.dptr=valuebuf2;
			gdbm_store(db2,key2,value2,GDBM_REPLACE);
			gdbm_close(db2);
			//removing from booking db
    			db2=gdbm_open("BOOKING.db",0,GDBM_WRCREAT,0666,0);
    		 	sprintf(keybuf2,"%d",b.id);
    		 	key2.dsize=strlen(keybuf2)+1;
    		 	key2.dptr=keybuf2;
			gdbm_delete(db2,key2);
			gdbm_close(db2);
			
			//storing cancelled ticket details
			db2=gdbm_open("CANCELREPORT.db",0,GDBM_WRCREAT,0666,0);
    		 	sprintf(keybuf2,"%d",b.id);
    		 	key2.dsize=strlen(keybuf2)+1;
    		 	key2.dptr=keybuf2;
   			sprintf(valuebuf2,"%s %d %d %d %s %d %s %s %d %d %d %d",us.uname,b.dt.day,b.dt.month,b.dt.year,b.trname,b.quotanum,s1.start,s1.stop,b.fcsbooked,b.scsbooked,s1.fcf,s1.scf);
    			value2.dsize=strlen(valuebuf2)+1;
    			value2.dptr=valuebuf2;
			gdbm_store(db2,key2,value2,GDBM_INSERT);
			gdbm_close(db2);	
			cout<<"\nYour ticket cancelled successfully";
		}
	}//end of if op==1
	else if(op==2){	
		struct booking b;
		cout<<"\nEnter your waitlist id: ";
		cin>>b.id;
		GDBM_FILE db1;
		datum key1,value1;
		char keybuf1[100],valuebuf1[200];
    		db1=gdbm_open("WAITINGLIST.db",0,GDBM_WRCREAT,0666,0);
    		sprintf(keybuf1,"%d",b.id);
    		key1.dsize=strlen(keybuf1)+1;
    		key1.dptr=keybuf1;
		if(!gdbm_exists(db1,key1)){cout<<"\nNo waitlist exist";gdbm_close(db1);return;}
		else{
			value1=gdbm_fetch(db1,key1);
   		  	sscanf(value1.dptr,"%s %d %d %d %s %d %s %s %d %d %d %d",b.uname,&b.dt.day,&b.dt.month,&b.dt.year,b.trname,&b.quotanum,b.start,b.stop,&b.fcsbooked,&b.scsbooked,&b.fcf,&b.scf);
			if(strcmp(b.uname,us.uname)!=0){cout<<"\nName doesnt match.Not your waiting list id";gdbm_close(db1);return;}
			cout<<"\nstarting station: "<<b.start<<"\ndestination station: "<<b.stop;
			gdbm_delete(db1,key1);
			gdbm_close(db1);
			cout<<"\nyour waitlist cancelled successfully";
		}
	}
	else cout<<"\nInvalid option";	
	return;
}		

