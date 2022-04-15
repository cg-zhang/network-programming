#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50

void error_handling(char *buf);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events;   // 用于接收返回的状态发生变化的文件描述符
	struct epoll_event event;    // 注册监视对象时指定的event事件
	int epfd, event_cnt;    // epoll例程描述符和发生事件的文件描述符数

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
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

	// 创建epoll例程
	epfd = epoll_create(EPOLL_SIZE);
	// 动态分配wait函数所用事件结构体空间
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	// 初始化需要关注的事件结构体
	event.events = EPOLLIN;  // 关注读取数据事件
	event.data.fd = serv_sock;  // 事件发生的套接字
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);  // 注册文件描述符 

	while (1)
	{
		// 调度epoll，返回发生的事件描述符
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if (event_cnt == -1)
		{
			puts("epoll wait() error");
			break;
		}
		// 遍历每个事件所在的文件描述符
		for (int i = 0; i < event_cnt; ++i)
		{
			if (ep_events[i].data.fd == serv_sock)
			{
				// 如果是服务端套接字发生变化，说明要进行处理连接请求
				adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				// 初始化并注册当前客户端的epoll例程
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client: %d \n", clnt_sock);
			}
			else
			{
				// 客户端文件描述符发生变化，意味着从客户端读取数据，或者断开连接
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
				if (str_len == 0)
				{
					//  当前客户端结束输入，断开连接，删除epoll例程
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client : %d \n", ep_events[i].data.fd);
				}
				else 
				{
					// 客户端发送过来数据，需要做出回声
					write(ep_events[i].data.fd, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
