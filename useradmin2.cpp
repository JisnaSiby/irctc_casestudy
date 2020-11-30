#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
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
class Train{
    private:
        char name[25], start[25], dest[25];
	struct date dat;
	struct time tim;
	vector<char*> inter;
    public:
        Train(){}
        Train(char *n,struct date dt,struct time ti,char *s, char *d,vector<char*> i){
            strcpy(name,n);
            strcpy(start,s);
            strcpy(dest,d);
	    inter=i;
	    dat=dt;
	    tim=ti;
        }
	void setName(char *n){strcpy(name,n);}
        void setStart(char *s){strcpy(start,s);}
        void setDest(char *d){strcpy(dest,d);}
        void setInter(vector<char*> i){inter=i;}
        char *getName(){return name;}
        char *getStart(){return start;}
        char *getDest(){return dest;}
        vector<char*> getInter(){return inter;}
	friend class Manager;
};
class Manager{
private:
	char name[20];
	char password[20];
public:
Manager(){}
Manager(char *n,char *p){
	strcpy(name,n);
	strcpy(password,p);
}
void addTrain(){
    char name[25], st[25],dest[25], station[3][25];
    string str;
    int count;
    struct date dt;
    struct time ti;
    vector<char*> inter={};
    printf("\n\t\tEnter Train details: ");
    printf("\n\t\t\tName:");
    cin.ignore();cin.get(name,25);
    printf("\n\t\t\tEnter the Date:");
    printf("\n\t\t\t\tDay:");scanf("%d",&dt.day);
    printf("\n\t\t\t\tmonth:");scanf("%d",&dt.month);
    printf("\n\t\t\t\tyear:");scanf("%d",&dt.year);
    printf("\n\t\t\tEnter the time:");
    printf("\n\t\t\t\thour:");scanf("%d",&ti.hour);
    printf("\n\t\t\t\tminute:");scanf("%d",&ti.minute);
    printf("\t\t\tStarting station: "); scanf("%s",st);
    printf("\t\t\tDestination station: "); scanf("%s",dest);
    printf("\n\t\t\tEnter the no:of intermediate stations(max=3): ");scanf("%d",&count);
    if(count>3 || count<0){printf("\n\t\tError");exit(0);}
    if(count==0){inter.push_back(NULL);inter.push_back(NULL);inter.push_back(NULL);}
    else{
    	for(int i=0;i<count;i++){
		printf("\t\t\tEnter the intermediate station %d: ",i+1);
    		scanf("%s",station[i]);
		inter.push_back(station[i]);
    	}
    }
    printf("__________________________________________________________________________");
    Train t(name,dt,ti,st,dest,inter);
    //adding to database
    GDBM_FILE db;
    datum key,value;
    char keybuf[100],valuebuf[256];
    db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    sprintf(keybuf,"%d %d %d %s",dt.day,dt.month,dt.year,name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    sprintf(valuebuf,"%d %d %s %s %s %s %s",ti.hour,ti.minute,st,dest,inter[0],inter[1],inter[2]);
    printf("\n%s %s",keybuf,valuebuf);
    value.dsize=strlen(valuebuf)+1;
    value.dptr=valuebuf;
    gdbm_store(db,key,value, GDBM_INSERT);
    printf("\nRecord added");
    gdbm_close(db);
    return;
}

void modifyTrain(){
    char name[25], st[25],dest[25], station[3][25],tmpstation[3][25];
    int count,op;
    vector<char*> inter={};
    struct date dt;
    struct time ti;
    printf("\n\t\t\tEnter name of the train to be modified: ");
    cin.ignore(); cin.get(name,25);
    printf("\n\t\t\tEnter the Date:");
    printf("\n\t\t\t\tDay:");scanf("%d",&dt.day);
    printf("\n\t\t\t\tmonth:");scanf("%d",&dt.month);
    printf("\n\t\t\t\tyear:");scanf("%d",&dt.year);	
   //access
    GDBM_FILE db;
    datum key,value;
    char keybuf[100],valuebuf[256];
    db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    sprintf(keybuf,"%d %d %d %s",dt.day,dt.month,dt.year,name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    int ret=gdbm_exists(db,key);
    if(ret==0){printf("\n\t\t\tTrain doesnot exist:");gdbm_close(db);return;}
    else{
      value=gdbm_fetch(db,key);
      sscanf(value.dptr,"%d %d %s %s %s %s %s",&ti.hour,&ti.minute,st,dest,tmpstation[0],tmpstation[1],tmpstation[2]);
      printf("\n\t\t\tcurrent details:%s",value.dptr);
      printf("\n\t\t\tEnter the starting station: ");scanf("%s",st);
      printf("\n\t\t\tEnter the destination station: ");scanf("%s",dest);
      printf("\n\t\t\tEnter the no:of intermediate stations(max=3): ");scanf("%d",&count);
      if(count>3 || count<0){printf("\n\t\tError");exit(0);}
      if(count==0){inter.push_back(NULL);inter.push_back(NULL);inter.push_back(NULL);}
      else{
  	for(int i=0;i<count;i++){
		printf("\t\t\tEnter the intermediate station %d: ",i+1);
    		scanf("%s",station[i]);
		inter.push_back(station[i]);
        }
      }
      Train t(name,dt,ti,st,dest,inter);
      printf("__________________________________________________________________________");
      //replacing
      sprintf(keybuf,"%d %d %d %s",dt.day,dt.month,dt.year,name);
      key.dsize=strlen(keybuf)+1;
      key.dptr=keybuf;
      sprintf(valuebuf,"%d %d %s %s %s %s %s",ti.hour,ti.minute,st,dest,inter[0],inter[1],inter[2]);
      printf("\n%s %s",keybuf,valuebuf);
      value.dsize=strlen(valuebuf)+1;
      value.dptr=valuebuf;
      gdbm_store(db,key,value, GDBM_REPLACE);
      printf("\nRecord modified");
      gdbm_close(db);
      return;
    }
}

void deleteTrain(){
    char name[25];
    struct date dt;
    printf("\nEnter name of the train to be deleted:");
    cin.ignore();cin.get(name,25);
    printf("\n\t\t\tEnter the Date:");
    printf("\n\t\t\t\tDay:");scanf("%d",&dt.day);
    printf("\n\t\t\t\tmonth:");scanf("%d",&dt.month);
    printf("\n\t\t\t\tyear:");scanf("%d",&dt.year);	
    GDBM_FILE db;
    datum key,value;
    char keybuf[100],valuebuf[256];
    db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    sprintf(keybuf,"%d %d %d %s",dt.day,dt.month,dt.year,name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    int ret=gdbm_exists(db,key);
    if(ret==0){printf("\n\t\t\tTrain doesnot exist:");gdbm_close(db);return;}
    else{
	//delete all quota
   	 GDBM_FILE db1;
   	 datum key1,value1;
   	 char keybuf1[100],valuebuf1[256];
   	 db1=gdbm_open("QUOTA.db",0,GDBM_WRCREAT,0666,0);
	for(int i=1;i<11;i++){
   	 sprintf(keybuf1,"%d %d %d %s %d",dt.day,dt.month,dt.year,name,i);
   	 key1.dsize=strlen(keybuf1)+1;
   	 key1.dptr=keybuf1;
    	 gdbm_delete(db1,key1);        
	}
    	gdbm_close(db1);
	//deleting train 
    	gdbm_delete(db,key);        
    	printf("\nRecord deleted");
    	gdbm_close(db);
    	return;
    }
}

void displayTrain(){
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


void setQuota(){
    	char name[25],station[4][25];
    	struct time ti;
	struct date dt;
	int k=1;
    	printf("\n\t\t\tEnter name of the train:");
    	cin.ignore();cin.get(name,25);
    	printf("\n\t\t\tEnter the Date:");
    	printf("\n\t\t\t\tDay:");scanf("%d",&dt.day);
   	printf("\n\t\t\t\tmonth:");scanf("%d",&dt.month);
   	printf("\n\t\t\t\tyear:");scanf("%d",&dt.year);	
    	GDBM_FILE db;
    	datum key,value;
    	char keybuf[100],valuebuf[256];
    	db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    	sprintf(keybuf,"%d %d %d %s",dt.day,dt.month,dt.year,name);
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
    	int ret=gdbm_exists(db,key);
    	if(ret==0){printf("\n\t\t\tTrain doesnot exist:");gdbm_close(db);return;}
    	else{
    	  value=gdbm_fetch(db,key);
    	  sscanf(value.dptr,"%d %d %s %s %s %s %s",&ti.hour,&ti.minute,station[0],station[4],station[1],station[2],station[3]);
	  struct seat s1;
	  GDBM_FILE db1,db2;
    	  datum key1,value1;
    	  char keybuf1[100],valuebuf1[256];
    	  db1=gdbm_open("QUOTA.db",0,GDBM_WRCREAT,0666,0);
	  db2=gdbm_open("SEAT.db",0,GDBM_WRCREAT,0666,0);
	  for(int i=0;i<4;i++){
		for(int j=i+1;j<5;j++){
		  if(strcmp(station[i],"(null)")!=0 && strcmp(station[j],"(null)")!=0){
			s1.quota=k++;
			strcpy(s1.tname,name);
			strcpy(s1.start,station[i]);
			strcpy(s1.stop,station[j]);
			printf("\n\t\t\tstarting stop=%s\tdestination=%s\n\t\t\tEnter seat details",s1.start,s1.stop);
			printf("\n\t\t\tEnter the no:of 1st class seat: ");scanf("%d",&s1.fcs);
			printf("\n\t\t\tEnter the no:of 2nd class seat: ");scanf("%d",&s1.scs);
			printf("\n\t\t\tEnter the fare of 1st class seat: ");scanf("%d",&s1.fcf);
			printf("\n\t\t\tEnter the fare of 2nd class seat: ");scanf("%d",&s1.scf);
    			sprintf(keybuf1,"%d %d %d %s %d",dt.day,dt.month,dt.year,s1.tname,s1.quota);
    			key1.dsize=strlen(keybuf1)+1;
    			key1.dptr=keybuf1;
    			sprintf(valuebuf1,"%s %s %d %d %d %d",s1.start,s1.stop,s1.fcs,s1.scs,s1.fcf,s1.scf);
   			printf("\n%s",valuebuf1);
   			value1.dsize=strlen(valuebuf1)+1;
    			value1.dptr=valuebuf1;
   			gdbm_store(db1,key1,value1, GDBM_INSERT);
			gdbm_store(db2,key1,value1,GDBM_INSERT);
   			printf("\nsuccefully added quota %d",k-1);
			}
		}
	    }
	  gdbm_close(db);
	  gdbm_close(db1);
	  gdbm_close(db2);
	  }
}
void viewQuota(){
	int i=1;
	GDBM_FILE db;
    	datum key, value;
    	db=gdbm_open("QUOTA.db",0,GDBM_WRCREAT,0666,0);
	key=gdbm_firstkey(db);
	printf("\n\t\t\tAll the Quota details are given below\n");
   	while(key.dptr){
    	   	 value=gdbm_fetch(db,key);
   		 printf("\n\t\t\t %s %s",key.dptr,value.dptr);
        	 datum nextkey;
       		 nextkey=gdbm_nextkey(db,key);
       		 free(key.dptr);
       		 key=nextkey;
	}
	gdbm_close(db);
	}
};
class User:public Manager{
        private:
		 char uname[40],pwd[40],fname[50],lname[50],email[50],gender[20];
                 int age;
                long int mobno;
		int wallet;
        public:
		User(){}
		User(char *n,char *p){strcpy(uname,n);strcpy(pwd,p);}
		User(char *n,char *p,char *fn,char *ln,int a,char *gen,char *em,long int mob){}
void user_reg(){
        GDBM_FILE dbf;
        datum key,data;
        char pass[50];
        char keybuf[500],databuf[500];
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
User user_login(){
	int flag=0;
        GDBM_FILE dbf;
        char pass[50],uname[50];
        datum key, data;
        cout<<"\n\t\tUser Name: ";
        cin>>uname;
        cout<<"\t\tPassword : ";
        cin>>pass;
	User us(uname,pass);
	char u[50],p[50];
        dbf=gdbm_open("userlogin.db",0,GDBM_WRCREAT,0666,0);
        key=gdbm_firstkey(dbf);
	sscanf(key.dptr,"%s %s",u,p);
        while(key.dptr){
                data=gdbm_fetch(dbf,key);
                if(strcmp(u,uname)==0 && strcmp(p,pwd)==0){
                        flag=1;
                }
                datum nextkey;
                nextkey=gdbm_nextkey(dbf,key);
                free(key.dptr);
                key=nextkey;
        }
        if(flag==1)
                cout<<"Login successfull"<<endl;
        else
                cout<<"Invalid Username or Password"<<endl;

        gdbm_close(dbf);
        return us;
}

void modify(User us){
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
void setwallet(User us){
	int walletextra;
        GDBM_FILE dbf;
        datum key, data;
        char pass[25];
        char keybuf[50],databuf[256];
        cout<<"\n\t\t\tEnter the depositing amount: ";
        cin>>walletextra;
        dbf=gdbm_open("wallet.db",0,GDBM_WRCREAT,0666,0);
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

void viewTrain(){
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

void bookTrain(User us){
    	printf("\nEnter train name:");
    	char trname[50];
	cin>>trname;
	printf("\nEnter the date:");
    	GDBM_FILE db;
    	datum key,value;
    	char keybuf[100],valuebuf[256];
    	db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    	sprintf(keybuf,"%s",trname);
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
    	int ret=gdbm_exists(db,key);
    	if(ret==0){printf("\n\t\t\tTrain doesnot exist:");gdbm_close(db);return;}
    	else{
	int op;
   	value=gdbm_fetch(db,key);
    	printf("\nExist");
    	printf("\n\t\t\t%s %s",trname,value.dptr);
    	printf("\npress 1 for booking the above train 0 to return to previous page");
    	scanf("%d",&op);
	if(op==1){
  	char type[25],stt[25],destn[25];
    	int seatno;
	struct date dt;
	struct time ti;
	char st[20],dest[20],inter[2][20];
    	sscanf(value.dptr,"%d %d %d %d %d %s %s %s %s %s",&dt.day,&dt.month,&dt.year,&ti.hour,&ti.minute,st,dest,inter[0],inter[1],inter[2]);
    	cout<<"\n\t\tEnter the starting station: ";
    	cin>>stt;
    	cout<<"\n\t\tEnter the destination station: ";
    	cin>>destn;
    	cout<<"\n\t\tEnter the class(first/second): ";
    	cin>>type;
    	cout<<"\n\t\tEnter the number of seats: ";
    	cin>>seatno;
    	/*GDBM_FILE db1;
    	datum key1,value1;
    	char keybuf1[100],valuebuf1[256];
    	db=gdbm_open("SEAT.db",0,GDBM_WRCREAT,0666,0);
	for(int i=1;i<=10;i++){
    	  sprintf(keybuf1,"%d %d %d %s %d",dt.day,dt.month,dt.year,trname,i);
    	  key1.dsize=strlen(keybuf1)+1;
    	  key1.dptr=keybuf1;
	  ret=gdbm_exist(db1,key1);
	  if(ret==1){
		struct seat s1;
		value1=gdbm_fetch(db1,key1);
   		sscanf(value1.dptr,"%s %s %d %d %d %d",s1.start,s1.stop,&s1.fcs,&s1.scs,&s1.fcf,&s1.scf);
   		printf("\n%s",valuebuf1);
		
   		value1.dsize=strlen(valuebuf1)+1;
    		value1.dptr=valuebuf1;
   		gdbm_store(db1,key1,value1, GDBM_INSERT);
		gdbm_store(db2,key1,value1,GDBM_INSERT);
	

        return;*/
	}
    gdbm_close(db);
    }
}
};
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
                         cout<<"\n\t\t1.Modify Record\t\t2.Add money to wallet\t\t3:view trains\t\t4.booking\t\t5.exit"<<endl;
			cout<<"\t\tEnter your choice: ";cin>>opt;
                        if(opt==1)
                                ul.modify(us);
                        else if(opt==2)
                                ul.setwallet(us);
			else if(opt==3)ul.viewTrain();
                       //else if(opt==4)ul.bookTrain(us);
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
    printf("\n\t\tPress 1: add train, 2: modify train, 3: delete train, 4: display train, 5:set quota for a train, 6:view quota 7:Generate report 0: quit\n");
    while(true){
	printf("\nEnter the option: ");
        scanf("%d",&choice);
        if(choice==1)m1.addTrain(); 
        else if(choice==2)m1.modifyTrain();
        else if(choice==3)m1.deleteTrain();
        else if(choice==4)m1.displayTrain();
        else if(choice==5)m1.setQuota();
        else if(choice==6)m1.viewQuota();
//	else if(choice==6)m1.report();
        else break;
    }
    return 0;
}
