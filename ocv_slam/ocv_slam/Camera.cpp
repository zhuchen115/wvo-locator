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
namespace slam{

	Camera::Camera()
	{
		/// default size
		image_size_f = Size(1024, 768);
#ifdef GEN_LOG
		openlog("Robot::SLAM", LOG_PID | LOG_CONS, LOG_USER);
#endif
		/// Load / Initialize the calibrate data
		if (access("./camera_calibrate.xml", 0))
		{
#ifdef GEN_LOG
			syslog(LOG_INFO, "Found Camera Calibrate Data, Load it");
#endif
			LoadCalibrate();
		}
		else 
		{
#ifdef GEN_LOG
			syslog(LOG_INFO, "Camera Calibrate Data NOT Found, Init a new one");
#endif
			InitCalibrate();
		}
	}


	Camera::~Camera()
	{
	}

	void Camera::LoadCalibrate()
	{
		/// Open the camera calibrate storage
		FileStorage fs("./camera_calibrate.xml", FileStorage::READ);
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
		/// Matlab have different expression on intrinsic matrix
		/// \f [\left\{ \begin{matrix}
		///  {f_x} & 0 & {c_x} \\
		///  0 & {f_y} & {c_y} \\
		///  0 & 0 & 1
		///  \end{matrix} \right\}
		///  \f]
		float inm[3][3] = { { 2519.92860042713,-2.78965667954286,1582.50916974410 },
							{ 0,				2519.86474505123,1234.50470704092 },
							{ 0,				0,				 1 } };
		intrinsic_matrix = Mat(3, 3, CV_32FC1, inm);
		/// The distortion of lens k1,k2,p1,p2,p3
		float coef[5] = { 0.139899103447188,0.124744277842460,0.00328390733162216,-0.00670376512731372,-1.73399238519861 };
		distortion_coeff = Mat(1, 5, CV_32FC1, Scalar::all(0));
		Mat R = Mat::eye(3, 3, CV_32F);
		/// Create Cached Map for 1024*768;
		initUndistortRectifyMap(intrinsic_matrix, distortion_coeff, R, intrinsic_matrix, Size(1024, 768), CV_32FC1, mapx, mapy);
#ifdef GEN_LOG
		syslog(LOG_INFO, "Saving Camera Calibrate Data");
#endif
		/// Create the matrix storage
		FileStorage fs("./camera_calibrate.xml", FileStorage::WRITE);
		fs.write("intrinsic", intrinsic_matrix);
		fs.write("distortion", distortion_coeff);
		fs.write("mapx", mapx);
		fs.write("mapy", mapy);
		/// Save the file
		fs.release();
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
#ifdef GEN_LOG
		syslog(LOG_INFO, "Saving Camera Calibrate Data");
#endif
		/// Create the matrix storage
		FileStorage fs("./camera_calibrate.xml", FileStorage::WRITE);
		fs.write("intrinsic", intrinsic_matrix);
		fs.write("distortion", distortion_coeff);
		fs.write("mapx", mapx);
		fs.write("mapy", mapy);
		/// Save the file
		fs.release();
	}

	Mat Camera::CapCalibrateImage(int index, Size sz,int exposure, bool gray)
	{
		/// Initialize a video capture
		VideoCapture capture(index);
		
#ifdef GEN_LOG
		syslog(LOG_INFO, "Capture Image from device:%d, Size:%d*%d", index,sz.width,sz.height);
#endif
		/// Set image size
		capture.set(CAP_PROP_FRAME_WIDTH, sz.width);
		capture.set(CAP_PROP_FRAME_HEIGHT, sz.height);
		/// Set exposure time
		capture.set(CAP_PROP_EXPOSURE, exposure);
		if (!capture.grab())
		{
			fprintf(stderr, "Error in Grab image from device:%d\n",index);
#ifdef GEN_LOG
			syslog(LOG_INFO, "Error in Grab image from device:%d",index);
#endif
			capture.release();
			return Mat();
		}
		Mat img,grayimg;
		if (!capture.retrieve(img))
		{
			fprintf(stderr, "Error in retrive image from device:%d\n", index);
#ifdef GEN_LOG
			syslog(LOG_INFO, "Error in retrive image from device:%d", index);
#endif
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
		
	}
	Mat Camera::CapRawImage(int index, Size sz, int exposure, bool gray)
	{
		/// Initialize a video capture
		VideoCapture capture(index);

#ifdef GEN_LOG
		syslog(LOG_INFO, "Capture Image from device:%d, Size:%d*%d", index, sz.width, sz.height);
#endif
		/// Set image size
		capture.set(CAP_PROP_FRAME_WIDTH, sz.width);
		capture.set(CAP_PROP_FRAME_HEIGHT, sz.height);
		/// Set exposure time
		capture.set(CAP_PROP_EXPOSURE, exposure);
		if (!capture.grab())
		{
			fprintf(stderr, "Error in Grab image from device:%d\n", index);
#ifdef GEN_LOG
			syslog(LOG_INFO, "Error in Grab image from device:%d", index);
#endif
			capture.release();
			return Mat();
		}
		Mat img, grayimg;
		if (!capture.retrieve(img))
		{
			fprintf(stderr, "Error in retrive image from device:%d\n", index);
#ifdef GEN_LOG
			syslog(LOG_INFO, "Error in retrive image from device:%d", index);
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


