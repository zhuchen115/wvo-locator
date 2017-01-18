#include <stdio.h>
#include "mice.h"

int main()
{
	char cmd;
	mice_init();
	long long ax,ay;
	double x,y;
	while(1)
	{
		cmd=getchar();
		fflush(stdin);
		switch(cmd)
		{
			case 'e':
				return 1;
			case 'r':
				mice_readraw(&ax,&ay);
				printf("Raw X: %lld, Y: %lld\n",ax,ay);
				break;
			case 'd':
				mice_getposition(&x,&y);
				printf("Axis X: %lf, Y: %lf\n",x,y);
				break;
		}
	}
}
