#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* reverse(void* arg)
{
    int len = strlen(arg);

    char *new_string = (char *)malloc(len);

    for(int i = 0; i < len; ++i)
        new_string[i] = ((char*)arg)[len - i - 1];

    return new_string;
}

int main(int argc, char**argv)
{
	if(argc<2)
	{
		printf("compileaza cum trebuie ba\n");
		return -1;
	}
	pthread_t thr;
	if(pthread_create(&thr, NULL, reverse, argv[1]))
	{
		perror(NULL);
		return errno;
	}
	void *result;
	if(pthread_join(thr, &result))
	{
		perror(NULL);
		return errno;
	}
	char* afis = (char*)result;
	printf("%s",afis);
	return 0;
}
