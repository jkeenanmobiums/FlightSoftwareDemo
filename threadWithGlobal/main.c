#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Thread_1_Counter;
int Thread_2_Counter;
int checker;

void * thread_func_1(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		t.tv_nsec += 500000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		//Thread_1_Counter++;
		printf("Thread 1: %d\t Thread 2: %d\n", Thread_1_Counter, Thread_2_Counter);
	}
}

void * thread_func_2(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		t.tv_nsec += 800000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		Thread_2_Counter++;
		printf("Thread 1: %d\t Thread 2: %d\n", Thread_1_Counter, Thread_2_Counter);
	}
}

int main()
{
	Thread_1_Counter = 0;
	Thread_2_Counter = 0;
	pthread_t thread_1, thread_2;
	pthread_create(&thread_1, NULL, thread_func_1, NULL);
	pthread_create(&thread_2, NULL, thread_func_2, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
}
