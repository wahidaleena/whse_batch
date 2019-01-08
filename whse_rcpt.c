#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>

/*
typedef int MYSQL;
extern MYSQL *mysql_init(MYSQL *mysql);
extern MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag);
extern unsigned int mysql_errno(MYSQL *mysql);
extern int mysql_query(MYSQL *mysql, const char *stmt_str);
//*/

struct Data
{
    char *i_num;
    char *m_stat;
    char *s_loc;
};

void getWareHouseAccess(char *DB_NAME,char *HOST, char *USER, char *PASS, char *TABLE_NAME, char *FILENAME)
{
    strcpy(DB_NAME, "warehouse_receipts");
    strcpy(HOST, "WHSE_01");
    strcpy(USER, "user");
    strcpy(PASS, "accessSpecify");
    strcpy(TABLE_NAME, "Received_Items");
    strcpy(FILENAME, "N01.Z4");
}

int connect2Mysql(MYSQL **mysql, char *HOST, char *USER, char *PASS, char *DB_NAME, char *TABLE_NAME)
{
    *mysql = mysql_init(NULL);
    mysql_real_connect(*mysql, HOST, USER, PASS, DB_NAME, 0, 0, 0);
    return mysql_errno(*mysql);
}

void updateDatatoDB(MYSQL *mysql, FILE *myfile, struct Data *d, int i)
{
    int j = 0;
    while (j<i)
    {
        char * i_num = d[j].i_num;
        char * stat = d[j].m_stat;
        mysql_query(mysql, "UPDATE Received_Items  SET movement_status = stat WHERE item_nbr = i_num");
        fprintf(myfile, "UPDATE Received_Items  SET movement_status = %s WHERE item_nbr = %s", stat, i_num);
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

    getWareHouseAccess(DB_NAME, HOST, USER, PASS, TABLE_NAME, FILENAME);

    fp = fopen(FILENAME, "w");

    int i = readDataFromFile(fp, d);

    MYSQL *mysql;
    status = connect2Mysql(&mysql, HOST, USER, PASS, DB_NAME, TABLE_NAME);

    if(status)
    {
        updateDatatoDB(mysql, fp, d, i);
    }

    fclose(fp);
    return 0;
}
