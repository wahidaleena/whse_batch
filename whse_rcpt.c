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
        mysql_query(mysql, "SELECT COUNT (DISTINCT SIZE_DESC) INTO   ZERO FROM SSITMURX IRX, SSITMXRF IUX, MERTLDFT RTL WHERE  RTL.USERID  = XF_USERIDAND  RTL.TYPE =  'R'AND  IRX.CORP_ITEM_CD = MEX7_CORP_ITEM_CD AND  IRX.UNIT_TYPE    = MEX7_UNIT_TYPE AND  IRX.UPC_MANUF    = PRX_CURR_UPC_MANUF AND  IRX.UPC_SALES    = PRX_CURR_UPC_SALES AND  IRX.UPC_COUNTRY  = PRX_CURR_UPC_COUNTRY AND  IRX.UPC_SYSTEM   = PRX_CURR_UPC_SYSTEM AND  RTL.PA_ROG       =  IRX.ROG AND  IUX.CORP_ITEM_CD =  IRX.CORP_ITEM_CD AND  IUX.UPC_MANUF    =  IRX.UPC_MANUF AND  IUX.UPC_SALES    =  IRX.UPC_SALES AND  IUX.UPC_COUNTRY  =  IRX.UPC_COUNTRY AND  IUX.UPC_SYSTEM   =  IRX.UPC_SYSTEM AND (IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS01 <> ' ' THEN WS_ROGS_WS_ROGS01 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS02 <> ' ' THEN WS_ROGS_WS_ROGS02 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS03 <> ' ' THEN WS_ROGS_WS_ROGS03 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS04 <> ' ' THEN WS_ROGS_WS_ROGS04 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS05 <> ' ' THEN WS_ROGS_WS_ROGS05 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS06 <> ' ' THEN WS_ROGS_WS_ROGS06 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS07 <> ' ' THEN WS_ROGS_WS_ROGS07 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS08 <> ' ' THEN WS_ROGS_WS_ROGS08 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS09 <> ' ' THEN WS_ROGS_WS_ROGS09 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS10 <> ' ' THEN WS_ROGS_WS_ROGS10 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS11 <> ' ' THEN WS_ROGS_WS_ROGS11 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS12 <> ' ' THEN WS_ROGS_WS_ROGS12 ELSE '    ' END OR  IRX.ROG = CASE WHEN WS_ROGS_WS_ROGS01 =  '    ' AND WS_ROGS_WS_ROGS02 =  '    ' AND WS_ROGS_WS_ROGS03 =  '    ' AND WS_ROGS_WS_ROGS04 =  '    ' AND WS_ROGS_WS_ROGS05 =  '    ' AND WS_ROGS_WS_ROGS06 =  '    ' AND WS_ROGS_WS_ROGS07 =  '    ' AND WS_ROGS_WS_ROGS08 =  '    ' AND WS_ROGS_WS_ROGS09 =  '    ' AND WS_ROGS_WS_ROGS10 =  '    ' AND WS_ROGS_WS_ROGS11 =  '    ' AND WS_ROGS_WS_ROGS12 =  '    ' THEN RTL.PA_ROG END) QUERYNO 5 ");
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
