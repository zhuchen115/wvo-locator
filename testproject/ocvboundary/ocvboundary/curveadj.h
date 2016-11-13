#ifndef _CURVE_ADJ_H_
#define _CURVE_ADJ_H_
#include <math.h>
#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
using namespace cv;
//namespace incv
//{
/// Adjust the image using curves
/// Same as the curve function in photoshop
	class CurveAdj {
	public:
		enum CURVE_CHANNEL
		{
			CURVE_CHANNEL_R = 1,
			CURVE_CHANNEL_G = 2,
			CURVE_CHANNEL_B = 4,
			CURVE_CHANNEL_RGB = 7
		};
		CurveAdj(Mat img);
		CurveAdj();
		void set_control_point(int x, int y);
		void set_control_point(vector<int> x, vector<int> y);
		Mat transform(char channel = CURVE_CHANNEL_RGB);
		Mat transform(Mat imgin, char channel = CURVE_CHANNEL_RGB);
		Mat adaptive_transform(Mat imgin);

	protected:
		/// Calculate the curve for color transform
		vector<int> calc_curve(vector<double> X, vector<double> Y);
	private:
		///The vector to store the curve data
		vector<int> curve;
		Mat orig_image;
	};
//}



#endif // !_CURVE_ADJ_H_