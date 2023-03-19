#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <libpq-fe.h>

char *data_analysis(char *buffer, char * data_recv);
void getData(char *recv_data, char *data);
void reply_status(char * reply, int sockfd);
void sign_in(char *buffer, int sockfd);
void login(char *buffer, int sockfd);
void send_list_video(int sockfd);
void stream_video(char *buffer, int sockfd);
void phan_quyen_video(char *buffer, int sockfd);
void recv_video(char *buffer, int sockfd);
void delete_video(char *buffer, int sockfd);
int req_analysis(char* recv_mes);
void* conn_handler(void *sockfd);
void* run_engine(char* cmd);
PGconn* new_conn_todb();

//PGconn *conn;
int count_pthread = 0;
#define SIZE_MAX 1024
int port_stream = 3000;


int main(){
	//connection establish
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    // conn = new_conn_todb();
    if (server_socket == -1)
    {
        perror("Socket initialisation failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5500);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }	
    else
        printf("Socket successfully binded..\n");

    if (listen(server_socket, 3) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    int client_socket;
    printf("5500\n");

    char cmd[100] = "python3 ./vod_nodejs/serverside/video/app.py";
    pthread_t py_thread;
    if (pthread_create(&py_thread, NULL, run_engine, cmd) < 0)
    {
        perror("Could not create thread");
        return 1;
    }else
        printf("recv server created!!!\n");

	while(1){
		//accept connection

		pthread_t new_thread;
        printf("Listening...\n");
        client_socket = accept(server_socket, NULL, NULL);
        puts("Connection accepted");
        //printf("truoc khi tap pthread: %d\n", client_socket);
        if (pthread_create(&new_thread, NULL, conn_handler, &client_socket) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
        if (client_socket < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
            printf("Server acccept the client...\n");
        puts("Handler assigned");        // sprintf(tmp, "%s", recv_mes);
        // strcat(msg, tmp);
    }
	return 0;
}

// void do_exit(PGconn *conn) {
    
//     fprintf(stderr, "%s\n", PQerrorMessage(conn));    
//     PQfinish(conn);    
// }

PGconn* new_conn_todb()
{
    PGconn *conn = PQconnectdb("dbname=VideoStream host=localhost user=duy password=duyproxy1");
        if(PQstatus(conn) != CONNECTION_OK)
        {
            printf("connect unsuccessful!!!\n");
            PQfinish(conn);
        }
    return conn;
}


char *data_analysis(char *buffer, char * data_recv)
{
    data_recv = strtok(buffer, "{");
    data_recv = strtok(NULL, ",");
    data_recv = strtok(NULL, "}");
    return data_recv;
}

void getData(char *recv_data, char *data)
{
    int location = 0, i = 0;
    while (location < strlen(recv_data))
    {
        if(recv_data[location] == 58)
        {
            location = location + 2;
            break;
        }
        location++;
    }
    while (recv_data[location] != 34)
    {
        data[i] = recv_data[location];
        location ++;
        i++;
    }
    data[i] = '\0';
    printf("do dai data: %d\n", strlen(data));
    //return data;
}

void sign_in(char *buffer, int sockfd)
{
    //PQreset(conn);
    PGconn *conn = new_conn_todb();

    char *username = (char*) malloc(50*sizeof(char));
    char *pwd = (char*) malloc(50*sizeof(char));
    char *data_recv = (char*) malloc(1024*sizeof(char));
    char *tmp;
    strcpy(data_recv, data_analysis(buffer, data_recv));
    tmp = strtok(data_recv, ",");
    getData(tmp, username);
    pwd = strtok(NULL, ":");
    pwd = strtok(NULL, "\"");
    char query[255];
    char query2[255];
    sprintf(query, "INSERT INTO nguoidung(user_name, pwd) VALUES (\'%s\', \'%s\')", username, pwd); 
    char *addQuery = &query[0];
    PQexec(conn, addQuery);

    char body[1024];
    sprintf(query2, "SELECT user_id FROM nguoidung WHERE user_name = \'%s\'", username);
    char *addQuery2 = &query2[0];
    PGresult *res = PQexec(conn, addQuery2);
     if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
    }  
    sprintf(body, "{\"id_user\":\"%s\"}", PQgetvalue(res, 0,0));
    char msg[SIZE_MAX];
    sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body), body);
    send(sockfd, msg, strlen(msg), 0);
    PQfinish(conn);
}


void login(char * buffer, int sockfd)
{
    // PQreset(conn);
    PGconn *conn = new_conn_todb();

    //gia su xu ly xong buffer
    char *username = (char*) malloc(50*sizeof(char));
    char *pwd = (char*) malloc(50*sizeof(char));
    char *data_recv = (char*) malloc(1024*sizeof(char));
    char *tmp;
    int i;
    int check;
    strcpy(data_recv, data_analysis(buffer, data_recv));
    printf("%s\n", data_recv);
    tmp = strtok(data_recv, ",");
    printf("%s\n", tmp);
    getData(tmp, username);
    printf("username: %s\n", username);
    pwd = strtok(NULL, ":");
    pwd = strtok(NULL, "\"");
    printf("pwd: %s\n", pwd);
    // PGconn *conn = new_conn_todb();
    PGresult *res = PQexec(conn, "SELECT * FROM nguoidung");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
    }    
    int rows = PQntuples(res);
    printf("rows: %d\n", rows);
    char body[1024];
    for ( i = 0; i < rows; i++)
    {
        printf("id: %s - name: %s - pwd: %s\n", PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2));
        if(strcmp(username, PQgetvalue(res, i, 1)) == 0 && strcmp(pwd, PQgetvalue(res, i, 2)) == 0)
        {
            printf("%s login success\n", username);
            check = 1;
            break;
        }
    }
    if(check == 1)
    {
        sprintf(body, "{\"id_user\":\"%s\"}", PQgetvalue(res, i,0));
    }
    if (check == 0)
    {
        sprintf(body, "{\"id_user\":\"NULL\"}");
    }
    
    char msg[SIZE_MAX];
    sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body), body);
    send(sockfd, msg, strlen(msg), 0);
    PQfinish(conn);
}

void* run_engine(char* cmd)
{
    if (!system(cmd)){
        printf("not successful!\n");
    }
}

void send_list_video(int sockfd){
    // PQreset(conn);
    // printf("core dump ne!!\n");
    PGconn *conn = new_conn_todb();

    int max_size = 6000, current_size = 1;
    int i, j;
    int rows, columns;
    char buff[SIZE_MAX];
    char *body;
    char video_id[6], title[100], display_mod[7], thumbnail[500], view_number[30], upload_date[20], user_id[6], discription[1000], path_to_video[200];
    // PGconn *conn = new_conn_todb();
    char query[255] = "SELECT * FROM video";
    char *addQuery = &query[0];
    char *msg;
    msg = (char*) calloc(sizeof(char), max_size + 1000);
    char video_detail[5000];
    body = (char *) calloc(sizeof(char), max_size);
    PGresult *res = PQexec(conn, addQuery);
    if(PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        printf("No data retrieved\n");  
        PQclear(res); 
    }
    else
    {
        body[0] = '[';
        rows = PQntuples(res);
        for(i = 0; i < rows; i++)
        {   
            strcpy(video_id, PQgetvalue(res, i, 0));
            strcpy(title, PQgetvalue(res, i, 1));
            strcpy(display_mod, PQgetvalue(res, i, 2));
            strcpy(thumbnail, PQgetvalue(res, i, 3));
            strcpy(view_number, PQgetvalue(res, i, 4));
            strcpy(upload_date, PQgetvalue(res, i, 5));
            strcpy(discription, PQgetvalue(res, i, 6));
            strcpy(path_to_video, PQgetvalue(res, i, 7));
            strcpy(user_id, PQgetvalue(res, i, 8));
            sprintf(video_detail, "{\"video_id\":\"%s\", \"title\":\"%s\", \"display_mod\":\"%s\",\"thumbnail\":\"%s\",\"view_number\":\"%s\",\"upload_date\":\"%s\",\"user_id\":\"%s\",\"discription\":\"%s\",\"path_to_video\":\"%s\"}", video_id, title, display_mod, thumbnail, view_number, upload_date, user_id, discription, path_to_video);
            current_size = current_size + sizeof(video_detail);
            if(current_size >= max_size)
            {
                body = (char*) realloc(body, max_size + 6000);
                max_size = max_size + 6000;
            }
            strcat(body, video_detail);
            strcat(body, ",");
        }
        body[strlen(body) - 1] = ']';

        sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n{\"status\":\"ok\",\"data\":%s}", strlen(body) + 23, body);
        send(sockfd, msg, strlen(msg), 0);
    }
    PQfinish(conn);
}

void stream_video(char *buffer, int sockfd){
    //PQreset(conn);
    PGconn *conn = new_conn_todb();

    char *video_id = (char*) malloc(1024*sizeof(char));
    char *data_recv = (char*) malloc(1024*sizeof(char));
    char *tmp;
    char *tmp2;
    char cmd[100];
	port_stream++;
	char path[100];
    char video_name[SIZE_MAX];
    char stream_seraddr[100];
    char msg[1024];

    strcpy(data_recv, data_analysis(buffer, data_recv));
    getData(data_recv, video_id);
    printf("%s\n", video_id);

    // PGconn *conn = new_conn_todb();
    char query[255] = "SELECT path_to_video FROM video WHERE video_id = ";
    strcat(query, "\'");
    strcat(query, video_id);
    strcat(query, "\'");
    char *addQuery = &query[0];

    PGresult *res = PQexec(conn, addQuery);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");  
        PQclear(res);      
    }
    else
    {
        strcpy(video_name, PQgetvalue(res, 0, 0));
        sprintf(path, "./vod_nodejs/serverside/video/%s", video_name);
        sprintf(cmd, "node ./vod_nodejs/serverside/server.js %d %s", port_stream, path);
        sprintf(stream_seraddr, "{\"path\":\"http://localhost:%d\"}", port_stream);
        sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(stream_seraddr), stream_seraddr);
        pthread_t new_pthread;
        if(pthread_create(&new_pthread, NULL, &run_engine, cmd) >= 0)
        {
            send(sockfd, msg, strlen(msg), 0);
        }
    }
    PQfinish(conn);
}
void phan_quyen_video(char *buffer, int sockfd){
    //PQreset(conn);
    PGconn *conn = new_conn_todb();

	//connect db and change video publicity
    char *video_id = (char*) malloc(1024*sizeof(char));
    char *data_recv = (char*) malloc(1024*sizeof(char));
    char *tmp, *string_code;
    int code;
    //phan tich mess ra video id

    strcpy(data_recv, data_analysis(buffer, data_recv));
    tmp = strtok(data_recv, ",");
    getData(tmp, video_id);
    printf("%s", video_id);
    string_code = strtok(NULL, ":");
    string_code = strtok(NULL, "\"");
    code = atoi(string_code);
    printf("%d\n", code);
    // PGconn *conn = new_conn_todb();
    char query[255] = "UPDATE video SET display_mod = true WHERE video_id = "; 
    if(code == 0)
    {
        strcpy(query, "UPDATE video SET display_mod = false WHERE video_id = ");
    }
    strcat(query, "\'");
    strcat(query, video_id);
    strcat(query, "\';");
    char *addQuery = &query[0];

    PQexec(conn, addQuery);
    
    char body[1024] = "{\"status\":\"ok\"}";
    char msg[SIZE_MAX];
    sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body), body);
    send(sockfd, msg, strlen(msg), 0);
    PQfinish(conn);
}

void recv_video(char *buffer, int sockfd){
    // PQreset(conn);
    PGconn *conn = new_conn_todb();

	//su ly tach du lieu
    char *title = (char *) malloc(1024*sizeof(char));
    char *display_mod = (char *) malloc(10*sizeof(char));
    char *thumbnail = (char *) malloc(1024*sizeof(char));
    char *view_number = (char *) malloc(15*sizeof(char));
    char *upload_date = (char *) malloc(15*sizeof(char));
    char *discription = (char *) malloc(1000*sizeof(char));
    char *path_to_video = (char *) malloc(1024*sizeof(char));
    char *data_recv = (char *) malloc(5000*sizeof(char));
    char *user_id = (char *) malloc(6*sizeof(char));
    char *tmp;

    strcpy(data_recv, data_analysis(buffer, data_recv));
    tmp = strtok(data_recv, ",");
    getData(tmp, title);
    tmp = strtok(NULL, ",");
    getData(tmp, display_mod);
    tmp = strtok(NULL, ",");
    getData(tmp, thumbnail);
    tmp = strtok(NULL, ",");
    getData(tmp, view_number);
    tmp = strtok(NULL, ",");
    getData(tmp, upload_date);
    tmp = strtok(NULL, ",");
    getData(tmp, discription);
    tmp = strtok(NULL, ",");
    getData(tmp, path_to_video);

    user_id = strtok(NULL, ":");
    user_id = strtok(NULL, "\"");
    printf("%s\n", user_id);
    char query[5000];
    sprintf(query, "INSERT INTO video (title, display_mod, thumbnail, view_number, upload_date, discription, path_to_video, user_id) VALUES (\'%s\', \'%s\' , \'%s\' , %s, \'%s\', \'%s\', \'%s\', %s);", title, display_mod, thumbnail, view_number, upload_date, discription, path_to_video, user_id);
    char *addQuery = &query[0];
    PQexec(conn, addQuery);
    char body[1024] = "{\"status\":\"ok\"}";
    char msg[SIZE_MAX];
    sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body), body);
    send(sockfd, msg, strlen(msg), 0);
    PQfinish(conn);
}

void delete_video(char *buffer, int sockfd)
{
	PGconn *conn = new_conn_todb();

 char *video_id = (char*) malloc(1024*sizeof(char));
 char *data_recv = (char*) malloc(1024*sizeof(char));
 strcpy(data_recv, data_analysis(buffer, data_recv));
 getData(data_recv, video_id);
 //printf("video id: %s", video_id);
 char query[200];
 
 sprintf(query, "SELECT path_to_video FROM video WHERE video_id = %s", video_id);
 char *addQuery1 = &query[0];
 char cmd[100];
 
 PGresult *res = PQexec(conn, addQuery1);
 if (PQresultStatus(res) != PGRES_TUPLES_OK) {
 printf("No data retrieved\n"); 
 PQclear(res);
 } 
 else
 {
 //printf("video name: %s", PQgetvalue(res, 0, 0));
 sprintf(cmd, "rm ./vod_nodejs/serverside/video/%s", PQgetvalue(res, 0, 0));
 run_engine(cmd);
 char query2[200];
 sprintf(query2, "DELETE FROM video WHERE video_id = %s", video_id);
 char *addQuery = &query2[0];
 PQexec(conn, addQuery);
 char body[1024] = "{\"status\":\"delete ok\"}";
 char msg[SIZE_MAX];
 sprintf(msg, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body), body);
 send(sockfd, msg, strlen(msg), 0);
 }
    PQfinish(conn);
}

int req_analysis(char* buffer){
    char *recv_mes;
    recv_mes = (char*) malloc(SIZE_MAX*sizeof(char));
    strcpy(recv_mes, buffer);
    char *method;
    char *tmp;
    char *type_code;
    int typeCode;
    method = strtok(recv_mes, " ");
    tmp = strtok(NULL, "{");
    tmp = strtok(NULL, ":");
    type_code = strtok(NULL, "\"");
    typeCode = atoi(type_code);
    return typeCode;
}

void* conn_handler(void *sockfd){
    // conn = new_conn_todb();

    count_pthread++;
    int id = count_pthread;
    int type_code;
    printf("start: %d\n", id);
	char recv_mes[1024];
	char* header;
	char* body;
    int sock = *(int*) sockfd;
	int msg_type;
    char *msg;
    char cors_res[SIZE_MAX] = {"HTTP/1.1 200 No Content\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nVary: Accept-Encoding, Origin\nKeep-Alive: timeout=2, max=100\nConnection: Keep-Alive"};
    char buffer[5000];

    char body_get_req[1024] = "{\"status\":\"do not accept get req\"}";
    char msg_get_req[SIZE_MAX];

    char retval;

    read(sock, buffer, 5000);
    printf("\n%s\n", buffer);
    //check if preflight or POST, GET req
    if (buffer[0] == 'O'){
        send(sock, cors_res, strlen(cors_res), 0);  //send back prefligt res
        //bzero(buffer, SIZE_MAX);
        printf("\nDONE SEND\n");
    }
    else if (buffer[0] != 'G')
    {   
        type_code = req_analysis(buffer);
        printf("%d\n", type_code);
        switch (type_code)
        {
        case 1:
            send_list_video(sock);
            bzero(buffer, SIZE_MAX);
            close(sock);
            printf("\nDONE SEND\n");
            printf("end: %d\n", id);
            pthread_exit(retval);
            break;
        case 2:
            phan_quyen_video(buffer, sock);
            bzero(buffer, SIZE_MAX);
            close(sock);
            printf("\nDONE SEND\n");
            printf("end: %d\n", id);
            pthread_exit(retval);
            break;
        case 3:
            recv_video(buffer, sock);
            break;
        case 4:
            stream_video(buffer, sock);
            break;
        case 5:
            login(buffer, sock);
            bzero(buffer, SIZE_MAX);
            close(sock);
            printf("\nDONE SEND\n");
            printf("end: %d\n", id);    
            pthread_exit(retval);
            break;
        case 6:
            sign_in(buffer, sock);
            bzero(buffer, SIZE_MAX);
            close(sock);
            printf("\nDONE SEND\n");
            printf("end: %d\n", id);
            pthread_exit(retval);
        case 7:
            delete_video(buffer, sock);
            bzero(buffer, SIZE_MAX);
            close(sock);
            printf("\nDONE SEND\n");
            printf("end: %d\n", id);
            pthread_exit(retval);
        default:
            break;
        }
        //bzero(buffer, SIZE_MAX);
        printf("\nDONE SEND\n");
    }else{
        printf("this request has no body\n");
        sprintf(msg_get_req, "HTTP/1.1 200 OK\nAccess-Control-Allow-Origin: *\nAccess-Control-Allow-Methods: POST, GET, OPTIONS\nAccess-Control-Allow-Headers: X-PINGOTHER, Content-Type\nAccess-Control-Max-Age: 86400\nContent-Length: %d\nContent-Type: application/json\nConnection: Closed\n\n%s", strlen(body_get_req), body_get_req);
        send(sock, msg_get_req, strlen(msg_get_req), 0);
    }
    bzero(buffer, SIZE_MAX);
    close(sock);
    printf("end: %d\n", id);
}


