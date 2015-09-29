#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void * thread_func_1(void *arg)
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		t.tv_nsec += 10000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		//Execute Code here
	    //read data
	    /*
	     * Now read what the server put in the memory.
	     */
	    for (s1 = shm1; *s1 != NULL; s1++)
	        putchar(*s1);
	    putchar('\t');
	    for (s2 = shm2; *s2 != NULL; s2++)
	        putchar(*s2);
	    putchar('\n');
		//end execute Code here
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
		

		for (s1 = shm1; *s1 != NULL; s1++)
	        putchar(*s1);
	    putchar('\t');
	    for (s2 = shm2; *s2 != NULL; s2++)
	        putchar(*s2);
	    putchar('\n');
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
     * Locate the segment.
     */
    if ((shmid1 = shmget(key1, SHMSZ1, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm1 = shmat(shmid1, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    /*
     * We'll name our shared memory segment
     * "5679".
     */
    key2 = 5679;

    /*
     * Locate the segment.
     */
    if ((shmid2 = shmget(key2, SHMSZ2, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm2 = shmat(shmid2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    return 0;
}

int main()
{
	sharedMemoryInit();
	Thread_1_Counter = 0;
	Thread_2_Counter = 0;
	pthread_t thread_1, thread_2;
	pthread_create(&thread_1, NULL, thread_func_1, NULL);
	pthread_create(&thread_2, NULL, thread_func_2, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);
}
