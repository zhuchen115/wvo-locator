#include <opencv2\opencv.hpp>
#include <stdio.h>

using namespace cv;

Point LED_R(Mat img);
Point LED_G(Mat img);
Point LED_B(Mat img);
Mat HSV_therhold(Mat img, Vec3b min, Vec3b max);
void findleds(Mat grayimg, vector<vector<Point>>& contours_out, bool disp= false);
int main()
{
	char fname[256];
	Mat img;
	while (1)
	{
		printf("Input the file name: ");
		scanf_s("%s", fname,255);
		img = imread(fname);
		if (!img.data)
		{
			printf("Invalid File\n");
			continue;
		}
		LED_R(img);
		LED_G(img);
		LED_B(img);
	}
	return 1;
}

Point LED_R(Mat img)
{
	Vec3b min(156,0,0), max(10,200,255);
	Mat f = HSV_therhold(img, min, max);
	//imshow("Therhold", f);
	Mat show(img);
	vector<vector<Point>> contours;
	findleds(f, contours);
	//drawContours(show, contours, -1, Scalar(0,255,0), 1);
	float r;
	Point2f center;
	if (contours.size() != 0)
	{
		minEnclosingCircle(Mat(contours[0]), center, r);
		circle(show, Point(center), static_cast<int>(r), Scalar(0, 0, 255), 2);

		//imshow("LED R", show);
		//waitKey(0);
		return center;
	}
	else {
		printf("Not Found R!\n");
		return NULL;
	}
}

Point LED_G(Mat img)
{
	Vec3b min(35, 0, 100), max(77, 255, 255);
	Mat f = HSV_therhold(img, min, max);
	Mat show(img);
	vector<vector<Point>> contours;
	findleds(f, contours);
	float r;
	Point2f center;
	if (contours.size() != 0)
	{
		minEnclosingCircle(Mat(contours[0]), center, r);
		circle(show, Point(center), static_cast<int>(r), Scalar(0, 255, 0), 2);
		return center;
	}
	else 
	{
		printf("Not Found G \n");
		imshow("Therhold", f);
		findleds(f, contours,true);
		waitKey(0);
		return NULL;
	}
	//imshow("LED G", show);
	//waitKey(0);
	
}

Point LED_B(Mat img)
{
	Vec3b min(85, 0, 0), max(124, 114, 255);
	Mat f = HSV_therhold(img, min, max);
	//imshow("Therhold", f);
	//waitKey(0);
	Mat show(img);
	vector<vector<Point>> contours;
	findleds(f, contours);
	float r;
	Point2f center;
	if (contours.size() != 0)
	{
		minEnclosingCircle(Mat(contours[0]), center, r);
		circle(show, Point(center), static_cast<int>(r), Scalar(255, 0, 0), 2);
		imshow("LED B", show);
		waitKey(0);
		return center;
	}
	else 
	{
		printf("Not Found B\n");
		return NULL;
	}
}

Mat HSV_therhold(Mat img,Vec3b min, Vec3b max)
{
	Mat hsvimg = img.clone(), out = img.clone();;
	cvtColor(out, hsvimg, CV_BGR2HSV);
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			Vec3b px = hsvimg.at<Vec3b>(i, j);
			if (px.val[1] < min.val[1]|| px.val[1] > max.val[1] ||
				px.val[2] < min.val[2] || px.val[2] > max.val[2])
			{
				out.at<Vec3b>(i, j)[0] = 0;
				out.at<Vec3b>(i, j)[1] = 0;
				out.at<Vec3b>(i, j)[2] = 0;
			}
			
			if (min.val[0] < max.val[0])
			{
				if (px.val[0] <= min.val[0] || px.val[0] >= max.val[0])
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
			else
			{
				if (px.val[0] <= min.val[0] && px.val[0] >= max.val[0])
				{
					out.at<Vec3b>(i, j)[0] = 0;
					out.at<Vec3b>(i, j)[1] = 0;
					out.at<Vec3b>(i, j)[2] = 0;
				}
			}
			//int sum = (int)(out.at<Vec3b>(i, j)[0] + (int)out.at<Vec3b>(i, j)[1] + (int)out.at<Vec3b>(i, j)[2]);
			/*if (sum > 700 || sum <100)
			{
				out.at<Vec3b>(i, j)[0] = 0;
				out.at<Vec3b>(i, j)[1] = 0;
				out.at<Vec3b>(i, j)[2] = 0;
			}
			else 
			{
				int mk =0,mv = out.at<Vec3b>(i, j)[0];
				for (int k = 0; k < 3; k++)
				{
					if (mv < out.at<Vec3b>(i, j)[k])
					{
						mv = out.at<Vec3b>(i, j)[k];
						mk = k;
					}
				}
				for (int k = 0; k < 3; k++)
				{
					out.at<Vec3b>(i, j)[k] = (k == mk) ? 255 : 0;
				}

			}*/
		}
	return out;
}

void findleds(Mat imgin, vector<vector<Point>>& contours_out,bool disp)
{
	Mat gray, show(imgin.size(), CV_8UC3);
	vector<vector<Point>> contours;
	Canny(imgin, gray, 20, 600);
	if (disp) {
		namedWindow("LED");
		imshow("LED", gray);
		waitKey(0);
	}
	findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	if (disp)
	{
		printf("Contours Found %d\n", contours.size());
	}
	int mi = -1;
	double am = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = fabs(contourArea(contours.at(i)));
		if (am < area)
		{
			mi = i;
			am = area;
		}
	}
	if(mi>=0)
	contours_out.push_back(contours.at(mi));
}