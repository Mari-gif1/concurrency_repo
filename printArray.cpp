#include <pthread.h>
#include <stdio.h>
#include <cstdlib>

struct threadParam
{
	int* m_array;
	int m_left;
	int m_right;
	threadParam(int* arr, int left, int right) : m_array(arr), m_left(left), m_right(right)
	{}
	threadParam(){}
};

static void* sum(void* param)
{
	threadParam* data = (struct threadParam*)param;
	int* arr = data->m_array;
	int left = data->m_left;
	int right = data->m_right;
	int sum = 0;
	for (int i = left; i < right; ++i)
	{
		sum += arr[i];
	}

	printf("\nSum %i of the thread %ld\n", sum, pthread_self());
	delete data;
}

int main()
{
	pthread_t cThread[2];
	const int N = 20;
	int arr[N]{};
	for (int i = 0; i < N; ++i)
		arr[i] = i;
	int left = 0;
	int right = N / 2;
	for (int i = 0; i < 2; ++i)
	{
		if (pthread_create(&cThread[i], NULL, sum, (void*)(new threadParam(arr, left, right))))
		{
			perror("Thread creation error!");
			exit(EXIT_FAILURE);
		}
		left += N / 2;
		right += N / 2;
	}
	for (int i = 0; i < 2; ++i)
	{
		pthread_join(cThread[i], NULL);
	}
	return 0;
}
