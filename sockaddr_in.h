#include <stdio.h>
#include <arpa/inet.h>
#include "in_addr.h"

struct sockaddr_in
{
	sa_family_t        sin_family;        // 地址族
	uint16_t           sin_port;          // 16位TCP/UDP端口号
	struct in_addr     sin_addr;          // 32位ip地址
	char               sin_zero[8];       // 不使用
};
