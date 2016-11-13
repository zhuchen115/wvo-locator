#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "curveadj.h"
using namespace cv;
Mat rgb_therhold(Mat in);
void sobel_edge(Mat in, Mat &out);
int findcontour(Mat grayimg, vector<vector<Point>>& contours_out);
int main(int argc, char** argv)
{
	char filename[256];
	char* rgbth = "RGB Therhold";
	char* wout = "Detected Edge";
	char* wcontours = "Contours";
	Mat imgin, out;
	if (argc == 1) {
		printf("Input the orignal file name:");
		scanf_s("%s", filename, 255);
		imgin = imread(filename);
	}
	else {
		imgin = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	}
	if (!imgin.data) {
		printf("Cannot Load Image!\n");
		return -1;
	}
	int i = 0;
	Mat imgtf;
	vector<vector<Point>> contours_chairs;
	CurveAdj curve;
	curve.set_control_point(99, 45);
	namedWindow(rgbth, CV_WINDOW_AUTOSIZE);
	/// Define the original contour
	do {
		if (i)
		{
			curve.transform(imgin, CurveAdj::CURVE_CHANNEL_RGB);
		}
		i++;
		imgtf = rgb_therhold(imgin);
		imshow(rgbth, imgtf);
		waitKey(1000);
		cvtColor(imgtf, imgtf, CV_BGR2GRAY);
		findcontour(imgtf, contours_chairs);
	} while (contours_chairs.size() > 7);
	int chair_index = 0;
	///This should seldom happen
	if (contours_chairs.size() > 1) {
		printf("Find too many contours in source image! \n");
		Mat imgchairs(imgtf.size(), CV_8UC3, Scalar(0));
		Scalar colorbar[7] = {Scalar(255,0,0),Scalar(0,255,0),Scalar(0,0,255),Scalar(255,255,0),Scalar(0,255,255),Scalar(255,0,255),Scalar(255,255,255)};
		
		for (int i = 0; i < contours_chairs.size(); i++)
		{
			drawContours(imgchairs, contours_chairs, i, colorbar[i], 2);
		}
		
		namedWindow("Find a chair Contour Manually");
		imshow("Find a chair Contour Manually", imgchairs);
		while(!((chair_index>47)&&(chair_index<56)))
			chair_index = waitKey(0);
		chair_index -= 48;
	}
	Mat timgrslt(imgtf.size(), CV_8U, Scalar(0));
	drawContours(timgrslt, contours_chairs, chair_index, Scalar(255), 2);
	/// Match Targets
	while (1) {
		Mat frslt= timgrslt.clone();
		printf("Input the target file name:");
		scanf_s("%s", filename, 255);
		imgin = imread(filename);
		vector<vector<Point>> contours_match;
		if (!imgin.data) {
			printf("Cannot Load Image!\n");
			return -1;
		}
		i = 0;
		do {
			imgin = curve.adaptive_transform(imgin);
			i++;
			imgtf = rgb_therhold(imgin);
			imshow(rgbth, imgtf);
			waitKey(1000);
			cvtColor(imgtf, imgtf, CV_BGR2GRAY);
			findcontour(imgtf, contours_match);
		} while (contours_match.size() > 17);

		double comres, min_comres = 1000, comres_sum;
		int loc;
		/// Output the Hu difference
		printf("Match Results: \nMethod\t\t\tResult\t\tArea\n");
		for (int i = 0; i < contours_match.size(); i++)
		{
			comres_sum = 0;
			double area = fabs(contourArea(contours_match.at(i)));
			printf("-------------------------------------------------------\nResult No.%d\n", i);
			comres = matchShapes(contours_chairs.at(chair_index), contours_match.at(i), CV_CONTOURS_MATCH_I1, 0);
			printf("CV_CONTOURS_MATCH_I1 : %6.5f\t\t%6.5f\n", comres, area);
			comres_sum += comres;
			comres = matchShapes(contours_chairs.at(chair_index), contours_match.at(i), CV_CONTOURS_MATCH_I2, 0);
			printf("CV_CONTOURS_MATCH_I2 : %6.5f\t\t%6.5f\n", comres, area);
			comres_sum += comres;
			comres = matchShapes(contours_chairs.at(chair_index), contours_match.at(i), CV_CONTOURS_MATCH_I3, 0);
			printf("CV_CONTOURS_MATCH_I3 : %6.5f\t\t%6.5f\n", comres, area);
			comres_sum += comres;
			if (comres_sum < min_comres)
			{
				min_comres = comres_sum;
				loc = i;
			}
		}
		/// This parameter is give out by experiments
		if (min_comres < 1)
		{
			printf("Best Contour: %d, Comres: %f\n", loc, min_comres);

			drawContours(frslt, contours_match, loc, Scalar(255), 2);
			namedWindow(wcontours);
			imshow(wcontours, frslt);
		}
		else
		{
			printf("Contour not found\n");

		}
		if (waitKey(0) == 27) {
			break;
		}
	}
	return 1;
}

#define CHANNEL_R_MIN 88
#define CHANNEL_R_MAX 255
#define CHANNEL_G_MIN 64
#define CHANNEL_G_MAX 255
#define CHANNEL_B_MIN 59
#define CHANNEL_B_MAX 245
#define CHANNEL_R_MIN2 110
#define CHANNEL_R_MAX2 217
#define CHANNEL_G_MIN2 95
#define CHANNEL_G_MAX2 214
#define CHANNEL_B_MIN2 90
#define CHANNEL_B_MAX2 200

/// Filter the image by RGB
Mat rgb_therhold(Mat in)
{
	Mat out(in); ///Clone the old image
	for (int i = 0; i < in.rows; i++)
		for (int j = 0; j < in.cols; j++)
		{
			Vec3b px = in.at<Vec3b>(i, j);
			if (px.val[0] <= CHANNEL_B_MIN || px.val[0] >= CHANNEL_B_MAX ||
				px.val[1] <= CHANNEL_G_MIN || px.val[1] >= CHANNEL_G_MAX ||
				px.val[2] <= CHANNEL_R_MIN || px.val[1] >= CHANNEL_R_MAX)
			{
				out.at<Vec3b>(i, j)[0] = 0;
				out.at<Vec3b>(i, j)[1] = 0;
				out.at<Vec3b>(i, j)[2] = 0;
			}
		}
	return out;
}

/// Edge Detection
/// Function was not in use
void sobel_edge(Mat in, Mat &out)
{
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

	Sobel(in, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
	Sobel(in, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, out);
}

///Find and filter out the contours
int findcontour(Mat grayimg, vector<vector<Point>>& contours_out)
{
	vector<vector<Point>> contours;
	//Mat show(grayimg.size(), CV_8U, Scalar(0));
	contours_out.clear();
	/*namedWindow("Cannay Therholder");

	Canny(grayimg, grayimg,150,300);
	imshow("Cannay Therholder", grayimg);
	waitKey(0);*/
	findContours(grayimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//drawContours(show, contours, -1, Scalar(255), 2);
	//imshow("Cannay Therholder", show);

	printf("\nNumber of contours: %d\n", contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		double area = fabs(contourArea(contours.at(i)));
		///Filter object between 0.7m~5m
		if (area > 7600&&area<190000)
		{
			contours_out.push_back(contours.at(i));
		}
	}
	contours.clear();
	printf("\nNumber of contours Return: %d\n", contours_out.size());
	return contours_out.size();
}