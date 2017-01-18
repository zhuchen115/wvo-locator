#ifndef _MICE_H
#define _MICE_H
#ifdef linux
#include <errno.h>
#endif

#ifdef __cplusplus
extern "C"{
#endif
void mice_init();
int mice_pause();
int mice_readraw(long long *x,long long *y);
int mice_getposition(double *x,double *y);

#ifdef __cplusplus
}
#endif

#endif
