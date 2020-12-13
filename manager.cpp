#include <iostream>
#include <cstdio>
#include <gdbm.h>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "manager.hpp"
#include "train.hpp"
using namespace std;

Manager::Manager(){}
Manager::Manager(char *n,char *p){
	strcpy(name,n);
	strcpy(password,p);
}
void Manager::addTrain(){
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

void Manager::modifyTrain(){
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

void Manager::deleteTrain(){
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
	//deleting train 
    	gdbm_delete(db,key);  
    	printf("\nRecord deleted");
    	gdbm_close(db);
    	return;
    }
}

void Manager::setQuota(){
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
			printf("\n\t\t\tstarting station=%s\tdestination=%s\n\t\t\tEnter seat details",s1.start,s1.stop);
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
void Manager::viewbalance(){
	char ad[20]="admin";
    	datum key, value;
    	char keybuf[100],valuebuf[256];
	GDBM_FILE db=gdbm_open("ADMINWALLET.db",0,GDBM_WRCREAT,0666,0);
	sprintf(keybuf,"%s",ad);
	key.dsize=strlen(keybuf)+1;
    	key.dptr=keybuf;
	int ret1=gdbm_exists(db,key);
	if(ret1==0) printf("\namount=0");
	else{
		int amt;
		value=gdbm_fetch(db,key);
		sscanf(value.dptr,"%d",&amt);
		printf("\namount=%d",amt);
	}
	gdbm_close(db);
	return;
}
void Manager::viewDatabase(char *dbname){
	int i=1;
	GDBM_FILE db;
    	datum key, value;
    	db=gdbm_open(dbname,0,GDBM_WRCREAT,0666,0);
	key=gdbm_firstkey(db);
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

