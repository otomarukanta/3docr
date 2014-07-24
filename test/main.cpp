
#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "feature_extracter.h"
using namespace std;

int main(int argc, char *argv[]) {
	
	cv::Mat img = cv::imread("test.png",1);
	
	FeatureExtracter fex;
	Eigen::VectorXd feature(392);
	fex.extract(img,feature);

	for (int i = 0; i < 32; ++i){
	cout << feature[i] << endl;
	}

}

