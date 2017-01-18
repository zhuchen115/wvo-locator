/**
*********************************
* @file robotentry.cpp
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


#include <opencv2\opencv.hpp>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include "mice.h"
#include "objectdimension.h"
#include "Camera.h"
#include "ContourFinder.h"
#include "spi_driver.h"

using namespace cv;
using namespace std;
using namespace image_recognize;
using namespace robot_control;

int main()
{
	//Initialize the Camera
	Camera cam;
	//Initialize the Recognize System
	ContourFinder finder;
	//Initialize the SPI
	ControllerPort port;
	//Star the mesaurement
	ObjectDimension dim;
	// Store the caputred image
	Mat img;
	//Object Infomation
	ObjectInfo target1, target2;
	// The cm axis from mouse displacement
	double x, y,x1,y1,movedist;
	float targetdist, objectoffset;

	Deflection def;
	
	finder.InitTraining();
	dim.InitScale(185.54, 0.984, 0.23);
	mice_init();

	while (1)
	{
		// Capture one image
		img = cam.CapCalibrateImage();
		if (!img.data)
		{
			LOG_PRINT_ERR(LOG_WARNING, "Cannot Capture Image!");
			break;
		}
		if (!finder.FindTarget(img, target1))
		{
			LOG_PRINT(LOG_INFO, "No Target Found");
			// TODO: Add some random route
		}
		// Detect the robot is approach the target
		if (target1.height > 400)
		{
			// The chair should be hooked
			if (target1.orientation == 0&& fabs(target1.offset)<HOOK_MAX_OFFSET)
			{
				LOG_PRINT(LOG_DEBUG, "Target Approached, Ready to take the target");
				// Set the hook released for hook target
				port.TargetTool(0x01, 1);
				sleep(1);
				LOG_PRINT(LOG_DEBUG, "Try to hook target..");
				// Move 0.8m
				port.MoveForward(20, 40);
				// Wait 3 seconds for move
				sleep(3);
				// Set the hook locked
				port.TargetTool(0x01, 0);
				// Wait 1 second for hook
				sleep(1);
				// Move back 0.8m to avoid distrubance
				port.MoveBackward(10, 40);
				// Rotate the camera and move the camera above the robot for navigation;
				port.CamAngle(180);
				sleep(5);
				port.CamHeight(120);
				// TODO: Move the chair to point
			}
			// To adjust the robot, the robot has to go back 1.5m.
			port.MoveBackward(50, 75);
			sleep(2);
		}
		else //The target has more than 0.8m away
		{
			mice_getposition(&x, &y);
			//Move forward half meters
			port.MoveForward(30, 25);
			sleep(2);
			mice_getposition(&x1, &y1);
			double dy = y1 - y, dx = x1 - x;
			//Calculate the real move distance
			movedist = sqrt(dx*dx + dy*dy)/100;
			img = cam.CapCalibrateImage();
			if (!finder.FindTarget(img, target2))
			{
				LOG_PRINT(LOG_DEBUG, "Target Missing, Fallback");
				port.MoveBackward(30, 75);
				sleep(4);
				continue;
			}

			targetdist = dim.DistanceByLine(target1.height, target2.height, movedist) - movedist;
			LOG_PRINT(LOG_DEBUG, "Find a target object in %f m, the orientation of it is %d",targetdist, target1.orientation*45);
			if (target1.orientation == 0)
			{
				// Get the offset of object
				objectoffset = dim.DimensionByLine(target1.offset, target2.offset, movedist);
				// Target cannot be captured
				if (objectoffset > 0.2) 
				{
					LOG_PRINT(LOG_DEBUG, "The object is on the side, adjusting to %f m", objectoffset);
					
					def.turn.direction = (target2.offset > 0) ? 0 : 1;
					def.turn.angle2 = 90;
					def.turn.angle1 = 0;
					// Turn 90 degree
					port.Heading(def);
					sleep(4);
					// Now make the robot and object straight forward
					do {
						unsigned char dist = (objectoffset > 5) ? 250 : (unsigned char)objectoffset * 50;
						port.MoveForward(50, dist);
						sleep(6);
						objectoffset -= 5;
					} while (objectoffset > 0.2);
					// Now turn back;
					def.turn.direction = 1 - def.turn.direction;
					port.Heading(def);
					sleep(4);
				}
				else 
				{
					LOG_PRINT(LOG_INFO, "The object can be hooked, moving");
					port.MoveForward(70, (targetdist - 0.5)*50);
					sleep(8);
				}
			}
			// Target has an orientation
			else
			{
				LOG_PRINT(LOG_INFO, "Make the head anticlockwisely 70 degree");
				def.turn.direction = 1;
				def.turn.angle2 = 70;
				def.turn.angle1 = 0;
				port.Heading(def);
				sleep(4);
				//Go 2*d*sin20 m
				port.MoveForward(34, 34.2*targetdist);
				sleep(6);
				//Make the vision back to the target
				def.turn.direction = 0;
				def.turn.angle2 = 110;
				port.Heading(def);
				sleep(4);
				continue;
			}
		}

	}

}