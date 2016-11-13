#include "LocTransfer.h"
#include <cmath>

/// RAD to DEG Transform
/// \f[\frac{180}{\pi}]
#define RAD_DEG 57.295779513082320876798154814105

namespace robot_control {


	LocTransfer::LocTransfer()
	{
	}


	LocTransfer::~LocTransfer()
	{
	}

	Deflection LocTransfer::direction(Point x, Size sz, double distance)
	{
		int center_x = sz.width / 2;
		Deflection def;
		float direction = atanhf(((float)x.x-center_x)/distance);
		int angle = (int)(direction * RAD_DEG);
		def.turn.direction = (direction > 0) ? 0 : 1;
		def.turn.angle1 = (angle >> 8) & 0x01;
		def.turn.angle2 = (angle & 0xff);
		return def;
	}

	double LocTransfer::distance(int length)
	{
		if (dist_unit_length)
		{
			return ((double)length) / ((double)dist_unit_length);
		}
		else {
			return 0;
		}
	}

}