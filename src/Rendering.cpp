//  library import
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	// receive pre-recorded video file
	VideoCapture cap(argv[1]);

	if (!cap.isOpened())
	{
		printf("Can't open the camera");
		return -1;
	}

    //  read camera intrinsic and distCoeffs matrix from the file
    cv::Mat intrinsic, distCoeffs;

    FileStorage fs("camera_matrix.txt",FileStorage::READ);
    fs["intrinsic"] >> intrinsic;
    fs["distCoeffs"] >> distCoeffs;

    std::cout << intrinsic << std::endl;
    std::cout << distCoeffs << std::endl;

    return 0;
}