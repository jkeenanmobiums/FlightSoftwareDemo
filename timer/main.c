#include <stdlib.h>
#include <stdio.h>

#include <time.h>

int main()
{
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while(1)
	{
		t.tv_nsec += 100000000;
		if(t.tv_nsec >= 1000000000)
		{
			t.tv_sec += 1;
			t.tv_nsec -= 1000000000;
		}

		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

		printf("Hi\n");
	}
}
