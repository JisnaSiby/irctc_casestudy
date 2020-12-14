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
	int quota;
	char tname[25];
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
    sprintf(keybuf,"%s",name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    sprintf(valuebuf,"%d %d %d %d %d %s %s %s %s %s",dt.day,dt.month,dt.year,ti.hour,ti.minute,st,dest,inter[0],inter[1],inter[2]);
    printf("\n%s %s",t.name,valuebuf);
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
   //access
    GDBM_FILE db;
    datum key,value;
    char keybuf[100],valuebuf[256];
    db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    sprintf(keybuf,"%s",name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    value=gdbm_fetch(db,key);
    sscanf(value.dptr,"%d %d %d %d %d %s %s %s %s %s",&dt.day,&dt.month,&dt.year,&ti.hour,&ti.minute,st,dest,tmpstation[0],tmpstation[1],tmpstation[2]);
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
    sprintf(keybuf,"%s",name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    sprintf(valuebuf,"%d %d %d %d %d %s %s %s %s %s",dt.day,dt.month,dt.year,ti.hour,ti.minute,st,dest,inter[0],inter[1],inter[2]);
    printf("\n%s %s",t.name,valuebuf);
    value.dsize=strlen(valuebuf)+1;
    value.dptr=valuebuf;
    gdbm_store(db,key,value, GDBM_REPLACE);
    printf("\nRecord modified");
    gdbm_close(db);
    return;
}

void deleteTrain(){
    char name[25];
    printf("\nEnter name of the train to be deleted:");
    cin.ignore();cin.get(name,25);
    GDBM_FILE db;
    datum key,value;
    char keybuf[100],valuebuf[256];
    db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    sprintf(keybuf,"%s",name);
    key.dsize=strlen(keybuf)+1;
    key.dptr=keybuf;
    gdbm_delete(db,key);        
    printf("\nRecord deleted");
    gdbm_close(db);
    return;
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
    	GDBM_FILE db;
    	datum key,value;
    	char keybuf[100],valuebuf[256];
    	db=gdbm_open("TRMS.db",0,GDBM_WRCREAT,0666,0);
    	sprintf(keybuf,"%s",name);
    	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
    	value=gdbm_fetch(db,key);
    	sscanf(value.dptr,"%d %d %d %d %d %s %s %s %s %s",&dt.day,&dt.month,&dt.year,&ti.hour,&ti.minute,station[0],station[4],station[1],station[2],station[3]);
	struct seat s1;
	GDBM_FILE db1;
    	datum key1,value1;
    	char keybuf1[100],valuebuf1[256];
    	db1=gdbm_open("QUOTA.db",0,GDBM_WRCREAT,0666,0);
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
    			sprintf(keybuf1,"%d",s1.quota);
    			key1.dsize=strlen(keybuf1)+1;
    			key1.dptr=keybuf1;
    			sprintf(valuebuf1,"%s %s %s %d %d %d %d",s1.tname,s1.start,s1.stop,s1.fcs,s1.scs,s1.fcf,s1.scf);
   			printf("\n%d %s",s1.quota,valuebuf1);
   			value1.dsize=strlen(valuebuf1)+1;
    			value1.dptr=valuebuf1;
   			gdbm_store(db1,key1,value1, GDBM_INSERT);
   			printf("\nsuccefully added quota %d",k-1);
			}
		}
	}
	gdbm_close(db1);
	}
};
int main(){
    int choice,op;
    char u[20],pass[20];
    char n[20]="sahal";
    char p[20]="123";
    Manager m1(n,p);
    printf("\t\t\t1:User\t\t2:Admin:\n\t\t\tEnter the option: ");
    scanf("%d",&op);
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
    printf("\n\t\tPress 1: add train, 2: modify train, 3: delete train, 4: display train, 5:set quota for a train,6:Generate report 0: quit\n");
    while(true){
	printf("\nEnter the option: ");
        scanf("%d",&choice);
        if(choice==1)m1.addTrain(); 
        else if(choice==2)m1.modifyTrain();
        else if(choice==3)m1.deleteTrain();
        else if(choice==4)m1.displayTrain();
        else if(choice==5)m1.setQuota();
//	else if(choice==6)m1.report();
        else break;
    }
    return 0;
}
