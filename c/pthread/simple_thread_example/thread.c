#include "thread.h"

void  *pthread_func(void *arg)
{
	pthread_t current_thread_id;
	//get current thread id
	current_thread_id = pthread_self();
	printf("new thread, thread id = %lu ...\n", \
	current_thread_id);
	pthread_exit(NULL);
	return NULL;

}
int main(void)
{
	
	int pthread_fd;
	pthread_t thread_id;
	int counter = 10;
	printf("\n..........main thread.....\n");
	while(counter--)
	{
		pthread_create(&thread_id, NULL,\
		pthread_func, NULL);
		//print created thread id
		printf("creat thread id = %lu\n", thread_id);
		sleep(1);
		//wait thread exit every time, it can set same thread id everytime.
		//pthread_join(thread_id, NULL);
		
	}
	pthread_join(thread_id, NULL);	// wait last thread exit
	printf("\n--------- main thread exit ---------\n");
	pthread_exit(0);				// main thread exit

	return 0;
}
