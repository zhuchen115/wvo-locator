/// *************************************
///
/// \file ContourFinder.cpp
/// Find and detect the object from the image
/// The image should be calibrated before put into use
///
/// \author Chen ZHU
/// \version v0.1.0
/// \date 2016-12-4
/// \copyright GPL v3
///
/// **************************************

#include "ContourFinder.h"
#include "ColorFilter.h"
#include "Camera.h"
/// Using access function
#ifdef __GNUC__
#include <unistd.h>
#include <syslog.h>
#else
#include <io.h>
#endif

namespace image_recognize {

	ContourFinder::ContourFinder()
	{
	}

	void ContourFinder::LoadTraining()
	{
		/// The training file not exist
#ifdef __GNUC__
		if (access(CONTOUR_CACHE_FILE, 0) < 0)
#else
		if (_access(CONTOUR_CACHE_FILE, 0) < 0)
#endif
		{
			InitTraining();
			return;
		}
		/// Check the file is readable
#ifdef __GNUC__
		if (access(CONTOUR_CACHE_FILE, 2) < 0)
#else
		if (_access(CONTOUR_CACHE_FILE, 2) < 0)
#endif
		{
#if GEN_LOG
			openlog("ppRobot.Contours", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
			LOG_PRINT_ERR(LOG_WARNING, "Cannot read training file from %s", CONTOUR_CACHE_FILE);		
#if GEN_LOG
			closelog();
#endif
			return;
		}
		FileStorage fs(CONTOUR_CACHE_FILE, FileStorage::READ);
		int sz;
		char cname[35];
		vector<Point> contour;
		fs["csize"] >> sz;
		for (int i = 0; i < sz; i++)
		{
			sprintf(cname, "cont_%d", i);
			fs[cname] >> contour;
			contours_training.push_back(contour);
		}
		fs.release();
	}

	/**
	* Initialize the Training contours from image
	*/
	void ContourFinder::InitTraining()
	{
		char filename[512]; // The filename of image
		Mat image_train; // The image to load
		vector<vector<Point>> contours_chairs; // Store the contours found
		Camera cam;
#if GEN_LOG
		openlog("ppRobot.Contours", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		for (int i = 0; i < 360; i += 45)
		{
			sprintf(filename, "%s/trainimg/o%d_m1.jpg", SHARE_DOCUMENT_DIR, i);
			LOG_PRINT(LOG_INFO, "Loading training image from: %s", filename);
			image_train = imread(filename, CV_LOAD_IMAGE_COLOR);
			if (!image_train.data)
			{
				LOG_PRINT_ERR(LOG_WARNING, "Cannot open training image file: %s", filename);
			}
			const Vec3b min(0, 0, 109), max(255, 255, 255);

			/// Fix the distortion
			image_train = cam.RemapImage(image_train);
			/// Filter the color
			image_train = ColorFilter::HSV_threshold(image_train, min, max);
			Mat gimage_train(image_train.size(), CV_8UC1);
			/// Make the image into grayscale 
			cvtColor(image_train, gimage_train, CV_BGR2GRAY);
			/// Save some memory on embedded system
			image_train.release();
			if (FindContoursByArea(gimage_train, contours_chairs)) // Contour(s) found
			{
				if (contours_chairs.size() == 1) 
				{
					
					double area = fabs(contourArea(contours_chairs.at(0)));
					LOG_PRINT(LOG_INFO, "Found training contour, area: %lf", area);
					Rect r = boundingRect(contours_chairs.at(0));
                    printf("Traing Height %d\n",r.height);
                    contours_training.push_back(contours_chairs.at(0));
					/// Clear the contours found for next round
					contours_chairs.clear();
				}
				else
				{
					double maxarea = 0;

					vector<Point> selected;
					/// Find the biggest one
					for (int j = 0; j < contours_chairs.size(); j++)
					{
						double area = fabs(contourArea(contours_chairs.at(j)));
						if (maxarea < area)
						{
							maxarea = area;
							selected = contours_chairs.at(j);
						}
					}
                    Rect r = boundingRect(selected);
                    printf("Training Height %d",r.height);
					contours_training.push_back(selected);
					/// Clear the contours for next round
					contours_chairs.clear();
					LOG_PRINT(LOG_INFO, "Found training contour, area: %lf", maxarea);
				}
			}
			else // Contours not found
			{
				LOG_PRINT_ERR(LOG_WARNING, "Contours NOT Found from training image, training image name: %s", filename);
			}

		}
#if GEN_LOG
		closelog();
#endif
		SaveTraining();
	}
	/*!
	Save the training contours
	*/
	void ContourFinder::SaveTraining()
	{
		/// Check the file is writeable
#ifdef __GNUC__
		if ((access(CONTOUR_CACHE_FILE,0)==0)&&(access(CONTOUR_CACHE_FILE, 2) < 0))
#else
		if ((_access(CONTOUR_CACHE_FILE, 0) == 0) && (_access(CONTOUR_CACHE_FILE, 2) < 0))
#endif
		{
#if GEN_LOG
			openlog("ppRobot.Contours", LOG_CONS | LOG_PID, LOG_LOCAL4);
			syslog(LOG_WARNING, "Cannot write file to %s", CONTOUR_CACHE_FILE);
			closelog();
#endif
			fprintf(stderr, "Cannot write contour training file to %s", CONTOUR_CACHE_FILE);
			return;
		}
		FileStorage fs(CONTOUR_CACHE_FILE, FileStorage::WRITE);
		fs.write("csize", contours_training.size());
		char cname[100];
		for (int i = 0; i < contours_training.size(); i++)
		{
			sprintf(cname, "cont_%d", i);
			fs.write(cname, contours_training.at(i));
		}
		
		fs.release();
	}

	/**
	* Find Contours and filter by area range
	* @param grayimg The gray image input
	* @param contours_out The found contours
	* @param minarea The mininum area of the contours
	* @param maxarea The maxinum area of the contours
	* @return the number of contours found
	*/
	int ContourFinder::FindContoursByArea(Mat grayimg, vector<vector<Point>>& contours_out, float minarea, float maxarea)
	{
		vector<vector<Point>> contours;
		findContours(grayimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		Mat draw(grayimg.size(), CV_8UC1, Scalar(0));

		for (int i = 0; i < contours.size(); i++)
		{
			// Calculate the area of contours
			double area = fabs(contourArea(contours.at(i)));

			if (area > minarea && area<maxarea)
			{
				contours_out.push_back(contours.at(i));
			}
		}
		contours.clear();
		return contours_out.size();
	}

	int ContourFinder::FindTarget(Mat image, ObjectInfo & info)
	{
#if GEN_LOG
		openlog("ppRobot.Contours", LOG_CONS | LOG_PID, LOG_LOCAL4);
		syslog(LOG_DEBUG, "Finding target in the image");
#endif
		/// Filter the image using adaptive filter
		Mat image_match = ColorFilter::AdaptiveVFilter(image);
		Mat image_mgray(image_match.size(), CV_8UC1); // Store the grayscale image
		vector<vector<Point>> contours_match; // Store the contours found
													  /// Convert the image into gray scale
		cvtColor(image_match, image_mgray, CV_BGR2GRAY);
		/*imshow("Gray Match Image", image_mgray);
		waitKey(0);*/
		/// Release the memory
		image_match.release();

		if (!FindContoursByArea(image_mgray, contours_match))
		{
			LOG_PRINT(LOG_NOTICE, "Cannot find target contour in the image");
			return 0;
		}

		double comres, min_comres = 1000, comres_sum; 
		int bcontour,bangle;
		//FILE* fp;
		//fp = fopen("D:/code/vsproj/wvo-locator/var_share/Hu.txt","a+");
		for (int agl = 0; agl < contours_training.size(); agl++)
		{
			for (int j = 0; j < contours_match.size(); j++)
			{
				if (agl == 0) {
					printf("Contour No.%d\n", j);
		//			fprintf(fp, "Contour No.%d\n", j);
					Moments mon = moments(contours_match.at(j));
					double hum[7];
					HuMoments(mon, hum);
					for (int k = 0; k < 7; k++)
					{
						printf("Hu[%d]=%.15e\n", k, hum[k]);
		//				fprintf(fp,"Hu[%d]=%.15e\n", k, hum[k]);
					}
					printf("\n-----------------\n");
				}
				comres_sum = 0;
				//double area = fabs(contourArea(contours_match.at(j)));
				comres = matchShapes(contours_training.at(agl), contours_match.at(j), CV_CONTOURS_MATCH_I1, 0);
				comres_sum += comres;
				comres = matchShapes(contours_training.at(agl), contours_match.at(j), CV_CONTOURS_MATCH_I2, 0);
				comres_sum += comres;
				comres = matchShapes(contours_training.at(agl), contours_match.at(j), CV_CONTOURS_MATCH_I3, 0);
				comres_sum += comres;
				// Find out best match
				if (comres_sum < min_comres)
				{
					min_comres = comres_sum;
					bcontour = j;
					bangle = agl;
				}
			}
		//	fprintf(fp, "A:%d,C:%lf,BA:%d\n", agl, comres_sum, bangle);
			LOG_PRINT(LOG_INFO, "Contours matching angle %d at best comeres sum %lf, best decided angle: %d,", agl,comres_sum,bangle);
		}
		
		if (min_comres < CONTOUR_COMERS_THRESHOLD)
		{
		//	fprintf(fp, "Best Angle:%d, Comres: %f\n", bangle, min_comres);
		//	fclose(fp);
			LOG_PRINT(LOG_INFO, "Best Angle: %d, Comres: %f\n", bangle, min_comres);
			info.orientation = bangle;
			info.area = fabs(contourArea(contours_match.at(bcontour)));
			Rect r = boundingRect(contours_match.at(bcontour));
			info.offset = r.x + (r.width >> 1) - (image.size().width >> 1);
			info.height = r.height;
			info.width = r.width;
			return 1;
		}
		else
		{
			LOG_PRINT(LOG_INFO, "Contour not found\n");
		//	fclose(fp);
			return 0;
		}
		
	}

	int ContourFinder::GetTrainingDimension(int angle, ObjectInfo & dimension)
	{
		vector<Point> contour = contours_training.at(angle);
		Rect r = boundingRect(contour);
		dimension.area = fabs(contourArea(contour));
		dimension.height = r.height;
		dimension.width = r.width;
		return 1;
	}


	ContourFinder::~ContourFinder()
	{
	}
}
