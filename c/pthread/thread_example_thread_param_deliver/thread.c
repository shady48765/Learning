#include "thread.h"

void  *pthread_func(void *arg)
{
	int current_thread_id;
	//get current thread id
	current_thread_id = (unsigned int)pthread_self();
	printf("new thread, thread id = %u ...\n", \
	current_thread_id);
	sleep(1);
	pthread_exit(NULL);
	return NULL;

}
int main(void)
{
	
	int pthread_fd;
	pthread_t thread_id;
	int counter = 30;
	printf("\n..........main thread.....\n");
	while(counter--)
	{
		pthread_create(&thread_id, NULL,\
		pthread_func, NULL);
		//print created thread id
		printf("creat thread id = %u\n", \
		(unsigned int)thread_id);
		pthread_join(thread_id, NULL); //wait thread exit
	}
	pthread_exit(0);


	return 0;
}
