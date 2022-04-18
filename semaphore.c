#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void * read(void * arg);
void * accu(void * arg);

static sem_t sem_one;  // 用来计算
static sem_t sem_two;  // 用来读取值
static int num;

int main(int argc, char * argv[])
{
	pthread_t id_t1, id_t2;
	// 初始化信号量
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);   // 先读后计算

	// 创建读线程，计算线程
	pthread_create(&id_t1, NULL, read, NULL);
	pthread_create(&id_t2, NULL, accu, NULL);

	// 等待线程结束
	pthread_join(id_t1, NULL);
	pthread_join(id_t2, NULL);

	// 销毁信号量
	sem_destroy(&sem_one);
	sem_destroy(&sem_two);

	return 0;
}

void * read(void * arg)
{
	for (int i = 0; i < 5; ++i)
	{
		// 用户输入5个数字
		fputs("Input num: ", stdout);
		// 请求two信号量，保证上一个值已被读取
		sem_wait(&sem_two);
		scanf("%d", &num);
		// 释放sem_one信号量，保证接下来能够读取数据
		sem_post(&sem_one);
	}
	return NULL;
}

void * accu(void * arg)
{
	int sum = 0;
	for (int i = 0; i < 5; ++i)
	{
		// 请求one信号量，保证已输入数据
		sem_wait(&sem_one);
		sum += num;
		// 释放two信号量，读取之后继续读取数据
		sem_post(&sem_two);
	}
	printf("Result: %d\n", sum);
	return NULL;
}

