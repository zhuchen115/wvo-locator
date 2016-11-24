#include "objectdimension.h"
#include <cmath>
#include "math_func.h"

using namespace std;

namespace slam{

	ObjectDimension::ObjectDimension()
	{
	}


	ObjectDimension::~ObjectDimension()
	{
	}
	void ObjectDimension::InitScale(float ruler, float distance)
	{
		/// Assume the camera is a pinhole camera model
		scale = ruler*distance;
	}
	float ObjectDimension::DistanceByLine(float len, float len2, float move)
	{
		/// Assume pinhole camera and geometry
		return (len2*move) / (len+len2);
	}
	float ObjectDimension::DistanceByArea(float area, float area2, float move,int approach)
	{
		switch (approach)
		{
			case AREA_APPROACH_RECT:
				float la1 = fast_invsqrt(area);
				float la2 = fast_invsqrt(area2);
				return (move*la1) / (la1 + la2);
				break;
			case AREA_APPROACH_CIRCLE:
				float r1 = sqrt(area / M_PI);
				float r2 = sqrt(area / M_PI);
				return (r1*move) / (r1 + r2);
				break;
			default:
				return 0.0;
		}

		return 0.0;
	}
	float ObjectDimension::DimensionByLine(float len, float len2, float move)
	{
		/// Assume pinhole camera and geometry
		return (len*len2*move) / (scale*(len*len2));
	}
}