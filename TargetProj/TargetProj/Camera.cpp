/// *********************************
/// 
/// \file camera.cpp
///
/// Implementation Capture and/or Calibrate the image 
///
/// \author Chen Zhu
/// \version v0.1.0
/// \date 2016-11-20
/// \copyright GPL v3
///
/// ***********************************

#include "Camera.h"
#ifdef __GNUC__
#include <unistd.h>
#include <syslog.h>
#define GEN_LOG 1
#else
#include <io.h>
#endif

namespace image_recognize{

	Camera::Camera()
	{
		/// default size
		image_size_f = Size(1024, 768);
#if GEN_LOG
		openlog("ppRobot.Camera", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		strcat(calibfile, "camera_calibrate.xml");
		/// Load / Initialize the calibrate data
#ifdef linux
		if (access(calibfile, 0)==0)
#else
		if (_access(calibfile, 0) == 0)
#endif
		{
			LOG_PRINT(LOG_INFO, "Found Camera Calibrate Data, Load it");
			LoadCalibrate();
		}
		else 
		{
			LOG_PRINT(LOG_INFO, "Camera Calibrate Data NOT Found, Init a new one");
			InitCalibrate();
		}
	}


	Camera::~Camera()
	{
	}

	void Camera::LoadCalibrate()
	{
		/// Open the camera calibrate storage
		FileStorage fs(calibfile, FileStorage::READ);
		/// Load the intrinsic matrix
		fs["intrinsic"] >> intrinsic_matrix;
		/// Load the distortion coefficient
		fs["distortion"] >> distortion_coeff;
		/// Load Map X
		fs["mapx"] >> mapx;
		/// Load Map Y
		fs["mapy"] >> mapy;
		/// Close the file 
		fs.release();
	}

	void Camera::InitCalibrate()
	{
#if GEN_LOG
		openlog("ppRobot.Camera", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		/// Matlab have different expression on intrinsic matrix
		/// \f [\left\{ \begin{matrix}
		///  {f_x} & 0 & {c_x} \\
		///  0 & {f_y} & {c_y} \\
		///  0 & 0 & 1
		///  \end{matrix} \right\}
		///  \f]
		float inm[3][3] = { { 2520.58754679752,	0,				 1591.42320850165 },
							{ 0,				2520.64559563684,1234.57252783749 },
							{ 0,				0,				 1 } };
		intrinsic_matrix = Mat(3, 3, CV_32FC1, inm);
		/// The distortion of lens k1,k2,p1,p2,p3
		float coef[5] = { 0.169380872868542, -0.277893897574537,0.00383818913657745,-0.00473755693866810 };
		distortion_coeff = Mat(1, 5, CV_32FC1, Scalar::all(0));
		Mat R = Mat::eye(3, 3, CV_32F);
		/// Create Cached Map for 1024*768;
		initUndistortRectifyMap(intrinsic_matrix, distortion_coeff, R, intrinsic_matrix, Size(1024, 768), CV_32FC1, mapx, mapy);
		LOG_PRINT(LOG_INFO, "Saving Camera Calibrate Data");
		/// Create the matrix storage
		FileStorage fs(calibfile, FileStorage::WRITE);
		fs.write("intrinsic", intrinsic_matrix);
		fs.write("distortion", distortion_coeff);
		fs.write("mapx", mapx);
		fs.write("mapy", mapy);
		/// Save the file
		fs.release();
#if GEN_LOG
		closelog();
#endif
	}

	Mat Camera::RemapImage(Mat img)
	{
		/// 1024*768 Image Remap
		Mat ca = img.clone();
		if (img.size() == Size(1024, 768))
		{
			remap(img, ca, mapx, mapy, INTER_LINEAR);
		}
		/// Cached Remap
		else if (img.size() == image_size_f)
		{
			remap(img, ca, mapx_f, mapy_f, INTER_LINEAR);
		}
		/// Uncached 
		else
		{
			Mat R = Mat::eye(3, 3, CV_32F);
			/// Remake the map
			initUndistortRectifyMap(intrinsic_matrix, distortion_coeff, R, intrinsic_matrix, img.size(), CV_32FC1, mapx_f, mapy_f);
			remap(img, ca, mapx_f, mapy_f, INTER_LINEAR);
		}
		return ca;
	}

	void Camera::SaveCalibrate()
	{
#if GEN_LOG
		openlog("ppRobot.Camera", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		LOG_PRINT(LOG_INFO, "Saving Camera Calibrate Data");

		/// Create the matrix storage
		FileStorage fs(calibfile, FileStorage::WRITE);
		fs.write("intrinsic", intrinsic_matrix);
		fs.write("distortion", distortion_coeff);
		fs.write("mapx", mapx);
		fs.write("mapy", mapy);
		/// Save the file
		fs.release();
	}

	Mat Camera::CapCalibrateImage(int index, Size sz,int exposure, bool gray)
	{
#if GEN_LOG
		openlog("ppRobot.Camera", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		/// Initialize a video capture
		VideoCapture capture(index);
		
		LOG_PRINT(LOG_INFO, "Capture Image from device:%d, Size:%d*%d", index,sz.width,sz.height);

		/// Set image size
		capture.set(CAP_PROP_FRAME_WIDTH, sz.width);
		capture.set(CAP_PROP_FRAME_HEIGHT, sz.height);
		/// Set exposure time
		capture.set(CAP_PROP_EXPOSURE, exposure);
		if (!capture.grab())
		{
			LOG_PRINT_ERR(LOG_WARNING, "Error in Grab image from device:%d\n",index);
			capture.release();
			return Mat();
		}
		Mat img,grayimg;
		if (!capture.retrieve(img))
		{
			LOG_PRINT_ERR(LOG_WARNING, "Error in retrive image from device:%d\n", index);
			capture.release();
			return Mat();
		}
		capture.release();
		img = RemapImage(img);
		if (gray)
		{
			cvtColor(img, grayimg, COLOR_BGR2GRAY);
			return grayimg;
		}
		else 
		{
			return img;
		}
#if GEN_LOG
		closelog();
#endif
		
	}
	Mat Camera::CapRawImage(int index, Size sz, int exposure, bool gray)
	{
#if GEN_LOG
		openlog("ppRobot.Camera", LOG_CONS | LOG_PID, LOG_LOCAL4);
#endif
		/// Initialize a video capture
		VideoCapture capture(index);


		LOG_PRINT(LOG_INFO, "Capture Image from device:%d, Size:%d*%d", index, sz.width, sz.height);

		/// Set image size
		capture.set(CAP_PROP_FRAME_WIDTH, sz.width);
		capture.set(CAP_PROP_FRAME_HEIGHT, sz.height);
		/// Set exposure time
		capture.set(CAP_PROP_EXPOSURE, exposure);
		if (!capture.grab())
		{
			LOG_PRINT_ERR(LOG_WARNING, "Error in Grab image from device:%d\n", index);
			capture.release();
#if GEN_LOG
			closelog();
#endif
			return Mat();
		}
		Mat img, grayimg;
		if (!capture.retrieve(img))
		{
			LOG_PRINT_ERR(LOG_WARNING, "Error in retrive image from device:%d\n", index);
#if GEN_LOG
			closelog();
#endif
			capture.release();
			return Mat();
		}
		capture.release();
		if (gray)
		{
			cvtColor(img, grayimg, COLOR_BGR2GRAY);
			return grayimg;
		}
		else
		{
			return img;
		}
	}
}


