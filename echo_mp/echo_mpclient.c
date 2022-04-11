#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define buf_size 30

void error_handling(char *message);
void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);


int main(int argc, char *argv[])
{
	int sock;  // socket套接字描述符
	pid_t pid;  // fork函数返回值
	char buf[buf_size];   // 读写缓冲区
	struct sockaddr_in serv_adr;  // 服务端信息结构体

	// 调用时，指定IP地址和端口号
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	// 创建socket套接字
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	// 初始化服务端结构体信息
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	// 向服务端发起连接
	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("Connect() error!");
	else
		puts("Connected ......");
	
	pid = fork();
	// 子进程负责写, 父进程负责读
	if (pid == 0)
		write_routine(sock, buf);
	else
		read_routine(sock, buf);
	close(sock);
	return 0;

}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void write_routine(int sock, char *buf)
{
	while (1)
	{
	        // 获取输入	
		fgets(buf, buf_size, stdin);
		if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))
		{
			// 调用shutdown函数向服务器端传递EOF
			shutdown(sock, SHUT_WR);
			return;
		}
		write(sock, buf, strlen(buf)); // 向服务器写
	}
}

void read_routine(int sock, char *buf)
{
	while (1)
	{
		// 读取回声的数据
		int str_len = read(sock, buf, buf_size);
		if (str_len == 0)
			return;
		
		buf[str_len] = 0;
		// 输出
		printf("Message from server: %s", buf);
	}
}
