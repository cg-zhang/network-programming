#include <stdio.h>
#include <unistd.h>

#define buf_size 30

int main()
{
	int fds1[2], fds2[2];
	char str1[] = "Who are you?";
	char str2[] = "Thank you";
	char buf[buf_size];

	pipe(fds1);  // 子进程 ->  父进程
	pipe(fds2);  // 父进程 ->  子进程

	pid_t pid = fork();

	if (pid == 0)
	{
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, buf_size);
		printf("Message from parent: %s\n", buf);
	}
	else
	{
		read(fds1[0], buf, buf_size);
		printf("Message from child: %s\n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}
	return 0;
}
