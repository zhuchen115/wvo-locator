#include "objectdimension.h"
#include <opencv2/opencv.hpp>
#ifdef __GNUC__
#include <unistd.h>
#else
#include <io.h>
#endif
#include "math_func.h"

using namespace std;

namespace image_recognize{

	ObjectDimension::ObjectDimension()
	{
        memset(calibfile,0,255);
        strcat(calibfile,VAR_DOCUMENT_DIR);
        strcat(calibfile,"objdimension.xml");
#ifdef __GNUC__
		if (access(calibfile, 0)==0)
#else
		if(_access(calibfile,0)==0)
#endif
		{
			LoadScale();
		}
	}


	ObjectDimension::~ObjectDimension()
	{
	}
	float ObjectDimension::InitScale(float ruler, float distance,float length)
	{
		/// Assume the camera is a pinhole camera model
		scale = ruler*distance/length;
		return scale;
	}
	float ObjectDimension::DistanceByLine(float len, float len2, float move)
	{
		/// Assume pinhole camera and geometry
		return (len2*move) / (len-len2);
	}
	float ObjectDimension::DistanceByArea(float area, float area2, float move,int approach)
	{
		float la1, la2, r1, r2;
		switch (approach)
		{
			case AREA_APPROACH_RECT:
				la1 = fast_invsqrt(area);
				la2 = fast_invsqrt(area2);
				return (move*la1) / (la1 + la2);
				break;
			case AREA_APPROACH_CIRCLE:
				r1 = sqrt(area / M_PI);
				r2 = sqrt(area / M_PI);
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
		return (len*len2*move) / (scale*(len-len2));
	}
	float ObjectDimension::DimensionByRef(float len, float distance)
	{
		return len*distance / scale;
	}
	void ObjectDimension::LoadScale()
	{
		/// Open the camera calibrate storage
		cv::FileStorage fs(calibfile, cv::FileStorage::READ);

		fs["scale"] >> scale;

		fs.release();
	}
	void ObjectDimension::SaveScale()
	{
		cv::FileStorage fs(calibfile, cv::FileStorage::WRITE);
		fs.write("scale", scale);
		fs.release();
	}

    float ObjectDimension::DistanceByRef(float height,float href)
    {
        return (dref*href/height);
    }
}
