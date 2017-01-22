#include "math_func.h"

float fast_invsqrt(float val)
{
	long i;
	float x2, y;
	const float thf = 1.5f;

	x2 = val*0.5F;
	y = val;
	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y *(thf - (x2*y*y));
	return y;
}
