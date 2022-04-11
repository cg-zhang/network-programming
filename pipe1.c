#include <stdio.h>
#include <unistd.h>

#define buf_size 30

int main()
{
	int fds[2];
	char str[] = "Who are you?";
	char buf[buf_size];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if (pid == 0)
	{
		write(fds[1], str, sizeof(str));
	}
	else
	{
		read(fds[0], buf, buf_size);
		puts(buf);
	}
	return 0;
}
