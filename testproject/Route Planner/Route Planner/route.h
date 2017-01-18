#ifndef ROUTE_H
#define ROUTE_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
// Describe a route
struct Route
{
	// The route 
	vector<Point> route;
	// The fitness, the total length of the route
	double fitness;
};

class RoutePlan
{
public:
	RoutePlan(Mat m);
	~RoutePlan();
	void fitness(Route &route);
	void InitPopulation(Point start, Point end);
private:
	double distance(Point p1, Point p2);
	// Indicate the whole working space
	Mat _map;
	// The ga_population
	vector<Route> ga_population;
};

#endif