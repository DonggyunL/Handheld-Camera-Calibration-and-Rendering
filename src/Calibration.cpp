//  library import
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int ac, char** av) {

	VideoCapture cap("calibration1.mp4");

	if (!cap.isOpened())
	{
		printf("Can't open the camera");
		return -1;
	}

	Mat img;

	while (1)
	{
		cap >> img;
        
		if (img.empty())
		{
			printf("empty image");
			return 0;
		}
        
		imshow("mp4 for Calibration", img);

		if (waitKey(25) == 27)
			break;
	}

// // initializing

// // receive pre-recorded video file for calibraiton


// // detect checker board


// // camera calibration


// // store camera matrix to file and export

	return 0;
}