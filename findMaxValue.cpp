#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>

const int N = 10;
int arr[N]{ 0 };
pthread_barrier_t barrier;
int count = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* doSmth(void* args)
{
	int* i = (int*)args;
	while (true)
	{
		arr[*i] = std::rand() % 6 + 1;
		pthread_mutex_lock(&mymutex);
		if (++count == N)
			pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mymutex);
		sleep(1);
		pthread_barrier_wait(&barrier);
	}
	delete i;
}

int max_value(int* arr, int size)
{
	int max = arr[0];
	for (int i = 1; i < size; ++i)
		max = max < arr[i] ? arr[i] : max;
	return max;
}

int main()
{
	pthread_t th[N];
	pthread_barrier_init(&barrier, NULL, N + 1);
	for (int i = 0; i < N; ++i)
	{
		if (pthread_create(&th[i], NULL, &doSmth, new int(i)))
			perror("Failed to create thread");
	}

	while (true)
	{
		pthread_mutex_lock(&mymutex);
		while (count < N)
			pthread_cond_wait(&cond, &mymutex);
		int max = max_value(arr, N);
		std::cout << "max value: " << max<<'\n';
		pthread_mutex_unlock(&mymutex);
		pthread_barrier_wait(&barrier);
	}

	return 0;
}
