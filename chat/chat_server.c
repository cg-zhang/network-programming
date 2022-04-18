#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, int len);
void error_handling(char * msg);

int clnt_cnt = 0;  // 客户端计数
int clnt_socks[MAX_CLNT];   // 存储连接服务端的客户端套接字信息
pthread_mutex_t mutx;   // 互斥量

int main(int argc, char * argv[])
{
	int serv_sock, clnt_sock;   // 服务端、客户端套接字文件描述符
	struct sockaddr_in serv_adr, clnt_adr;   // 服务端、客户端地址信息结构体
	int clnt_adr_sz;   // 客户端结构体长度
	pthread_t t_id;   // 线程ID

	// 运行时指定端口号
	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 初始化互斥量
	pthread_mutex_init(&mutx, NULL);
	// 创建服务端套接字
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	// 设置可选项，跳过time_wait状态
	int option = 1;
	int opt_len = sizeof(option);
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, opt_len);

	// 初始化服务端地址族信息
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	// 绑定IP地址和端口号
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	// 开始监听
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// 循环处理请求连接的客户端
	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		// 接受请求
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		// 利用互斥量锁临界区
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		// 释放临界区
		pthread_mutex_unlock(&mutx);

		// 为当前客户端创建线程，处理函数为handle_clnt，传递描述符作为参数
                pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		// 销毁线程
		pthread_detach(t_id);
		
		printf("Connected client IP: %s, client : %d\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);
	}
	// 关闭服务端套接字
	close(serv_sock);
	return 0;
}

void * handle_clnt(void *arg)
{
	int clnt_sock = *((int*)arg);
	int str_len = 0;
	char msg[BUF_SIZE];

	// 读取客户端发送的数据，并向所有连接此服务端的客户端发送消息
	while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
	{
		printf("已接收到来自client：%d的消息\n", clnt_sock);
		send_msg(msg, str_len);
	}
	// 此客户端连接断开，建立临界区修改clnt_socks数组
	pthread_mutex_lock(&mutx);   // clnt_socks数组是临界资源
	// 移除此描述符，将后面的覆盖到前面
	for (int i = 0; i < clnt_cnt; ++i)
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (++i < clnt_cnt) 
				clnt_socks[i] = clnt_socks[i - 1];
			break;
		}
	}
	printf("client : %d quit!\n", clnt_sock);
	// 客户端数减1
	--clnt_cnt;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}

void send_msg(char * msg, int len)
{
	printf("已发送广播消息!\n");
	// clnt_socks数组是临界资源	
	pthread_mutex_lock(&mutx);
	for (int i = 0; i < clnt_cnt; ++i)
		write(clnt_socks[i], msg, len);
	pthread_mutex_unlock(&mutx);
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
