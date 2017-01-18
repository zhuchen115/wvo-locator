/// *********************************
/// 
/// \file camera.h
///
/// Capture and/or Calibrate the image 
///
/// \author Chen Zhu
/// \version v0.1.0
/// \date 2016-11-20
/// \copyright GPL v3
///
/// ***********************************
#ifndef SLAM_CAMERA_H
#define SLAM_CAMERA_H

#include <opencv2/opencv.hpp>
#include "config.h"

using namespace cv;
using namespace std;
namespace image_recognize {

	class Camera
	{
	public:
		/// Constructor
		Camera();
		~Camera();

		/// Load Calibrate Data from Saved Data
		void LoadCalibrate();

		/// Pre-loaded Calibrate from matlab
		void InitCalibrate();
		/// Remap image
		/// \param img the Image to be remapped
		/// \return the remapped image

		Mat RemapImage(Mat img);
		/// Save the calibrate data
		void SaveCalibrate();

		/// Capture a calibrated image form camera
		/// \param index the camera index, default: 0
		/// \param sz the size of image, default: 1024*768
		/// \param exposure the exposure time default: 700
		/// \param gray Gray Scale image default: true. If capture an IR image, let it true
		Mat CapCalibrateImage(int index = 0, Size sz = Size(1024, 768),int exposure = 700, bool gray = false);

		/// Capture a raw image without calibrate
		/// \param index the camera index, default: 0
		/// \param sz the size of image, default: 1024*768
		/// \param exposure the exposure time default: 700
		/// \param gray Gray Scale image default: true. If capture an IR image, let it true
		Mat CapRawImage(int index = 0, Size sz = Size(1024, 768), int exposure = 700, bool gray = false);
	private:
		/// intrinsic matrix of camera, 3x3 double
		Mat intrinsic_matrix;
		/// Distortion Coefficient, 1x5 double
		Mat distortion_coeff;
		/// The image maps for 1024*768
		Mat mapx, mapy;
		/// Map for other size cached
		Mat mapx_f,mapy_f;
		/// The image size
		Size image_size_f;
		/// Filename of Calibrate file
		char calibfile[256] = VAR_DOCUMENT_DIR;
	};

}

#endif