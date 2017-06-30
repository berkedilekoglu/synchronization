#ifndef INCGRD
#define INCGRD

#include <cv.h>
#include <ml.h>
#include <signal.h>
#include <highgui.h>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <atomic>
#include <time.h> //time
#include "boost/date_time/posix_time/posix_time.hpp" //boost time for better accuracy
//global functions
inline int MsTimeSinceLast(double prevTime){
	return 1000 * ( ((double)cv::getTickCount() - prevTime) / cv::getTickFrequency());
}

inline int FpsToInterval(float fps){
	return 1000.0 / fps;
}

/* This is a boost time object which gives us additional precision */
struct SapientTime{
	boost::posix_time::ptime now;
	std::string	instanceTimestamp;

	std::string Now();
	SapientTime();	//initialise time, including setting the instanceTimestamp
};

#endif
