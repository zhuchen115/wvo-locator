/**
*********************************
* @file recognizedemo.cpp
* This is the entry of Robot Control
* The program runs on Raspberry Pi 3
* 
* @author Chen ZHU
* @version v0.1.0
* @date 2017-1-12
* @copyright GPL v3
*
*********************************
*/


#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstdlib>
#ifdef __GNUC__
#include <unistd.h>
#else
#include <io.h>
#endif

#include "objectdimension.h"
#include "Camera.h"
#include "ContourFinder.h"


using namespace cv;
using namespace std;
using namespace image_recognize;

int main()
{
	//Initialize the Camera
	Camera cam;
	//Initialize the Recognize System
	ContourFinder finder;
	//Initialize the SPI
	//ControllerPort port;
	//Star the mesaurement
	ObjectDimension dim;
	// Store the caputred image
	Mat img;
	//Object Infomation
	ObjectInfo target1, train;
	// The cm axis from mouse displacement
	double x, y,x1,y1,movedist;
	float targetdist, objectoffset,direction;
	char fname[255];
	//Deflection def;
    printf("Initializing Training\n");	
	finder.InitTraining();
	dim.InitScale(23.59, 1.10, 0.28);
	//mice_init();
    printf("Initilization Finished\n");
	while (1)
	{
		//printf("Input the file name:");
		//scanf_s("%s", fname, 255);
		//img = imread(fname);
		// Capture one image
		img = cam.CapCalibrateImage();
		//img = cam.RemapImage(img);
		if (!img.data)
		{
			LOG_PRINT_ERR(LOG_WARNING, "Cannot Capture Image!");
			break;
		}
        printf("Finding Target\n");
		if (!finder.FindTarget(img, target1))
		{
			LOG_PRINT(LOG_INFO, "No Target Found");
			// TODO: Add some random route
            continue;
		}
		finder.GetTrainingDimension(target1.orientation, train);
        targetdist = dim.DistanceByRef(target1.height,train.height);
        objectoffset = dim.DimensionByRef(target1.offset,targetdist);
        direction = atan(objectoffset/targetdist)*180/CV_PI;
		printf("Orientation Traing Height: %dpx, Target Height: %dpx\n", train.height, target1.height);
        printf("Target Distance %fm, Orientation %d, Target Direction %f¡ã\n",targetdist,target1.orientation*45,direction);



		

	}

}
