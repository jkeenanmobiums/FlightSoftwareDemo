//monitoring code

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//Shared Memory
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSZ1 27
#define SHMSZ2 27

//global defined values
//first variable
int shmid1;
key_t key1;
char *shm1, *s1;
//second variable
int shmid2;
key_t key2;
char *shm2, *s2;

//END Shared Memory

int Thread_1_Counter;
int Thread_2_Counter;
int checker;

//Read Thermocouples
void * thread_func_1(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		///10 hz
		t.tv_nsec += 100000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		//Execute Code here
		Thread_1_Counter++;
	    s1 = shm1;
	    //write data
	    char arr[20];
	    sprintf(arr, "%d", Thread_1_Counter);
	    int i;
	    for (i = 0; i < sizeof(arr); i++)
	    {
	    	*s1++ = arr[i];
	    }
	    *s1 = NULL;
		printf("Thread 1: %d\t Thread 2: %d\n", Thread_1_Counter, Thread_2_Counter);
		printf("ThermoCouples Reading.")
		//end execute Code here
	}	
}

//Read Pressure Transducers
void * thread_func_2(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		//10hz
		t.tv_nsec += 100000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		//Execute Code here
		Thread_2_Counter++;
	    s2 = shm2;
	    //write data
	    char arr[20];
	    sprintf(arr, "%d", Thread_2_Counter);
	    int i;
	    for (i = 0; i < sizeof(arr); i++)
	    {
	    	*s2++ = arr[i];
	    }
	    *s2 = NULL;
		printf("Thread 1: %d\t Thread 2: %d\n", Thread_1_Counter, Thread_2_Counter);
		printf("Pressure Transducers Reading\n");
	}
}

//Valves Output
void * thread_func_3(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		//10hz
		t.tv_nsec += 100000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		printf("Outputting to Valves\n");
	}
}

//State Machine and Abort Check
void * thread_func_4(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		//10hz
		t.tv_nsec += 100000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
		
		printf("Checking Aborts\n");
	}
}



void * sharedMemoryInit()
{
	//sharedVariable creating segment

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key1 = 5678;

    /*
     * Create the segment.
     */
    printf("%d/n", SHMSZ2);
    if ((shmid1 = shmget(key1, SHMSZ2, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key2 = 5679;

    /*
     * Create the segment.
     */
    printf("%d/n", SHMSZ2);
    if ((shmid2 = shmget(key2, SHMSZ2, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    
}

int main()
{
	sharedMemoryInit();
	Thread_1_Counter = 0;
	Thread_2_Counter = 0;
	pthread_t thread_1, thread_2, thread_3, thread_4;
	pthread_create(&thread_1, NULL, thread_func_1, NULL);
	pthread_create(&thread_2, NULL, thread_func_2, NULL);
	pthread_create(&thread_3, NULL, thread_func_3, NULL);
	pthread_create(&thread_4, NULL, thread_func_4, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
	pthread_join(thread_3, NULL);
	pthread_join(thread_4, NULL);
}
