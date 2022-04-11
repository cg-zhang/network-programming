#include <stdio.h>
#include <unistd.h>

#define buf_size 30

int main()
{
	int fds[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you";
	char buf[buf_size];
	pid_t pid;

	pipe(fds);  // 创建管道
	pid = fork();  // 子进程也有两个管道描述符

	if (pid == 0)
	{
		// 子进程给父进程写
		write(fds[1], str1, sizeof(str1));
		// 进入管道的数据，谁先读是谁的，为了保证写完父进程能读取，须让子进程休眠一段时间
	        sleep(2);
		// 读取父进程的消息
		read(fds[0], buf, buf_size);
		printf("Message from parent: %s\n", buf);
	}
	else 
	{
		// 读取子进程发送的消息
		read(fds[0], buf, buf_size);
		printf("Message from child: %s\n", buf);
		// 向子进程发
		write(fds[1], str2, sizeof(str2));
		// 发送完消息后，等待子进程接受完结束进程，保证父进程后于子进程结束
		sleep(3);
	}
	return 0;

}
