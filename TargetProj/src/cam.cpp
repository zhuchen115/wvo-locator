#include <opencv2/opencv.hpp>
#include "Camera.h"
#include <unistd.h>
#include <time.h>
using namespace cv;
using namespace image_recognize;
int main()
{
Camera cam;
Mat img;
char fname[255];
memset(fname,0,255);
while(1)
{
    img=cam.CapRawImage();
    sprintf(fname,"cap/%d.jpg",time(NULL));
    imwrite(fname,img);
    printf("F%s\n",fname);
    sleep(10);
}


}
