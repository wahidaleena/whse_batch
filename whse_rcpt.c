#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <conio.h>

struct Data
{
    char i_num;	
    char m_stat;
    char s_loc;
};

int main () {
  
	FILE *fp;
	struct Data d[100];
	char DB_NAME[1000],USER[1000],PASS[1000],TABLE_NAME[1000],FILEN[1000];
	bool status;

   getWareHouseAccess(DB_NAME,USER,PASS,TABLE_NAME,FILEN);

	myfile=fopen("wareHouseContent.csv", "w");


	MYSQL mysql;
	status = connect2Mysql(mysql,USER, PASS, DB_NAME);
	
	if(status== True)
	{
		updateDatatoDB(mysql,d);
		fprintf(fp, "UPDATE TABLE_NAME SET movement_status = %s WHERE item_nbr = %s", stat,i_num);
	}
	else
	{
		updateDatatoFile(fp,d);
	}	


	fclose(fp);
	return 0;
}

bool connect2Mysql(MYSQL mysql, char USER, char PASS, char DB_NAME)
{
	bool status;
	mysql = mysql_init(NULL);
	mysql_real_connect(mysql, "WareHouseItem_Server", USER, PASS, DB_NAME, 0, 0, 0);
	return status;
}

void getWareHouseAccess(char DB_NAME,char USER,char PASS,char TABLE_NAME,char FILEN)
{
	strcpy(DB_NAME, getenv("wareHouseServer"));
	strcpy(USER, getenv("USERNAME"));
	strcpy(PASS, getenv("PASSWORD"));
	strcpy(TABLE_NAME, getenv("wareHouseTable"));
	strcpy(FILENAME, getenv("FILENAME"));
}

void updateDatatoDB(MYSQL mysql,FILE *myfile,Data d)
{
	int j = 0;
	while (j<i)
	{
		string i_num = d[j].i_num;
		string stat = d[j].m_stat;
		mysql_query(mysql, "UPDATE TABLE_NAME SET movement_status = stat WHERE item_nbr = i_num");
		j++;
	}
	updateDatatoFile(myfile,d);
	
}

void updateDatatoFile(FILE *myfile,Data d)
{
	int i=0;
	
	while (myfile)
	{
		getline(myfile, d[i].i_num, ',');
		getline(myfile, d[i].m_stat, ',');
		getline(myfile, d[i++].s_loc, '\n');
	}
}
