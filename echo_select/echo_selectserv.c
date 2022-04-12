#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100

void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	struct timeval timeout;

	socklen_t adr_sz;
	int fd_max, str_len, fd_num;
	char buf[BUF_SIZE];
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	// 初始化关注读取数据的文件描述符
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(serv_sock,&reads);
	// printf("serv_sock: %d \n", serv_sock);
	fd_max = serv_sock; // 记录最大的那个，方便后面传入select第一个参数

	while (1)
	{
		// 每次调用select后，会改变状态，拷贝一份可保存初始状态
		fd_set cpy_reads = reads;
		// 设置超时时间，调用select后时间会动态变化，因此放循环内
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;

		// 遍历每个描述符
		for (int i = 0; i < fd_max + 1; ++i)
		{
			// 如果cpy_reads数组中存在此描述符，且状态发生变化
			if (FD_ISSET(i, &cpy_reads))
			{
				// 如果服务端套接字发生变化，则需要受理连接请求
			        if (i == serv_sock)
				{
					adr_sz = sizeof(clnt_adr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
					// 注册与客户端连接的套接字描述符，注意是初始状态
					FD_SET(clnt_sock, &reads);
					// 更新最大描述符
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
					printf("connnected client: %d\n", clnt_sock);
				}
				else
				{
					// 非服务端套接字，说明有待读取的客户端发送来的数据，此时判断是结束输入还是常规字符串
					str_len = read(i, buf, BUF_SIZE);
					if (str_len == 0)
					{
						// 客户端输入结束，关闭了套接字，传送过来了EOF
						FD_CLR(i, &reads);
						close(i);
						printf("closed client: %d\n", i);
					}
					else
					{
						// 客户端还在输入，因此根据缓冲区做出回声
						write(i, buf, str_len);
					}
				}
			}
		}
	}
	close(serv_sock);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}

