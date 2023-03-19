#include <stdio.h>
#include <string.h>
#include <libpq-fe.h>
#include <stdlib.h>
#include <ctype.h>

void do_exit(PGconn *conn) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    
    PQfinish(conn);    
    
    exit(1);
}
int main()
{
    int i;
    PGconn *conn;
    conn = PQconnectdb("dbname=VideoStream host=localhost user=duy password=duyproxy1");
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    PGresult *res = PQexec(conn, "SELECT path_to_video FROM video WHERE video_id = 1");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        do_exit(conn);
        PQclear(res);
    }    
    int rows = PQntuples(res);
    int count = 0;
    for(i = 0; i < rows; i++)
    {
        printf("%s", PQgetvalue(res, i, 0));
    }
}