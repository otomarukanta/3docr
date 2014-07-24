#include "feature_extracter.h"
#include "gradient_feature.h"
#include "labeling.h"
#include <algorithm>

void FeatureExtracter::extract(cv::Mat src_img, Eigen::VectorXd &feature) {

	cv::Mat gray_img;
	cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	
	cv::Mat bin_img;
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);

	Labeling labeling(bin_img.cols, bin_img.rows);
	unsigned int *label_img = new unsigned int[bin_img.cols * bin_img.rows];
	unsigned int num_label = labeling.exec(bin_img, label_img);
    std::vector<unsigned int> sum_cnt(num_label, 0);
	for (int p = 0; p < bin_img.cols * bin_img.rows; ++p) {
        if ( bin_img.data[p] != 0) sum_cnt[label_img[p]]++;
	}
	auto max_iter = max_element(sum_cnt.begin(), sum_cnt.end());
	size_t index = std::distance(sum_cnt.begin(), max_iter);
	for (int p = 0; p < bin_img.cols * bin_img.rows; ++p) {
		bin_img.data[p] = (label_img[p] == index ? 255 : 0);
	}
	delete [] label_img;

	if (!GradientFeature(bin_img.data, bin_img.cols, bin_img.rows,
					feature.data(), grid_x, 8)) std::cout << "error." << std::endl;
}
