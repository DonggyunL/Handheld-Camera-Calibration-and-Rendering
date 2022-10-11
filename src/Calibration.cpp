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

	// //  show video
	// Mat img;

	// while (1)
	// {
	// 	cap >> img;
        
	// 	if (img.empty())
	// 	{
	// 		printf("empty image");
	// 		return 0;
	// 	}
        
	// 	imshow("camera img", img);

	// 	if (waitKey(25) == 27)
	// 		break;
	// }
	// return 0;
	
	int img_count = 0;
	int img_sample_max = 10;

	int fps = int(cap.get(CAP_PROP_FPS));
	int frame_diff = fps;
	int cur_frame_diff = frame_diff;

	int CheckerBoard[2]{6,8};

	std::vector<std::vector<cv::Point3f> > objpoints;
	std::vector<std::vector<cv::Point2f> > imgpoints;
	
	std::vector<cv::Point3f> objp;

	//  initiallize CheckerBoard
	for(int i{0}; i<CheckerBoard[1]; i++)
	{
		for(int j{0}; j<CheckerBoard[0]; j++)
			objp.push_back(cv::Point3f(j,i,0));
	}

	cv::Mat img;
	cv::Mat gray_img;

	//  convert video file to image, find checkerboard corners from the image, sample images to be used for calibration, and show
	while (img_count < img_sample_max){
		std::vector<cv::Point2f> corners;
		bool found;

		cap >> img;

		if(img.empty()) break;

		if(++cur_frame_diff <= frame_diff) continue;

		cvtColor(img, gray_img, COLOR_BGR2GRAY);

		found = cv::findChessboardCorners(gray_img, cv::Size(CheckerBoard[0], CheckerBoard[1]), corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
		if(found){
			std::cout << "found image" << std::endl;

			cornerSubPix(gray_img, corners, Size(11, 11), Size(-1, -1), TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));
			drawChessboardCorners(gray_img, cv::Size(CheckerBoard[0], CheckerBoard[1]), corners, found);

			objpoints.push_back(objp);
			imgpoints.push_back(corners);

			img_count++;

			cv::imshow("Image",gray_img);
			if(waitKey(int(500)) == 27) break;

			frame_diff = fps;
			cur_frame_diff = 0;
        }
		else frame_diff += int(fps/10);
	}

	std::cout << "found " << img_count << " images" << std::endl;

	// camera calibration
	cv::Mat intrinsic = cv::Mat(3, 3, CV_32FC1);
	cv::Mat distCoeffs;
	cv::Mat rvecs, tvecs;
	
	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;

	calibrateCamera(objpoints, imgpoints, gray_img.size(), intrinsic, distCoeffs, rvecs, tvecs);

	std::cout << "cameraMatrix : " << intrinsic << std::endl;
	std::cout << "distCoeffs : " << distCoeffs << std::endl;
	// std::cout << "Rotation vector : " << rvecs << std::endl;
	// std::cout << "Translation vector : " << tvecs << std::endl;

	// store camera matrix to file and export
	cv::FileStorage fs("camera_matrix.txt", cv::FileStorage::WRITE);
	fs << "intrinsic" << intrinsic;
	fs << "distCoeffs" << distCoeffs;

	return 0;
}
