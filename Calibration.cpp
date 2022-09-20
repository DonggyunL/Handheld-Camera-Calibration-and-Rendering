// library import
#include "opencv2/opencv.hpp"
#include "opencv2/opencv_modules.hpp"
#include <stdio.h>
#include <iostream>

int main(){
//opencv test
    cv::VideoCapture cap("Calibration1.mp4");

    if (!cap.isOpened()){
        printf("Can't open");
        return -1;
    }
// initializing

// receive pre-recorded video file for calibraiton


// detect checker board


// camera calibration


// store camera matrix to file and export

return 0;
}