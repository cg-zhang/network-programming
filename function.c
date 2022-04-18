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

	第二个参数结构体:
	struct sockaddr_in
	{
		sa_family_t    sin_family;     // 地址族
		uint16_t       sin_port;       // 16位端口号
		struct in_addr sin_addr;       // 32位IP地址
		char           sin_zero[8];    // 不使用，初始化为0
	};
     
    struct in_addr
    {
		in_addr_t      s_addr;         // 32位IP地址，整数类型
    };
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

	struct hostent
	{
		char * h_name;        // 官方域名
		char ** h_aliases;    // 除了官方域名还有其他域名可访问当前主页
		int h_addrtype;       // 协议族类型，包含IPv4和IPv6
		int h_length;         // IP地址长度(字节)
		char ** h_addr_list;  // 可能有多个IP对应一个域名，进行负载均衡
	};
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

	struct sigaction
	{
		void (*sa_handler)(int);   // 信号处理函数，函数指针
		sigset_t sa_mask;    // 信号相关选项和特性，初始化为0   sigemptyset(&act.sa_mask)
		int sa_flags;    // 同上
	};
*/


// 管道通信
#include <unistd.h>

int pipe(int filedes[2]);
    // 成功时返回0，失败时返回-1

/*
	filedes[0]: 通过管道接收数据时使用的文件描述符，即管道出口
	filedes[1]: 通过管道传输数据时使用的文件描述符，即管道入口
    (进入管道中的数据，不分主，先读的进程会把数据取走)
*/


// 设置检查(监视)范围及超时
#include <sys/select.h>
#include <sys/time.h>

int select(int maxfd, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout);
    // 成功时返回大于0的值，失败时返回-1，超时返回0

/*
	maxfd: 监视对象文件描述符数量
	readset: 将所有关注“是否存在待读取数据”的文件描述符注册到fd_set型变量，并传递其地址值
	writeset: 将所有关注“是否可传输无阻塞数据”的文件描述符注册到fd_set型变量，并传递其地址值
	exceptset: 将所有关注“是否发生异常”的文件描述符注册到fd_set型变量，并传递其地址值
	timeout: 调用select函数后，为防止陷入无限阻塞的状态，传递超时信息
	返回值: 发生错误时返回-1，超时返回0。因发生关注的事件返回时，返回大于0的值，代表发生事件的文件描述符

    // 调用select函数后，此结构体中的时间会动态变化，表示超时前剩余时间
	struct timeval 
	{
		long tv_sec;    // 超时时间(秒数)
		long tv_usec;   // 微秒数
	};
*/


// Linux的send函数
#include <sys/socket.h>

ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
        // 成功时返回发送的字节数(signed int)，失败返回-1

/*
	sockfd: 表示与数据传输对象的连接的套接字文件描述符
	buf: 保存待传输数据的缓冲地址值
	nbytes: 待传输数据的字节数(unsigned int)
	flags: 传输数据时指定的可选项信息
*/


// Linux的recv函数
#include <sys/socket.h>

ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags);
        // 成功时返回接收的字节数(收到EOF时返回0)，失败返回-1

/*
	sockfd: 表示与数据接收对象的连接的套接字文件描述符
	buf: 保存接收数据的缓冲地址值
	nbytes: 可接收数据的字节数(unsigned int)
	flags: 接收数据时指定的可选项信息
*/


/* 
	 可选项                    含义                           
	MSG_OOB            用于传输带外数据(紧急消息)                
	MSG_PEEK		   验证输入缓冲中是否存在接收的数据(发送方)     可读取缓冲区数据，但是不会删除缓冲区                 
	MSG_DONTROUTE      数据传输中不参照路由表，在本地网络
	                   中寻找目的地(发送方)                         
    MSG_DONTWAIT       调用I/O函数时不阻塞，用于使用非阻塞I/O(发送方 & 接收方)
    MSG_WAITALL        防止函数返回，直到接收全部请求的字节数(接收方)
*/


// Linux中的writev函数
#include <sys/uio.h>

ssize_t writev(int filedes, const struct iovec *iov, int iovcnt);
        // 成功时返回发送的字节数，失败时返回-1

/*
	filedes: 表示数据传输对象的套接字文件描述符。但该函数不限于套接字，还可以像read一样传递文件或标准输出描述符
	iov: iovec结构体数组的地址值，每个结构体中包含待发送数据的位置(指针)和长度信息
	iovcnt: 向第二个参数传递的数组长度，代表有几个缓冲区

	struct iovec
	{
		void * iov_base;    // 缓冲区地址
		size_t iov_len;     // 缓冲区大小
	};
*/


// Linux中的readv函数
#include <sys/uio.h>

ssize_t readv(int filedes, const struct iovec *iov, int iovcnt);
        // 成功时返回接收的字节数，失败时返回-1

/*
	filedes: 传递接收数据的文件描述符
	iov: 包含数据保存位置和大小信息和iovec结构体数组的地址值
	iovcnt: 向第二个参数中数组的长度
*/


// 多播

// 设置TTL，指定协议层和选项名
setsockopt(send_sock, IPPROTO_IP, UP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live));

// 加入多播组
struct ip_mrep join_adr;
join_adr.imr_multiaddr.s_addr = "多播组地址信息";
join_adr.imr_interfact.s_addr = "加入多播组的主机地址信息";
setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *)& join_adr, sizeof(join_adr));

struct ip_mreq
{
	struct in_addr imr_multiaddr;   // 加入的组IP地址
	struct in_addr imr_interface;   // 加入该组的套接字所属主机的IP地址，也可使用INADDR_ANY
};


// C语言标准I/O

FIFE *fopen(const char *path, const char *mode);
     // 与套接字不同，IO返回的是FILE结构体指针，而套接字返回文件描述符

/*
path: 打开文件路径
mode: 打开文件模式
*/
 
int fclose(FIFE *stream)
    // 关闭当前文件流
 
int fgetc(FIFE *stream)
	// 读取一个字符，返回该字符转换为int类型的值
 
int fputc(int c, FIFE *stream);
	// 把c转换为对应的字符，写到stream里
 
char *fgets(char *s, int size, FIFE *stream);
	  // 读到换行符或者EOF截止，结果保存到s中，会多保存一个'\0'
 
int fputs(const char *s, FIFE *stream);
	// 把s除去'\0'之外的字符写到stream里
 
size_t fread(void *ptr, size_t size, size_t nmemb, FIFE *stream);
	   // 读nmemb个元素，每个大小size字节，保存到ptr里
 
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FIFE *stream);
	   // 写nmemb个元素，每个大小size字节，写到stream里


// 利用fdopen函数将文件描述符转换为FILE结构体指针
FILE * fdopen(int fildes, const char * mode);
       // 成功时返回转换的FILE结构体指针，失败时返回NULL

/*
	fildes: 需要转换的文件描述符
	mode: 将要创建的FILE结构体指针的模式信息
*/


// 反向转换
#include <stdio.h>

int fileno(FILE * stream);
    // 成功时返回转换后的文件描述符，失败时返回-1


// dup & dup2
// FILE读写指针无法实现半关闭，因此需要复制文件描述符
#include <unistd.h>

int dup(int fildes);
int dup2(int fildes, int fildes2);
    // 成功时返回复制的文件描述符，失败时返回-1

/*
	fildes: 需要复制的文件描述符
	fildes2: 明确指定的文件描述符整数值
*/


// 创建保存epoll文件描述符的空间，被称为“epoll例程”
#include <sys/epoll.h>

int epoll_create(int size);
    // 成功时返回epoll文件描述符，失败时返回-1(size用来指定例程大小，实际后续版本忽略此参数，内核会动态调整其大小)


// 向空间注册并注销文件描述符
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event * event);
    // 成功时返回0，失败返回-1

/*
	epfd: 用于注册监视对象的epoll例程的文件描述符
	op: 用于指定监视对象的添加、删除或者更改等操作

	    EPOLL_CTL_ADD: 将文件描述符注册到epoll例程
        EPOLL_CTL_DEL: 从epoll例程中删除文件描述符
        EPOLL_CTL_MOD:更改注册的文件描述符的关注事件发生情况，也即修改event

	fd: 需要注册的监视对象文件描述符
	event: 监视对象的事件类型
*/


struct epoll_event
{
	__uint32_t events;
	epoll_data_t data;
};

/* events:
   EPOLLIN: 需要读取数据的情况
   EPOLLOUT: 输出缓冲为空，可以立即发送数据的情况
   EPOLLPRI: 收到OOB数据的情况(带外数据，也即紧急数据)
   EPOLLRDHUP: 断开连接或半关闭的情况，这在边缘触发方式下非常有用
   EPOLLERR: 发生错误的情况
   EPOLLET: 以边缘触发的方式得到事件通知
   EPOLLONESHOT: 发生一次事件后，相应文件描述符不再收到事件通知。因此需要向epoll_ctl函数的第二个参数传递EPOLL_CTL_MOD，再次设置事件
*/

typedef union epoll_data
{
	void *ptr;
	int fd;
	__uint32_t u32;
	__uint64_t u64;
} epoll_data_t;


// 等待描述符发生变化，调用后，第二个参数保存了发生事件的文件描述符集合，不需遍历
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
    // 成功时返回发生事件的文件描述符数，失败时返回-1

/*
	epfd: 表示事件发生监视范围的epoll例程的文件描述符
	events: 保存发生事件的文件描述符集合的结构体地址值(动态分配: malloc(sizeof(struct epoll_event)*EPOLL_SIZE))
	maxevents: 第二个参数中可以保存的最大事件数(EPOLL_SIZE宏常量)
	timeout: 以1/1000秒为单位的等待时间，传递-1时，一直等待直到发生事件
*/


// 更改或读取文件属性
#include <fcnt1.h>

int fcnt1(int filedes, int cmd, ...);
    // 成功时返回cmd参数相关值，失败时返回-1

/*
	filedes: 属性更改目标的文件描述符
	cmd: 表示函数调用的目的

	     F_GETFL: 获得属性
	     F_SETFL: 更改属性
*/


// 线程创建
#include <pthread.h>

int pthread_create(
	pthread_t * restrict thread, const pthread_attr_t * restrict attr,
	void * (* start_routine)(void *), void * restrict arg
	);
    // 成功时返回9，失败返回其他值

/*
	thread: 保存新创建线程ID的变量地址值
	attr: 用于传递线程属性的参数，传递NULL时，默认属性
	start_routine: 相当于线程main函数的、在单独执行流中执行的函数地址值(函数指针)
	arg: 通过第三个参数传递调用函数时包含传递参数信息的变量地址值
*/


// 等待线程结束(调用该函数的进程进入等待状态，直到线程ID对应的线程终止为止)
#include <pthread.h>

int pthread_t_join(pthread_t thread, void ** status);
    // 成功时返回0，失败返回其他值

/*
	thread: 该线程ID对应的线程终止后才会从该函数返回
	status: 保存线程的main函数返回值的指针变量地址值
*/


// 互斥量
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t * mutex, const pthread_mutexattr_t * attr);
int pthread_mutex_destroy(pthread_mutex_t * mutex);
    // 创建/销毁互斥量，成功时返回0，失败返回其他值

/*
	mutex: 创建互斥量时传递保存互斥量的变量地址值，销毁时需要传递相应的互斥量地址值
	attr: 传递即将创建的互斥量属性，没有则传递NULL
*/


// 互斥量锁住/释放临界区
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t * mutex);
int pthread_mutex_unlock(pthread_mutex_t * mutex);
    // 成功时返回0，失败返回其他值


// 销毁线程(join方法会使调用方陷入阻塞)
#include <pthread.h>

int pthread_detach(pthread_t thread);
    // 成功时返回0，失败返回其他值
    // 调用该函数不会引起线程终止或进入阻塞状态，可以通过该函数引导销毁线程创建的内存空间

/*
	thread: 终止的同时需要销毁的线程ID
*/