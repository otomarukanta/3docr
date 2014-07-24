#pragma once
#include <string>
#include <opencv2/opencv.hpp>

#include <Eigen/Core>
class Blob {
public:
	Blob() {
		sum = 0;
	};
	cv::Mat image;
	unsigned int sum;
	cv::Point_<unsigned int> pt1,pt2;
	cv::Point_<unsigned int> center;

	unsigned int category;
	Eigen::Matrix<short, 3, 1> angle;
	Eigen::Vector3d normal_vector;
	std::string character;

	unsigned int cluster;
};
