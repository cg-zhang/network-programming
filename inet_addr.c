#include <stdio.h>
#include <arpa/inet.h>

int main()
{
	char *addr1 = "1.2.3.4";
	char *addr2 = "1.2.3.256";

	unsigned long conv_addr = inet_addr(addr1);
	if (conv_addr != INADDR_NONE)
		printf("Network ordered integer addr: %#lx \n", conv_addr);
	else
		printf("Error occured! \n");

	conv_addr = inet_addr(addr2);
	if (conv_addr != INADDR_NONE)
		printf("Network ordered integer addr: %#lx \n", conv_addr);
	else
		printf("Error occured! \n");
	return 0;
}
