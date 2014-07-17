#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "labeling.h"

class Blobs {
public:
	Blobs(unsigned int w, unsigned int h);
	void extract(const cv::Mat src_img);
	int size(void);
	void rectangle(cv::Mat &img);
private:
	unsigned int num_blobs;
	unsigned int width;
	unsigned int height;
	Labeling labeling;
};
