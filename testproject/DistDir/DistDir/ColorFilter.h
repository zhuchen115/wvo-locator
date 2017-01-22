#ifndef COLOR_FILTER_H
#define COLOR_FILTER_H

#include <opencv2/opencv.hpp>
#include "config.h"

using namespace cv;
namespace image_recognize {
	class ColorFilter
	{

	public:
		~ColorFilter();
		/**
		* Filter Color By HSV color space
		* @param img the image to be filtered
		* @param min the min value of color in HSV
		* @param max the max value of color in HSV
		* @return The filterd image
		*/
		static Mat HSV_threshold(Mat img, Vec3b min, Vec3b max);

		/**
		* Filter Color By BGR color space
		* @param img the image to be filtered
		* @param min the min value of color in BGR
		* @param max the max value of color in BGR
		* @return The filterd image
		*/
		static Mat RGB_threshold(Mat in, Vec3b min, Vec3b max);

		/**
		* Filter Gray image value
		* @param img the image to be filtered
		* @param min the min value of pixel
		* @param max the max value of pixel
		* @return The filterd image
		*/
		static Mat Gray_therhold(Mat img, uchar min, uchar max);

		/**
		* Using Adaptive parameter of the image
		* Used to remove the shade from image
		* @param img the Image
		*/
		static Mat AdaptiveVFilter(Mat img);
	private:
		/// No Construction
		ColorFilter();

	};
}
#endif
