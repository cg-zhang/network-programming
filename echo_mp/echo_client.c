#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define buf_size 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[buf_size];
	int str_len;
	struct sockaddr_in serv_adr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("Connect() error!");
	else
		puts("Connected ......");
	
	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, buf_size, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		write(sock, message, strlen(message));
		str_len = read(sock, message, buf_size - 1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

