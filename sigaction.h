struct sigaction
{
	void (*sa_handler)(int);    // 信号处理函数指针值
	sigset_t sa_mask;    // 信号相关选项和特性，初始化为0
	int sa_flags;    // 同上一个参数
}