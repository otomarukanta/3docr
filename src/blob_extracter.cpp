#include "blob_extracter.h"
#include <iostream>

void BlobExtracter::extract(const cv::Mat& src_img, std::vector<Blob>& blobs) {
	cv::Mat gray_img;
	cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	
	cv::Mat bin_img;
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY_INV|cv::THRESH_OTSU);
// 	cv::imshow("test", bin_img);
	unsigned int num_label = labeling->exec(bin_img, label_img);
// 	cv::Mat test_img = bin_img.clone();
// 	for (int p = 0; p < bin_img.cols * bin_img.rows; ++p) {
// 		test_img.data[p] = label_img[p];
// 	}
// 	cv::imshow("test2", test_img);
	std::vector<Blob> blobs_tmp(num_label);
	blobs.clear();
	blobs.reserve(num_label);
	for (unsigned int y = 0; y < height; ++y) {
		for (unsigned int x = 0; x < width; ++x) {
			unsigned int label = label_img[y*width + x];
			if (label != 0) {
				Blob& b  = blobs_tmp[label];
				if (b.sum == 0) {
					b.pt1 = cv::Point_<unsigned int>(x,y);
					b.pt2 = cv::Point_<unsigned int>(x+1,y+1);
				}else {
					if (b.pt1.x > x) {
					   b.pt1.x = x;
					}
					if (b.pt2.x < x) {
						b.pt2.x = x;
					}
					if (b.pt2.y < y) {
						b.pt2.y = y;
					}
				}
				b.sum++;
			}
		}
	}

	int cluster = 0;
	for (auto it = blobs_tmp.begin(); it != blobs_tmp.end(); ++it) {
		if ( it->sum >= 50 ) {
			it->center = cv::Point_<unsigned int>((it->pt1.x + it->pt2.x) / 2,(it->pt1.y + it->pt2.y)/2);
			it->cluster = cluster;
			cv::Mat roi_img(src_img, cv::Rect(it->pt1, it->pt2));
			it->image = roi_img.clone();
			blobs.push_back(*it);
			cluster++;
		}
	}
}
