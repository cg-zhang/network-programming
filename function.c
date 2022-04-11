// 打开文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcnt1.h>

int open(const char *path, int flag);
    // 成功时返回文件描述符，失败返回-1

/*
	path: 文件名的字符串地址
	flag: 文件打开模式信息
      	  O_CREAT          必要时创建文件
      	  O_TRUNC          删除全部现有数据
      	  O_APPEND         维持现有数据，在后面追加
      	  O_RDONLY         只读打开
      	  O_WRONLY         只写打开
      	  O_WDWR           读写打开
*/


// 关闭文件
#include <unistd.h>

int close(int fd);
    // 成功时返回0，失败返回-1，不仅可以关闭文件，还可以关闭套接字


// 将数据写入文件
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t nbytes);
        // 成功时返回写入的字节数(signed int)，失败返回-1

/*
	fd: 显示数据传输对象的文件描述符
	buf: 保存要传输数据的缓冲地址值
	nbytes: 要传输数据的字节数(unsigned int)
*/


// 读取文件中的数据
#include <unistd.h>

ssize_t read(int fd, void *buf, size_t nbytes);
        // 成功时返回接收的字节数(遇到文件结尾返回0)，失败返回-1

/*
	fd: 显示数据接收对象的文件描述符
	buf: 要保存接收数据的缓冲地址值
	nbytes: 要接收数据的最大字节数
*/


// 创建套接字
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
    // 成功时返回文件描述符，失败返回-1

/*
	domain: 套接字中使用的协议族信息
	        PF_INET        IPv4互联网协议族(常用)
	        PF_INET6       IPv6互联网协议族
	        PF_LOCAL       本地通信的UNIX协议族
	        PF_PACKET      底层套接字的协议族
	        PF_IPX         IPX Novell协议族
	  type: 套接字数据传输类型信息
	        SOCK_STREAM    面向连接的套接字
	        SOCK_DGRAM     面向消息的套接字
	protocol: 计算机间通信中使用的协议信息
	        (大部分情况为0，除非出现 同一协议族中存在多个数据传输方式相同的协议)


/* 数据类型
    类型名称       数据类型说明                                头文件
    int8_t        signed 8-bit int
    uint8_t       unsigned 8-bit int(unsigned char)
    int16_t       signed 16-bit int
    uint16_t      unsigned 16-bit int(unsigned short)         sys/types.h
    int32_t       signed 32-bit int
    uint32_t      unsigned 32-bit int(unsigned long)

    sa_family_t   地址族
    socklen_t     长度(length of struct)                      sys/socket.h

    in_addr_t     IP地址，声明为uint32_t
    in_port_t     端口号，声明为uint16_t                       netinet/in.h
*/


// 由于CPU中存储方式不同(大端/小端)，因此传递前需要进行字节序转换
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
// h表示主机(host)字节序，n表示网络(network)字节序
// 以s结尾用于端口号转换，以l结尾用于IP地址转换


// 字符串IP地址 -> 32位整数型
#include <arpa/inet.h>

in_addr_t inet_addr(const char *string);
          // 成功时返回32位大端序整数型值，失败返回INADDR_NONE

int inet_aton(const char *string, struct in_addr *addr);
    // 成功时返回1，失败返回0，可以直接将转换的地址赋给结构体中的地址

/*
	string: 含有需转换的IP地址信息的字符串地址值
	addr: 将保存转换结果的in_addr结构体变量的 地址值
*/


// 网络字节序整数型 -> 字符串IP地址
#include <arpa/inet.h>

char *inet_ntoa(struct in_addr adr);
     // 成功时返回转换的字符串地址值，失败返回-1
     // 调用完应立即将返回信息复制到其他内存空间，再次调用会覆盖上一次调用返回的值


/*
	INADDR_ANY    自动获取运行服务器端的计算机IP地址
*/


// 向套接字分配网络地址
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
    // 成功时返回0，失败返回-1

/*
	sockfd: 要分配地址信息的套接字文件描述符(socket返回值)
	myaddr: 存有地址信息的结构体变量地址值(通过对sockaddr_in结构体类型值强转换)
	addrlen: 第二个结构体变量的长度
*/


// 进入等待连接请求状态
#include <sys/socket/h>

int listen(int sock, int backlog);
    // 成功时返回0， 失败返回-1

/*
	sock: 希望进入等待连接请求状态的的套接字文件描述符，传递的描述符套接字参数成为服务器端套接字
	backlog: 连接请求等待队列长度，表示最多接收n个连接请求进入队列
*/


// 受理客户端连接请求
#include <sys/socket.h>

int accept(int sock, strcut sockaddr *addr, socklen_t *addrlen);
    // 成功时返回创建的套接字文件描述符，失败返回-1

/*
	sock: 服务器套接字的文件描述符
	addr: 保存发起连接请求的客户端地址信息的变量地址值，调用函数后向传递来的地址变量参数填充客户端地址信息
	addrlen: 第二个参数addr结构体的长度，但是存有长度的变量地址。函数调用后，该变量即被填入客户端地址长度
*/


// 客户端发起连接
#include <sys/socket.h>

int connect(int sock, struct sockaddr *servaddr, socklen_t addrlen);
    // 成功时返回0，失败返回-1，此阶段给客户端套接字分配IP地址和端口号(随机)

/*
	sock: 客户端套接字文件描述符
	servaddr: 保存目标服务器端地址信息的变量地址值
	addrlen: 以字节为单位传递已传递给第二个结构体参数servaddr的地址变量长度
*/


// 基于UDP的数据I/O函数
#include <sys/socket.h>

// 发送消息，在此阶段给套接字分配IP(主机)和端口号
ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
        // 成功时返回传输的字节数，失败返回-1

/*
	sock: 用于传输数据的UDP套接字文件描述符
	buff: 保存待传输数据的缓冲地址值
	nbytes: 待传输的数据长度，以字节为单位
	flags: 可选项参数，若没有则传递0
	to: 存有目标地址信息的sockaddr结构体变量的地址值
	addrlen: 传递给参数to的地址值结构体变量长度
*/


// 接收发送端消息
#include <sys/socket.h>

ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
        // 成功时返回接收的字节数，失败返回-1

/*
	sock: 用于传输数据的UDP套接字文件描述符
	buff: 保存接收数据的缓冲地址值
	nbytes: 可接受的最大字节数
	flags: 可选项参数，若没有则传递0
	from: 存有发送端地址信息的sockaddr结构体变量的地址值
	addrlen: 保存参数为from的结构体变量长度的变量地址值
*/


// 优雅断开函数
#include <sys/socket.h>

int shutdown(int sock, int howto);
    // 成功时返回0，失败返回-1

/*
	sock: 需要断开地套接字文件描述符
	howto: 传递断开方式信息
	       SHUT_RD      断开输入流，无法接收数据
	       SHUT_WR      断开输出流，无法写入数据
	       SHUT_RDWR    同时断开I/O流
*/


// 利用域名获取IP地址
#include <netdb.h>

struct hostent *gethostbyname(const char *hostname);
               // 成功时返回hostent结构体地址，失败时返回NULL指针


// 利用IP地址获取域名信息
#include <netdb.h>

struct hostent *gethostbyaddr(const char *addr, socklen_t len, int family);
               // 成功时返回hostent结构体变量地址值，失败时返回NULL指针

/*
	addr: 含有IP地址信息的in_addr结构体指针。为了同时传递IPv4地址之外的其他信息，该变量的类型为char指针
	len: 向第一个参数传递的地址信息的字节数，IPv4时为4，IPv时为6
	family: 传递地址族信息
*/


// 读取套接字可选项
#include <sys/socket.h>

int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
    // 成功时返回0，失败返回-1

/*
	sock: 用于查看选项套接字文件描述符
	level: 要查看的可选项的协议层
	optname: 要查看的可选项名
	optval: 保存查看结果的缓冲地址值
	optlen: 向第四个参数optval传递的缓冲大小。调用函数后，该变量中保存通过第四个参数返回的可选项信息的字节数
*/


// 设置套接字可选项
#include <sys/socket.h>

int setsockopt(int sock, int level, int optname, const void *optval, socklen_t *optlen);
    // 成功时返回0，失败返回-1

/*
	sock: 用于更改选项套接字文件描述符
	level: 要更改的可选项的协议层
	optname: 要更改的可选项名
	optval: 保存要更改的选项信息的缓冲地址值
	optlen: 向第四个参数optval传递的可选项信息的字节数
*/ 


// 创建多进程
#include <unistd.h>

pid_t fork(void);
      // 成功时返回进程ID，失败时返回-1


// 销毁僵尸进程1：利用wait函数
#include <sys/wait.h>

pid_t wait(int * statloc);
      // 成功时返回终止的子进程ID，失败时返回-1
	  // (如果没有已终止的子进程，那么程序将阻塞，直到有子进程终止)
/* 
	调用此函数如果已有子进程终止，那么子进程终止时传递的返回值(exit函数的参数值、main函数的返回值)将保存到该函数的参数所指内存空间，参数还含有其他信息，通过下面两个函数来分离：
	WIFEXITED子进程正常终止时返回"真"(true)
	WEXITSTATUS返回子进程的返回值
*/


// 销毁僵尸进程2：使用waitpid函数
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int * statloc, int options);
      // 成功时返回终止的子进程ID(没有子进程终止则返回0)，失败返回-1

/*
	pid: 等待终止的目标子进程的ID，若传递-1，则与wait函数相同，可以等待任意子进程终止
	statloc: 与wait函数的参数意义相同
	options: 传递头文件sys/wait.h声明的常量WNOHANG，即使没有终止的子进程也不会进入阻塞状态，而是返回0并退出函数
*/


// 信号注册函数
#include <signal.h>

void (*signal(int signo, void (*func)(int)))(int);
     // 为了在产生信号时调用，返回之前注册的函数指针(发生信号时将唤醒由于调用sleep函数而进入阻塞状态的进程)

/*
	调用此函数时，第一个参数为特殊情况信息，第二个参数为第一个参数代表的情况发生时，所调用的函数(返回值为void，参数为int)
	特殊情况有:
	SIGALRM: 已到通过alarm函数注册的时间
	SIGINE: 输入CTRL + C
	SIGCHLD: 子进程终止
*/


 // alarm函数
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
         // 返回0或以秒为单位的距SIGALRM信号发生所剩时间

/*
	如果调用该函数的同时传递一个 参数，相应时间后将产生SIGALRM信号。如果传递0，则之前对信号的预约取消
	如果通过该函数预约信号后未指定该信号对应的处理函数(signal函数)，则终止进程
*/


// sigaction函数(signal在unix不同os中可能存在差别，但sigaction不会，因此更稳定)
#include <signal.h>

int sigaction(int signo, const struct sigaction *act, struct sigaction *oldact);
    // 成功时返回0，失败返回-1

/*
	signo: 与signal函数相同，传递信号信息
	act: 对应于第一个参数的信号处理函数
	oldact: 通过此参数获取之前注册的信号处理函数指针，若不需要传递0
*/

