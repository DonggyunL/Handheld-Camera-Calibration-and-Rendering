//  library import
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	// receive pre-recorded video file and convert to image for calibration
	VideoCapture cap(argv[1]);

	if (!cap.isOpened())
	{
		printf("Can't open the camera");
		return -1;
	}
	
	int img_count = 0;

	int CheckerBoard[2]{7,9};

	std::vector<std::vector<cv::Point3f> > objpoints;
	std::vector<std::vector<cv::Point2f> > imgpoints;
	
	std::vector<cv::Point3f> objp;

	for(int i{0}; i<CheckerBoard[1]; i++)
	{
		for(int j{0}; j<CheckerBoard[0]; j++)
			objp.push_back(cv::Point3f(j,i,0));
	}

	cv::Mat img;
	cv::Mat gray_img;

	while (1){
		std::vector<cv::Point2f> corners;
		bool found;

		cap >> img;

		if (img.empty()){
			break;
		}
		img_count++;

		cvtColor(img, gray_img, COLOR_BGR2GRAY);

		found = cv::findChessboardCorners(img, cv::Size(CheckerBoard[0], CheckerBoard[1]), corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
		if(found){
            		cornerSubPix(gray_img, corners, Size(11, 11), Size(-1, -1), TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));
            		drawChessboardCorners(gray_img, cv::Size(CheckerBoard[0], CheckerBoard[1]), corners, found);

			objpoints.push_back(objp);
			imgpoints.push_back(corners);
        }
	    cv::imshow("Image",img);

		if (waitKey(25) == 43)
			break;
	}

	// camera calibration
	cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
    	cv::Mat distCoeffs;
    	std::vector<cv::Mat> rvecs;
    	std::vector<cv::Mat> tvecs;
	

	intrinsic.ptr<float>(0)[0] = 1;
    	intrinsic.ptr<float>(1)[1] = 1;
	
	calibrateCamera(objpoints, imgpoints, gray_img.size(), intrinsic, distCoeffs, rvecs, tvecs);

	// std::cout << "cameraMatrix : " << intrinsic << std::endl;
	// std::cout << "distCoeffs : " << distCoeffs << std::endl;
	// std::cout << "Rotation vector : " << rvecs << std::endl;
	// std::cout << "Translation vector : " << tvecs << std::endl;

	// store camera matrix to file and export

	return 0;
}
