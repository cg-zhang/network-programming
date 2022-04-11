#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;  // 服务端、客户端套接字描述符
	struct sockaddr_in serv_adr, clnt_adr;  // 服务端、客户端IP地址等信息

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;  // 客户端结构体长度
	int str_len;   // 接收字符长度
	char buf[BUF_SIZE];  // 接收缓冲区
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	// 初始化sigaction结构体信息
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	// 注册信号
	sigaction(SIGCHLD, &act, 0);
	// 创建套接字
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	// 初始化服务端结构体信息
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 绑定服务端IP和端口号
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bing() error!");

	// 服务端开始监听
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error!");

	// 循环处理连接的客户端
	while (1)
	{
		adr_sz = sizeof(clnt_adr);
		// 处理客户端请求
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
		if (clnt_sock == -1)
			continue;
		else
			puts("New client connected...");

		// 为该客户端创建进程
		pid = fork();
		// 进程创建失败
		if (pid == -1)
		{
			close(clnt_sock);
			continue;
		}
		if (pid == 0)
		{
			// 子进程运行区域
			close(serv_sock); // 关闭父进程套接字
			// 接收客户端发送来的消息
			while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);

			// 关闭当前连接的套接字
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

// 信号处理函数，当子进程（客户端断开连接）时运行
void read_childproc(int sig)
{
	pid_t pid;
	int status;
	// 获取子进程id、返回值信息
	pid = waitpid(-1, &status, WNOHANG);
	printf("removed proc: %d \n", pid);
}
