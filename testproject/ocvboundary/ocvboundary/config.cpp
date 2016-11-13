#include "config.h"



config::config()
{
}


config::~config()
{
}

void config::getRGBTherhold(RGB_CHANNEL ch, unsigned char * start, unsigned char * end)
{
	switch (ch)
	{
	case CHANNEL_R:
		*start = rstart;
		*end = rend;
		break;
	case CHANNEL_G:
		*start = gstart;
		*end = gend;
		break;
	case CHANNEL_B:
		*start = bstart;
		*end = bend;
		break;
	}
}

unsigned char config::setRGBTherhold(RGB_CHANNEL ch, unsigned char start, unsigned char end)
{
	switch (ch)
	{
	case CHANNEL_R:
		rstart = start;
		rend = end;
		break;
	case CHANNEL_G:
		gstart = start;
		gend = end;
		break;
	case CHANNEL_B:
		bstart = start;
		bend = end;
		break;
	}
	return 1;
}

