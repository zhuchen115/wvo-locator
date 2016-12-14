#include "Camera.h"
#include "objectdimension.h"
#include "ContourFinder.h"
#include <stdio.h>

using namespace cv;
using namespace image_recognize;

int main()
{
	printf("Initializing Training image!\n");
	Camera cam;
	ContourFinder finder;
	finder.InitTraining();
	ObjectDimension dim;
	float scale = dim.InitScale(185.54, 0.984, 0.23);
	char filename[356];
	Mat img;
	printf("Scale is %f", scale);
	while (1) 
	{
		printf("Input the target image:");
		scanf("%s", filename);
		img = imread(filename, CV_LOAD_IMAGE_COLOR);
		// Calibrate Image
		img = cam.RemapImage(img);
		//imshow("Calibrate Image", img);
		//waitKey(0);
		ObjectInfo target,training;
		if (!finder.FindTarget(img, target))
		{
			printf("Target object not found!\n");
			continue;
		}
		finder.GetTrainingDimension(target.orientation, training);
		printf("Input the move distance:");
		float move;
		scanf("%f", &move);
		printf("Training Height:%d px, Target Height:%d px\n", training.height, target.height);
		float real_height = dim.DimensionByLine(training.height, target.height, move);
		float real_distance = dim.DistanceByLine(training.height, target.height, move);
		printf("Target height: %f (m), target distance: %f (m)\n",real_height,real_distance);
	}
}