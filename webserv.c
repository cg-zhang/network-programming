#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void * arg);
void send_data(FILE * fp, char* ct, char* file_name);
char* content_type(char* file); 
void send_error(FILE* fp);
void error_handling(char* messages);

int main(int argc, char* argv[])
{
        int serv_sock, clnt_sock;
        struct sockaddr_in serv_adr, clnt_adr;
        socklen_t clnt_adr_size; 
        char buf[BUF_SIZE];
        pthread_t t_id;
        if (argc != 2)
        {
                printf("Usage: %s <port>\n", argv[0]);
                exit(1);
        }       

        serv_sock = socket(PF_INET, SOCK_STREAM, 0);
        memset(&serv_adr, 0, sizeof(serv_adr));
        serv_adr.sin_family = AF_INET;
        serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
        serv_adr.sin_port = htons(atoi(argv[1]));

        if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
                error_handling("bind() error");
        if (listen(serv_sock, 20) == -1)
                error_handling("listen() error");

        while (1)
        {
                clnt_adr_size = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_sock, &clnt_adr_size);
                // 输出IP地址和端口号
                printf("Connection Request: %s:%d\n", inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));
                // 为该连接创建线程
                pthread_create(&t_id, NULL, request_handler, &clnt_sock);
                pthread_detach(t_id);
        }
        close(serv_sock);
        return 0;
}

// 对请求的内容字符串进行处理
void* request_handler(void* arg)
{
        int clnt_sock = *((int*)arg);
        char req_line[SMALL_BUF];   // 请求行
        FILE* clnt_read;
        FILE* clnt_write;

        char method[10];   // 请求方式
        char ct[15];   // 数据传输类型
        char file_name[30];   // 请求文件名

        // 套接字转换为FILE指针，后续使用标准IO处理
        clnt_read = fdopen(clnt_sock, "r");
        clnt_write = fdopen(clnt_sock, "w");
        // 读取请求行
        fgets(req_line, SMALL_BUF, clnt_read);
        // 请求行中没有包含HTTP版本
        if (strstr(req_line, "HTTP/") == NULL)
        {
                // 发送请求方式错误
                send_error(clnt_write);
                fclose(clnt_read);
                fclose(clnt_write);
                return 0;
        }
        // 提取请求方式
        strcpy(method, strtok(req_line, " /"));
        // 提取请求网页的地址
        strcpy(file_name, strtok(NULL, " /"));
        // 获取返回数据类型
        strcpy(ct, content_type(file_name));
        // 仅支持GET
        if (strcmp(method, "GET") != 0)
        {
                // 发送请求方式错误
                send_error(clnt_write);
                fclose(clnt_read);
                fclose(clnt_write);
                return 0;
        }
        // 关闭读，发送消息后再关闭写
        fclose(clnt_read);
        // 发送响应消息和数据
        send_data(clnt_write, ct, file_name);
}

// 返回数据传输类型
char* content_type(char* file)
{
        char extension[SMALL_BUF];
        char file_name[SMALL_BUF];
        strcpy(file_name, file);
        // 提取文件名
        strcpy(file_name, ".");
        // 提取后缀，html
        strcpy(extension, strtok(NULL, "."));

        if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
                return "text/html";   // 返回网页为html数据
        else 
                return "text/plain";  // 返回纯文本数据
}

// 错误响应
void send_error(FILE* fp)
{
        char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
        char server[] = "Server:Linux Web Server \r\n";
        char cnt_len[] = "Content-length:2048\r\n";
        char cnt_type[] = "Content-type:text/html\r\n\r\n";  // 消息头结束，与消息体之间有一个空行
        char content[] = "<html><head><title>NETWORK</title></head>"
                       "<body><font size=+5><br>发生错误！请检查请求文件名和请求方式！"
                       "</font></body></html>";

        // 传输错误响应消息
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);
        fflush(fp);
}

// 处理正确的请求
void send_data(FILE* fp, char* ct, char* file_name)
{
        char protocol[] = "HTTP/1.0 200 OK\r\n";
        char server[] = "Server:Linux Web Server \r\n";
        char cnt_len[] = "Content-length:2048\r\n";
        char cnt_type[SMALL_BUF];
        char buf[BUF_SIZE];
        FILE* send_file;  // 请求的文件

        // 返回的数据类型
        sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
        // 打开本地文件
        send_file = fopen(file_name, "r");
        if (send_file == NULL)
        {
                // 请求文件打开出错
                send_error(fp);
                return;
        }

        // 传输消息头信息
        fputs(protocol, fp);
        fputs(server, fp);
        fputs(cnt_len, fp);
        fputs(cnt_type, fp);

        // 传输消息主体
        while (fgets(buf, BUF_SIZE, send_file) != NULL)
        {
                fputs(buf, fp);
                fflush(fp);
        }
        fflush(fp);
        // 关闭写
        fclose(fp);
}

// 异常处理
void error_handling(char* message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
