#include <pthread.h>
#include <stdio.h>
static void* print(void* /*param*/)
{
	printf("Thread %lu is running\n", pthread_self());
}
int main()
{
	pthread_t cThread[3];
	for (int i = 0; i < 3; ++i)
	{
		pthread_create(&cThread[i], NULL, print, NULL);
	}
	
	for (int i = 0; i < 3; ++i)
	{
		pthread_join(cThread[i], NULL);
	}
	return 0;
}
