#include <vector>
#include <opencv2/opencv.hpp>
#include "blob.h"
#include "labeling.h"

class BlobExtracter {
public:
	BlobExtracter(unsigned int w, unsigned int h) : width(w), height(h) {
		label_img = new unsigned int[w * h];
		labeling = new Labeling(w, h);
	};
	~BlobExtracter() {
		delete [] label_img;
		delete labeling;
	}
	void extract(const cv::Mat& src_img, std::vector<Blob>& blobs) ;
private:
	Labeling *labeling;
	unsigned int width;
	unsigned int height;
	unsigned int *label_img;
};
