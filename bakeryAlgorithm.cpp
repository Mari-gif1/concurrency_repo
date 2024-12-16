#include <pthread.h>
#include <stdio.h>
#include <climits>

#define THREADS_COUNT 2

int x = 5;
int number[THREADS_COUNT]{ 0 };
int choosing[THREADS_COUNT]{ false };

int max(int* arr, int size)
{
	int max = arr[0];
	for (int i = 1; i < size; ++i)
		max = arr[i] > max ? arr[i] : max;
	return max;
}

void lock(int i)
{
	choosing[i] = true;
	number[i] = max(number, THREADS_COUNT) + 1;
	choosing[i] = false;
	for (int j = 0; j < THREADS_COUNT; ++j)
	{
		if (i == j) continue;
		while (choosing[j]);
		while (number[j] != 0 && (number[j] < number[i] || (number[j] == number[i] && j < i)));
	}
}

void unlock(int i)
{
	number[i] = 0;
}

void* increment(void* arg)
{
	int* i = (int*)arg;
	lock(*i);
	x++;
	unlock(*i);
	return NULL;
}

void* decrement(void* arg)
{
	int* i = (int*)arg;
	lock(*i);
	x--;
	unlock(*i);
	return NULL;
}


int main() {
    pthread_t threads[THREADS_COUNT];
    int thread_ids[THREADS_COUNT] = { 0, 1 };

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[0], NULL, increment, &thread_ids[0]);
        pthread_create(&threads[1], NULL, decrement, &thread_ids[1]);

        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);

        printf("Final value of x: %d\n", x);
    }

    return 0;
}
