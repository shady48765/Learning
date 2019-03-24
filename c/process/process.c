#include "process.h"

int main(void)
{
	int counter = 50;
	pid_t pro_id;

	while(counter--)
	{
		pro_id = fork();
		if(pro_id < 0)
		{
			perror("fork error:");
			exit(1);
		}
		if(pro_id == 0)
		{
			printf("child %d: process id = %d\n", counter, getpid());
			exit(0);
		}
		sleep(1);
	}
	printf("main process ready to close\n");
	sleep(5);

		return 0;
	}
