#ifndef SLAM_MAP_H
#define SLAM_MAP_H

#include "ocv_inc.h"
using namespace cv;

namespace slam {
	class Maps
	{
	public:
		static Maps GetInstance();
		~Maps();

		/// Put a line obstacle on the map (horizontal line on the image)
		/// \param location Location of robot in map coordinate system
		/// \param distance the distance of line to robot (cm)
		/// \param width the width of the object (cm)
		/// \param offset The distance between center and image center (cm)
		/// \param angle the angle between line and x axis
		void putLine(Point location,float distance,float width,float offset, float angle);

	private:

	protected:
		Maps();
		Mat map;
	};
}
#endif
