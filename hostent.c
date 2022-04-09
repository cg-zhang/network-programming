struct hostent
{
	char *   h_name;     // 官方域名
	char **  h_aliases;  // 存在由一个IP绑定的多个域名，为一个数组
	int      h_addrtype; // 返回IP地址的地址族信息
	int      h_length;   // 地址长度，IPv4为4字节，IPv6为16字节
	char **  h_addr_list; // 返回该域名对应的IP地址，以整数形式存储，可能有多个，为一个字符串指针数组，其元素指向的是in_addr结构体变量地址值而非字符串
}