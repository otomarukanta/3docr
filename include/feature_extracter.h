#pragma once
#include <opencv2/opencv.hpp>
#include <Eigen/Core>

class FeatureExtracter {
public:
	FeatureExtracter(unsigned int gx, unsigned int gy) : grid_x(gx), grid_y(gy) {};
	void extract(cv::Mat image, Eigen::VectorXd &feature);
private:
	unsigned int grid_x;
	unsigned int grid_y;
};
