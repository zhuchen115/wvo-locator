#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <syslog.h>
#include <errno.h>
#include "config.h"
#include "mice.h"

long long abs_x = 0, abs_y = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* mice_update(void*)
{
	int fd,bytes;
	signed char mdata[3];
	
	const char *pDev = "/dev/input/mice";
	fd = open(pDev,O_RDWR);
	if(fd == -1)
	{
		LOG_PRINT(LOG_WARNING,"Error in Opening %s",pDev);
		return ((void *)0);;
	}
	
	while(1)
	{
		bytes =read(fd,mdata,sizeof(mdata));
		if(bytes > 0)
		{
			pthread_mutex_lock(&mutex);
			abs_x += mdata[1];
			abs_y += mdata[2];
			pthread_mutex_unlock(&mutex);
		}
	}
    return ((void *)0);;
}

void mice_init()
{
	pthread_t threadm;
	if(pthread_create(&threadm,NULL,mice_update,NULL))
	{
		LOG_PRINT(LOG_ERR,"Error in creating mouse measurement thread");
		return;
	}
	
}

/// Pause or restart the counter
int mice_pause()
{
	/// Try lock the lock to block the loop;
	int lstate=pthread_mutex_lock(&mutex);
	switch(lstate)
	{
		case EDEADLK:
			// Unlock due to called second time.
			pthread_mutex_unlock(&mutex);
			return 1;
			break;
		case 0:
			return 0;
	}
}

int mice_readraw (long long *x,long long *y)
{
	int lstate=pthread_mutex_lock(&mutex);
	*x = abs_x;
	*y = abs_y;
	pthread_mutex_unlock(&mutex);
	return 1;
}

int mice_getposition(double *x,double *y)
{
	pthread_mutex_lock(&mutex);
	*x = ((double)abs_x)/MICE_X_COEF;
	*y = ((double)abs_y)/MICE_Y_COEF;
	pthread_mutex_unlock(&mutex);
	return 1;
}
