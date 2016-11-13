#include <math.h>
#include "cubicspline.h"
#include "curveadj.h"

CurveAdj::CurveAdj(cv::Mat img) 
{
	orig_image = img;
}

CurveAdj::CurveAdj()
{
}

vector<int> CurveAdj::calc_curve(vector<double> X, vector<double> Y)
{
	tk::spline s;
	s.set_points(X, Y);
	vector<int> out;
	for (int i = 0; i < 256; i++) 
	{
		out.push_back(int(s(i)));
	}
	return out;
}

void CurveAdj::set_control_point(int x, int y)
{
	vector<double> xb, yb;
	xb.push_back(0);
	yb.push_back(0);
	xb.push_back(x);
	yb.push_back(y);
	xb.push_back(255);
	yb.push_back(255);
	/// Make the spline curve
	curve = calc_curve(xb, yb);
}

void CurveAdj::set_control_point(vector<int> x, vector<int> y)
{
	vector<double> xb(x.begin(), x.end());
	vector<double> yb(y.begin(), y.end());
	curve = calc_curve(xb, yb);
}

Mat CurveAdj::transform(char channel)
{
	Mat out(orig_image);
	/// Transform for every pixel
	for (int i = 0; i<orig_image.rows; i++)
		for (int j = 0; j < orig_image.cols; j++)
		{
			Vec3b px = orig_image.at<Vec3b>(i, j);
			/// Red Channel
			if (channel & CURVE_CHANNEL_R == CURVE_CHANNEL_R) 
			{
				out.at<Vec3b>(i, j)[2] = curve[px.val[2]];
			}
			/// Green Channel
			if (channel & CURVE_CHANNEL_G == CURVE_CHANNEL_G)
			{
				out.at<Vec3b>(i, j)[1] = curve[px.val[1]];
			}
			/// Blue Chanel
			if (channel & CURVE_CHANNEL_B == CURVE_CHANNEL_B)
			{
				out.at<Vec3b>(i, j)[0] = curve[px.val[0]];
			}
		}
	return out;
}

Mat CurveAdj::transform(Mat imgin, char channel)
{
	orig_image = imgin;
	return transform(channel);
}

Mat CurveAdj::adaptive_transform(Mat imgin)
{
	Mat grayimg;
	///Throw away color and Calculate histgram
	cvtColor(imgin, grayimg, CV_BGR2GRAY);

	const int channel[1] = { 0 };
	const int histSize[1] = { 256 };
	float hranges[2] = { 0,255 };
	const float *range[] = { hranges };
	MatND hist;
	/// Calculate the histgram
	calcHist(&grayimg, 1, channel, Mat(), hist, 1, histSize, range);
	double maxval, minval;
	Point pmin, pmax;
	minMaxLoc(hist, &minval, &maxval, &pmin, &pmax);
	set_control_point(pmax.y, 10);
	return transform(imgin,CURVE_CHANNEL_RGB);
}
