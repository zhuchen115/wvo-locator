#include "ColorFilter.h"

namespace image_recognize {

	Mat ColorFilter::Gray_therhold(Mat img, uchar min, uchar max)
	{
		Mat out = img.clone(); //Clone the old image
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
			{
				uchar px = img.at<uchar>(i, j);
				if (px <= min || px >= max)
				{
					out.at<uchar>(i, j) = 0;
				}
			}
		return out;
	}

	Mat ColorFilter::AdaptiveVFilter(Mat img)
	{
		Mat hsvImg = img.clone();
		cvtColor(img, hsvImg, CV_BGR2HSV_FULL); // Change Image into HSV coordinate
		const int channels[1] = { 2 }; // Calculate the Histogram of V
		const int histSize[1] = { 256 }; // FULL Value of V
		float hrange[2] = { 0,255 };
		const float* range[1] = { hrange };
		Mat hist,binhist;
		calcHist(&hsvImg, 1, channels, Mat(), hist, 1, histSize, range); // Calculate the histogram
	
		normalize(hist, hist, 0, 1, NORM_MINMAX, -1, Mat()); //Make the histogram into [0 1]
		threshold(hist, binhist, ADAPTIVE_THRESHOLD, 1, THRESH_BINARY); // Make the histogram into binary
		uchar prev[3] = { 0,0,0 }; // Store Previous Value, anti-noise
		bool falling = false;
		int vthresh = 0;
		for (int h = 0; h < binhist.rows; h++)
		{
			uchar binVal = (uchar) binhist.at<float>(h);
			prev[0] = prev[1]; prev[1] = prev[2]; prev[2] = binVal;
			if (prev[0] && prev[1] && prev[2])
				falling = true;
			// Detect the falling edge
			if (falling && (!prev[0]) && (!prev[1]) && (!prev[2]))
			{
				vthresh = h - 2;
				break;
			}
		}
		Vec3b min(0, 0, vthresh), max(255, 255, 255);
		return HSV_threshold(img, min, max);
	}
	ColorFilter::ColorFilter()
	{
	}


	ColorFilter::~ColorFilter()
	{
	}

	Mat ColorFilter::HSV_threshold(Mat img, Vec3b min, Vec3b max)
	{
		assert(img.channels() == 3);
		Mat hsvimg = img.clone(), out = img.clone();
		cvtColor(out, hsvimg, CV_BGR2HSV_FULL);
		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
			{
				Vec3b px = hsvimg.at<Vec3b>(i, j);
				if (px.val[1] < min.val[1] || px.val[1] > max.val[1] ||
					px.val[2] < min.val[2] || px.val[2] > max.val[2])
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}

				if (min.val[0] < max.val[0])
				{
					if (px.val[0] < min.val[0] || px.val[0] > max.val[0])
					{
						out.at<Vec3b>(i, j)[0] = 0;
						out.at<Vec3b>(i, j)[1] = 0;
						out.at<Vec3b>(i, j)[2] = 0;
					}
				}
				else
				{
					if (px.val[0] < min.val[0] && px.val[0] > max.val[0])
					{
						out.at<Vec3b>(i, j)[0] = 0;
						out.at<Vec3b>(i, j)[1] = 0;
						out.at<Vec3b>(i, j)[2] = 0;
					}
				}

			}
		return out;
	}

	Mat ColorFilter::RGB_threshold(Mat in, Vec3b min, Vec3b max)
	{
		assert(in.channels() == 3);
		Mat out = in.clone(); //Clone the old image
		for (int i = 0; i < in.rows; i++)
			for (int j = 0; j < in.cols; j++)
			{
				Vec3b px = in.at<Vec3b>(i, j);
				if (px.val[0] <= min.val[0] || px.val[0] >= max.val[0] ||
					px.val[1] <= min.val[1] || px.val[1] >= max.val[1] ||
					px.val[2] <= min.val[2] || px.val[1] >= max.val[2])
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
		return out;
	}

}