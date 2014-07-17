#pragma once
#include <opencv2/opencv.hpp>
class Blob {
public:
	Blob() {
		sum = 0;
	};
	unsigned int sum;
	cv::Point_<unsigned int> pt1,pt2;
};
