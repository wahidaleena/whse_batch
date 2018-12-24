#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>

struct Data
{
    char *i_num;
    char *m_stat;
    char *s_loc;
};

void getWareHouseAccess(char *DB_NAME, char *USER, char *PASS, char *TABLE_NAME, char *FILENAME)
{
    strcpy(DB_NAME, "wareHouse_Batch");
    strcpy(USER, "user@whse_rcpt");
    strcpy(PASS, "accessSpecify");
    strcpy(TABLE_NAME, "wareHouseTable");
    strcpy(FILENAME, "N01.Z4");
}

int connect2Mysql(MYSQL *mysql, char *DB_NAME, char *USER, char *PASS, char *TABLE_NAME)
{
    int status;
    mysql = mysql_init(NULL);
    status = mysql_real_connect(mysql, TABLE_NAME, USER, PASS, DB_NAME, 0, 0, 0);
    return status;
}

void updateDatatoDB(MYSQL *mysql, FILE *myfile, struct Data *d, int i)
{
    int j = 0;
    while (j<i)
    {
        char * i_num = d[j].i_num;
        char * stat = d[j].m_stat;
        mysql_query(mysql, "UPDATE DB_NAME SET movement_status = stat WHERE item_nbr = i_num");
        fprintf(myfile, "UPDATE DB_NAME SET movement_status = %s WHERE item_nbr = %s", stat, i_num);
        j++;
    }
}

int readDataFromFile(FILE *myfile, struct Data *d)
{
    int i=0;
    size_t s;
    while (myfile)
    {
        getline(&(d[i].i_num), &s, myfile);
        getline(&(d[i].m_stat), &s, myfile);
        getline(&(d[i++].s_loc), &s, myfile);
    }

    return i;
}

int main () {

    FILE *fp;
    struct Data d[100];
    char DB_NAME[1000], USER[1000], PASS[1000], TABLE_NAME[1000], FILENAME[1000];
    int status;

    getWareHouseAccess(DB_NAME, USER, PASS, TABLE_NAME, FILENAME);

    fp = fopen(FILENAME, "w");

    int i = readDataFromFile(fp, d);

    MYSQL mysql;
    status = connect2Mysql(mysql, USER, PASS, DB_NAME);

    if(status)
    {
        updateDatatoDB(mysql, d, i);
    }

    fclose(fp);
    return 0;
}

