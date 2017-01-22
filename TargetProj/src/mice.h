

void mice_update(void);

void mice_init(void);

/// Pause or restart the counter
int mice_pause(void);

int mice_readraw (long long *x,long long *y);

int mice_getposition(double *x,double *y);
