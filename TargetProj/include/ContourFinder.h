#ifndef CONTOUR_FINDER_H
#define CONTOUR_FINDER_H
#include "config.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace image_recognize {

	/// The infomation of detected object
	struct ObjectInfo {
		float area; //! The area of the detected contours
		int orientation; //!< The detected Orientation (0-360)
		int offset; //!< The distance between image center and object center
		int height; //!< The height of bounded rectangle
		int width; //!< The width of bounded rectangle
	};

	class ContourFinder
	{
	public:
		enum ObjectOrientation {
			O0 = 0,
			O45 = 45,
			O90 = 90,
			O135 = 135,
			O180 = 180
		};
		ContourFinder();
		~ContourFinder();
		/// Load Traing Data from XML Storage file
		void LoadTraining();
		/// Regenerate training XML file
		void InitTraining();
		/// Save the contours training data
		void SaveTraining();

		/**
		* Find Contours and filter by area range
		* @param grayimg The gray image input
		* @param contours_out The found contours
		* @param minarea The mininum area of the contours
		* @param maxarea The maxinum area of the contours
		* @return the number of contours found
		*/
		int FindContoursByArea(Mat grayimg, vector<vector<Point>>& contours_out, float minarea = 1000, float maxarea = 190000);

		/**
		* Find the target from the image (best)
		* @param image CALIBRATED image
		* @param info the detected image
		*/
		int FindTarget(Mat image, ObjectInfo& info);

		/**
		* Get the dimension of the training object
		* @param angle the oritentation angle of the object
		* @param dimension the infomation of the object
		*/
		int GetTrainingDimension(int angle, ObjectInfo& dimension);
	private: 
		vector<vector<Point>> contours_training;
	};
}
#endif
