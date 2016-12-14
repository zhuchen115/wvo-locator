#include "Maps.h"

namespace slam {

	void Maps::putLine(Point location, float distance, float width, float offset, float angle)
	{
		float csa = cos(angle), sa = sin(angle);
		float dx = distance*csa + offset*sa;
		float dy = distance*sa - offset*csa;
		Point start,end;
		/// Convert the world axis to map axis
		start.x = location.x + dx / 5;
		start.y = location.y + dy / 5;
		end.x = (offset < 0) ? start.x - (width*sa/5): start.x + (width*sa/5);
		end.y = (offset < 0) ? start.y + (width*csa / 5) : start.y - (width*sa / 5);
		/// Draw a line on map
		line(map, start, end, Scalar(255));
	}

	Maps::Maps()
	{
		/// Assume the work space is 10m*10m, 5cm resolution
		/// Note that the real map is a circle
		map = Mat(2000, 2000, CV_8UC1);
	}


	Maps Maps::GetInstance()
	{
		static Maps _map;
		return _map;
	}



	Maps::~Maps()
	{
	}
}