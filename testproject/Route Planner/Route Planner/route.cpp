#include "route.h"



RoutePlan::RoutePlan(Mat m)
{
	_map = m;
}


RoutePlan::~RoutePlan()
{
}

void RoutePlan::fitness(Route &route)
{
	double f2; // The length of the route
	bool f1 = 1;
	
	for (int i = 0; i < route.route.size() - 1; i++)
	{
		Point pt = route.route.at(i);
		// There is an obstacle at this point
		if (_map.at<uchar>(pt) > 100)
		{
			f1 = 0;
			break;
		}
		f2 += distance(route.route.at(i), route.route.at(i + 1));
	}
	if (f1)
	{
		route.fitness = f2;
	}
	else
	{
		route.fitness = 0;
	}
}

void RoutePlan::InitPopulation(Point start, Point end)
{
}

double RoutePlan::distance(Point p1, Point p2)
{
	return sqrt(pow(p2.x-p1.x,2)+pow(p2.y-p1.y,2));
}
